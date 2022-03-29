#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <netdb.h>

#include "list.h"

#define BUF_SIZE 1000


char *host_name = "127.0.0.1"; /* local host */
int port = 8000;


int main(int argc, char *argv[]){	
	
	itemType msg;
	msg.tipo = TIPO_LETTORE;
	msg.numero_copie = 1;
	int answer;
		
	if(argc < 2){
		printf("Che libro vuoi prendere in prestito? \n");
		char titolo[STRING_SIZE]; 
		scanf("%s", titolo);
		strcpy(msg.titolo, titolo);
	}else{
		strcpy(msg.titolo, argv[1]);
	}
	
		
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


	printf("Invio la richiesta di: ");
	PrintItem(msg);

	/* This sends the name of the book to request  */
	if ( send(sockfd, &msg, sizeof(msg), 0) == -1 ) 
	{
		perror("Error on send\n");
		exit(1);
	}

	printf("Ho inviato la richiesta, rimango in attesa di risposta... \n");
	
	char buf[BUF_SIZE];	
	buf[0]='\0';

	if ( recv(sockfd, &answer, sizeof(&answer), 0) == -1 ) 
	{
		perror("Error in receiving response from server\n");
		exit(1);
	}

	printf("\n---Ricevuto il libro richiesto!---\n");

	close(sockfd);

	return 0;
}



