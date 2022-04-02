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

char *host_name = "127.0.0.1"; /* local host */

int port_server = 8000;
int port_client = 8001;

int clientHub(itemType sensor){
		struct sockaddr_in serv_addr;
		struct hostent* server;	
		int actuatorsNumber = 0;

		if ( ( server = gethostbyname(host_name) ) == 0 ) 
		{
			perror("Error resolving local host\n");
			exit(1);
		}

		bzero(&serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr_list[0]))->s_addr;
		serv_addr.sin_port = htons(port_client);
		
		int sockfd = socket(PF_INET, SOCK_STREAM, 0);
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


		printf("---Sensor %s sending temperature \"%f\"°C through Hub child process to Server...\n", sensor.id, sensor.temp);

		if ( send(sockfd, &sensor, sizeof(itemType), 0) == -1 ) 
		{
			perror("Error on send\n");
			exit(1);
		}

		printf("----Temperature sent. Waiting for number of actuators served from Server...\n");

		if ( recv(sockfd, &actuatorsNumber, sizeof(int), 0) == -1 ) 
		{
			perror("Error in receiving response from server\n");
			exit(1);
		}

		printf("\n------Sensor %s served %i actuators.\n", sensor.id, actuatorsNumber);

		close(sockfd);

		return actuatorsNumber;
}

int main(int argc, char *argv[]) 
{
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;

	int sensors_n;
	LIST sensors;
	LIST connected_sensors;
	itemType sensor;
	sensor.type = SENSOR;
	sensors = NewList();
	connected_sensors = NewList();

	if (argc < 2) 
		sensors_n = 1;
	else{
		sensors_n = atoi(argv[1]);
	}
	printf("---Waiting for %i sensor(s) to send temperature value(s).\n", sensors_n);

	// Socket opening
	int sockfd = socket( PF_INET, SOCK_STREAM, 0 );  
	if ( sockfd == -1 ) 
	{
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
	serv_addr.sin_port = htons(port_server);

	// Address bindind to socket
	if ( bind( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) == -1 ) 
	{
		perror("Error on binding");
		exit(1);
	}
	
	// Maximum number of connection kept in the socket queue
	if ( listen( sockfd, 20 ) == -1 ) 
	{
		perror("Error on listen");
		exit(1);
	}

	socklen_t address_size = sizeof( cli_addr );	
	
   	//char buf[BUF_SIZE];	

	while(1) 
	{
		printf("\n%i: ", getpid());
		printf("----Waiting for a new connection...\n");

		/*Accept new connection from Sensor*/		
		int conn_fd = accept(sockfd, (struct sockaddr *)&cli_addr, &address_size );
		int pid;

		if(conn_fd == -1){
			perror("accept error");
			exit(1);
		}

		/*Receive sensor specs*/
		if ( recv( conn_fd, &sensor, sizeof(itemType), 0 ) == -1) 
		{
			perror("Error on receive");
			exit(1);
		}

		sensor.served_n = 0;
		sensor.sockfd = conn_fd;

		printf("\n%i: ", getpid());
		printf("-----Received: \n");
		PrintItem(sensor);

		itemType* tmp_item = Find(sensors, sensor);
		if(!tmp_item){
			/*New sensor: insert in the known sensors list and in the connectedSensor list*/
			sensor.served_n++;
			sensors = EnqueueLast(sensors, sensor);
			connected_sensors = EnqueueFirst(connected_sensors, sensor);
			printf("new sensor! - sensor: %s  served (times): %i\n", sensor.id, sensor.served_n);
		}else{
			/*Known sensor: insert in the connectedSensor list, retreiving the old priority*/
			tmp_item->sockfd = sensor.sockfd;
			tmp_item->temp = sensor.temp;
			tmp_item->served_n++;
			connected_sensors = EnqueueOrdered(connected_sensors, *tmp_item);
			printf("sensor found - sensor: %s  served (times): %i\n", sensor.id, sensor.served_n);
		}
		int len = getLength(connected_sensors);
		if(len >= sensors_n){
			/* Fork to handle connection */
			LIST tmp = connected_sensors;
			do{
				int actuatorsNumber;
				if ( (pid = fork()) < 0 ){
					perror("fork error");
					exit(-1);
				}
				if (pid == 0) { 
					/* child servicing the request from conn_fd */ 
					printf("\nChild - %i: ", getpid());
					printf("...Serving sensor: %s\n", tmp->item.id);
					actuatorsNumber = clientHub(tmp->item);
					printf("-----Act: %i\n", actuatorsNumber);

					/* Send back to sensors the number of actuator served */
					if ( send(tmp->item.sockfd, &actuatorsNumber, sizeof(int), 0 ) == -1) 
					{
						perror("Error on send");
						exit(1);
					}

					close(tmp->item.sockfd);
					exit(0);
				}
				tmp = tmp ->next;
            }while (!(tmp == NULL));
			connected_sensors = DeleteList(connected_sensors);
		}

	}
	return 0;
}



