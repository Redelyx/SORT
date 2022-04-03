/*  Assegnamento Sistemi Operativi e in Tempo Reale
	Alice Cipriani mat. 340403						*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <netdb.h>

#include "list.h"

#define BUF_SIZE 30

int portHub = 8001;
int portActuator = 8002;

void disconnect(itemType * found, int newsockfd){
	itemType uns_msg;
	strcpy(uns_msg.id, "");
	if ( send( newsockfd, &uns_msg, sizeof(itemType), 0 ) == -1) {
		perror("Error on send");
		exit(1);
	}
	close(newsockfd);
	/*unsubscribe the actuator from every sensor he is listening to*/
}

int main(){
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;

	LIST actuators;
	LIST sensorsHistory;
	LIST tmp;
	LIST knownSensors;
	sensorsHistory = NewList();
	tmp = NewList();
	knownSensors = NewList();
	actuators = NewList();

	itemType msg;
	itemType new_msg;

	itemType * found;

	/*opening socket with hub child*/
	int sockfd = socket( PF_INET, SOCK_STREAM, 0 );  
	if ( sockfd == -1 ) {
		perror("Error opening socket");
		exit(1);
	}
	
	int options = 1;
	if(setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &options, sizeof (options)) < 0) {
		perror("Error on setsockopt");
		exit(1);
	}

	bzero( &serv_addr, sizeof(serv_addr) );

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portHub);

	// Address bindind to socket
	if ( bind( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) == -1 ) {
		perror("Error on binding");
		exit(1);
	}
	
	// Maximum number of connection kept in the socket queue
	if ( listen( sockfd, 20 ) == -1 ) {
		perror("Error on listen");
		exit(1);
	}

	socklen_t address_size = sizeof( cli_addr );	
	
	while(1) {
		float mean = 0;
		int count = 0;
		float sum = 0;
		tmp = DeleteList(tmp);

		printf("\n---All set. Waiting for a new connection...\n");
		
		// New connection acceptance		
		int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &address_size );      
		if (newsockfd == -1) {
			perror("Error on accept");
			exit(1);
		}
		
		// Message reception
		if ( recv( newsockfd, &msg, sizeof(itemType), 0 ) == -1) {
			perror("Error on receive");
			exit(1);
		}
		
		/*The server can receive messages from sensors(through hub's child) or from actuators.
			Also actuators can subscribe or unsubscribe*/
		if(msg.type == ACTUATOR){
			if(msg.mode == UNSUBSCRIBE){
				/*send a msg with an empty string to let the actuator disconnect*/
				found = Find(actuators, msg);
				if(found != NULL){
					disconnect(found, newsockfd);
					while(Find(actuators, msg) != NULL){
						actuators = Dequeue(actuators, msg);
					}
					printf("---Actuator %s unsubscribed.\n", msg.id);
					PrintList(actuators);
				}else{
					printf("!!!Actuator %s not found!\n", msg.id);
				}

				close(newsockfd);
			}else{
				if(Find(actuators, msg) != NULL){
					printf("Actuators already connected!\n ---Sending disconnection signal...---\n\n");
					disconnect(&msg, newsockfd);
				}else{
					msg.sockfd = newsockfd;
					/*Save the socketfd for closing the connection later*/
					int sensorNumber;
					if ( recv( newsockfd, &sensorNumber, sizeof(int), 0 ) == -1) {
						perror("Error on receive");
						exit(1);
					}

					printf("Actuator %s is listening to %i sensors: ", msg.id, sensorNumber);
					for(int i = 0; i<sensorNumber; i++){
						if ( recv( newsockfd, &new_msg, sizeof(itemType), 0 ) == -1) {
							perror("Error on receive");
							exit(1);
						}
						strcpy(msg.sensor, new_msg.id);
						printf(" %s ", new_msg.id);
						actuators = EnqueueFirst(actuators, msg);
						PrintList(actuators);
					}
				}
				printf("\n");
			}

		}else{
			printf("----Received from hub:\n");
			PrintItem(msg);

			/*Retrieve every temperature received from the sensor in the history and calculate mean value to send to actuators listening*/
			sensorsHistory = EnqueueFirst(sensorsHistory, msg);
			tmp = sensorsHistory;

			while(!isEmpty(tmp)){
				found = Find(tmp, msg);
				if(found != NULL){
					sum += found->temp;
					count++;
				}
				tmp = tmp->next;
			}
			msg.temp = sum / count;

			tmp = DeleteList(tmp);
			count = 0;
			tmp = actuators;
			
			while(!isEmpty(tmp)){
				/*send the mean temperature of a sensor to every actuator listening to it*/
				if(strcmp(tmp->item.sensor, msg.id)==0){
					if ( send( tmp->item.sockfd, &msg, sizeof(itemType), 0 ) == -1) 
					{
						perror("Error on send");
						exit(1);
					}
					count++;
				}
				tmp = tmp->next;
			}

			/* This sends the number of actuators served by the sensor*/
			printf("-----Sending number of actuators(%i) served by sensor %s\n", count, msg.id);
			if ( send( newsockfd, &count, sizeof(int), 0 ) == -1) 
			{
				perror("Error on send");
				exit(1);
			}
			
			close(newsockfd);
		}
	}

	close(sockfd);
	return 0;
}



