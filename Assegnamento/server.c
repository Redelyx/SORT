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

int main(){
	struct sockaddr_in serv_addr_hub;
	struct sockaddr_in cli_addr_hub;
	struct sockaddr_in serv_addr_act;
	struct sockaddr_in cli_addr_act;

	LIST sensorsHistory;
	LIST tmp_history;
	itemType sensor;
	itemType * found;
	sensorsHistory = NewList();
	tmp_history = NewList();

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

	bzero( &serv_addr_hub, sizeof(serv_addr_hub) );

	serv_addr_hub.sin_family = AF_INET;
	serv_addr_hub.sin_addr.s_addr = INADDR_ANY;
	serv_addr_hub.sin_port = htons(portHub);

	// Address bindind to socket
	if ( bind( sockfd, (struct sockaddr *)&serv_addr_hub, sizeof(serv_addr_hub) ) == -1 ) {
		perror("Error on binding");
		exit(1);
	}
	
	// Maximum number of connection kept in the socket queue
	if ( listen( sockfd, 20 ) == -1 ) {
		perror("Error on listen");
		exit(1);
	}

	socklen_t address_size = sizeof( cli_addr_hub );	
	
	while(1) {
		float mean = 0;
		int count = 0;
		float sum = 0;
		tmp_history = DeleteList(tmp_history);

		printf("\n---All set. Waiting for a new connection...\n");
		
		// New connection acceptance		
		int newsockfd_hub = accept(sockfd, (struct sockaddr *)&cli_addr_hub, &address_size );      
		if (newsockfd_hub == -1) 
		{
			perror("Error on accept");
			exit(1);
		}
		
		// Message reception
		if ( recv( newsockfd_hub, &sensor, sizeof(itemType), 0 ) == -1) 
		{
			perror("Error on receive");
			exit(1);
		}

		printf("----Received from hub:\n");
		PrintItem(sensor);

		sensorsHistory = EnqueueFirst(sensorsHistory, sensor);
		tmp_history = sensorsHistory;

		while(!isEmpty(tmp_history)){
			found = Find(tmp_history, sensor);
			if(found != NULL){
				sum += found->temp;
				count++;
			}
			tmp_history = tmp_history->next;
		}

		mean = sum / count;

		int counto = 5;

		/* This sends the number of actuators served by the sensor*/
		printf("-----Sending number of actuators(%i) served by sensor %s\n", count, sensor.id);
		if ( send( newsockfd_hub, &counto, sizeof(int), 0 ) == -1) 
		{
			perror("Error on send");
			exit(1);
		}
		
		close(newsockfd_hub);
	}

	close(sockfd);
	return 0;
}



