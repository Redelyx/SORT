/*  Assegnamento Sistemi Operativi e in Tempo Reale
	Alice Cipriani mat. 340403						*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <netdb.h>
#include <time.h>

#include <math.h>

#include "list.h"


#define BUF_SIZE 30


char *host_name = "127.0.0.1"; /* local host */
int port = 8000;


int main(int argc, char *argv[]) 
{
	//dafault value	
	itemType sensor;
	int iter_n;

	srand((unsigned int)time(NULL));

	if (argc < 3) {
		strcpy(sensor.id, "Sensor");		
		iter_n = 1;
	}else{
		strcpy(sensor.id, argv[1]);
		iter_n = atoi(argv[2]);
	}
	
	printf("sensor_id:%s\n Number of measures: %i\n", sensor.id, iter_n);

	for(int i = 0; i<iter_n; i++){

		struct sockaddr_in serv_addr;
		struct hostent* server;	
		
		sensor.temp = ((float)rand()/RAND_MAX)*(float)(40.0) - (float)(5.0);


		if ( ( server = gethostbyname(host_name) ) == 0 ) 
		{
			perror("Error resolving local host\n");
			exit(1);
		}

		bzero(&serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr_list[0]))->s_addr;
		serv_addr.sin_port = htons(port);
		
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


		printf("Sensor %s sending temperature \"%f\"°C to hub...\n", sensor.id, sensor.temp);

		/* This sends the temperature value to the hub */
		if ( send(sockfd, &sensor, sizeof(itemType), 0) == -1 ) 
		{
			perror("Error on send\n");
			exit(1);
		}

		printf("Temperature sent. Waiting for served actuator's number...\n");

		int actuatorsServed;
		if ( recv(sockfd, &actuatorsServed, sizeof(int), 0) == -1 ) 
		{
			perror("Error in receiving response from server\n");
			exit(1);
		}

		printf("\nI served %i actuators.\n", actuatorsServed);
		printf("Disconnecting...\n");

		close(sockfd);
	}	
	

	return 0;
}



