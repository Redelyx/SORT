/*  Assegnamento Sistemi Operativi e in Tempo Reale
	Alice Cipriani mat. 340403						*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <netdb.h>

#include "list.h"


#define BUF_SIZE 30

char *host_name = "127.0.0.1"; /* local host */
int port = 8001;


int main(int argc, char *argv[]) 
{
	itemType msg;
	itemType actuator;
	actuator.type = ACTUATOR;
	int mode;
	LIST sensorsToListen;
	LIST receivedMeasures;
	sensorsToListen = NewList();
	receivedMeasures = NewList();
	float tGoal;
	int onOffSwitch;

	if (argc < 3) {
		printf("Please insert Actuator id and mode: 0 for unsubscribe, 1 for subscribe.\n");
		exit(0);
	}else{
		strcpy(actuator.id, argv[1]);
		actuator.mode = atoi(argv[2]);		
	}

	if(actuator.mode == UNSUBSCRIBE){
		//Disinscrizione
		//il server chiude la connessione sia con l'attuatore in attesa che con il processo che ha chiesto la disinscrizione
		struct sockaddr_in serv_addr;
		struct hostent* server;	
		
		if ( ( server = gethostbyname(host_name) ) == 0 ) 
		{
			perror("Error resolving local host\n");
			exit(1);
		}

		bzero(&serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr_list[0]))->s_addr;
		serv_addr.sin_port = htons(port);
		
		int sockfd = socket( PF_INET, SOCK_STREAM, 0 );
		if ( sockfd == -1 ) {
			perror("Error opening socket\n");
			exit(1);
		}    

		if ( connect(sockfd, (void*)&serv_addr, sizeof(serv_addr) ) == -1 ) {
			perror("Error connecting to socket\n");
			exit(1);
		}
		
		/*Send actuator name and specs to server*/
		if ( send(sockfd, &actuator, sizeof(itemType), 0) == -1 ) {
			perror("Error on send\n");
			exit(1);
		}

		/*Receive answer from server. If msg.id is an empty string the unsubscribe was successfull*/
		if ( recv(sockfd, &msg, sizeof(itemType), 0) == -1 ) {
			perror("Error in receiving response from server\n");
			exit(1);
		}

		if(strcmp(msg.id, "")==0){
			/*Received empty string. Actuator must disconnect.*/
			printf("---Unsubscribe successfull...\n---Disconnecting...\n");
			close(sockfd);
			exit(0);
		}
	}else{
		//Iscrizione
		/*Insert temperature to achieve for this actuator*/
		printf("Insert Temperature to achieve:\n");
		scanf("%f", &actuator.temp);
		printf("Temperature set to: %f\n", actuator.temp);

		/*Insert list with sensors to listen*/
		printf("Insert Sensors to listen: 	(Terminate with \"exit\" string)\n");
		itemType sensor;
		do{
			scanf("%s", sensor.id);
			printf("---Starting to listen to %s\n", sensor.id);
			if(strcmp(sensor.id, "exit")!=0)
				sensorsToListen = EnqueueFirst(sensorsToListen, sensor);
		}while(strcmp(sensor.id, "exit")!=0);
			
		/*Establish connection with server*/
		struct sockaddr_in serv_addr;
		struct hostent* server;	
		
		if ( ( server = gethostbyname(host_name) ) == 0 ) 
		{
			perror("Error resolving local host\n");
			exit(1);
		}

		bzero(&serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr_list[0]))->s_addr;
		serv_addr.sin_port = htons(port);
		
		int sockfd = socket( PF_INET, SOCK_STREAM, 0 );
		if ( sockfd == -1 ) 
		{
			perror("Error opening socket\n");
			exit(1);
		}    

		if ( connect(sockfd, (void*)&serv_addr, sizeof(serv_addr) ) == -1 ) 
		{
			perror("Error connecting to socket\n");
			exit(1);
		}
		
		/*Send actuator name and specs to server*/
		if ( send(sockfd, &actuator, sizeof(itemType), 0) == -1 ) 
		{
			perror("Error on send\n");
			exit(1);
		}

		int len = getLength(sensorsToListen);
		/*Send number of sensors to listen to*/
		if ( send(sockfd, &len, sizeof(int), 0) == -1 ) 
		{
			perror("Error on send\n");
			exit(1);
		}

		printf("---Actuator specifications sent. Waiting for response...\n");

		LIST tmp_list = sensorsToListen;

		/*Send sensors one by one*/
		for(int i = 0; i < len; i++){
			if ( send(sockfd, &tmp_list->item, sizeof(itemType), 0) == -1 ) 
			{
				perror("Error on send\n");
				exit(1);
			}
			tmp_list = tmp_list->next;
		}
		
		printf("---List of Sensors sent. Waiting for response...\n");

		/*Wait for server to send back sensors temperatures*/
		while(1){
			onOffSwitch = 0;
			tmp_list = DeleteList(tmp_list);
			printf("---All set. Waiting for measures to adjust temperature...\n");

			if ( recv(sockfd, &msg, sizeof(itemType), 0) == -1 ) 
			{
				perror("Error in receiving response from server\n");
				exit(1);
			}

			if(strcmp(msg.id, "")==0){
				/*Received empty string. Actuator must disconnect.*/
				printf("---Disconnecting...\n");
				close(sockfd);
				exit(0);
			}else{
				/*Received new temperature...*/
				itemType * oldMsg = Find(receivedMeasures, msg);
				if(oldMsg){
					/*Received new temperature... from a known sensor. Update the old measure.*/
					printf("---Received temperature: %f from a known Sensor: %s\n", msg.temp, msg.id);
					oldMsg->temp = msg.temp;
				}else{
					/*Received new temperature... from a new sensor. Add new measure.*/
					printf("---Received temperature: %f from new Sensor: %s\n", msg.temp, msg.id);
					receivedMeasures = EnqueueFirst(receivedMeasures, msg);
				}
				tmp_list = receivedMeasures;
				/*Manage the heating on off switch.*/
				for(int i = 0; i < getLength(receivedMeasures); i++){
					if(actuator.temp > tmp_list->item.temp ){
						onOffSwitch++;
					}else{
						onOffSwitch--;
					}
					tmp_list = tmp_list->next;
				}
				if(onOffSwitch >= 0){
					printf("Powering ON heating...");
				}else{
					printf("Powering OFF heating...");
				}
			}
		}
	}
	return 0;
}



