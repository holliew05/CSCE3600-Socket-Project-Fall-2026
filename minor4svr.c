#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

#define MAXLINE 256 //maxline declaration

int main(int argc, char *argv[]) // arguments
{
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0); //create socket

	if(sockfd<0){
		perror("socket creation failed");
		return 1;
	}
	
	struct sockaddr_in server_addr;
	int port = atoi(argv[1]);

	memset(&server_addr, 0, sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);

	if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr))<0) //bind socket to port
	{
		perror("bind");
		return 1;
	}

	printf("[server]: ready to accept data...\n"); // waiting for messages

	char buffer[MAXLINE];
	struct sockaddr_in client_addr;
	socklen_t client_len;
	int brec;
	int mcount = 0;
	struct in_addr client_ip;
	srand(time(NULL));
	while(1){ //looping indefinitely
		client_len = sizeof(client_addr);
		brec = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0, 
				(struct sockaddr*)&client_addr, &client_len);
		if(brec<0){
			perror("recvfrom");
		}
		buffer[brec]='\0';
		printf("[client]: %s\n", buffer); //display PING
		
		
	
	int status = rand()%10; //simulating packet loss -> rand, 30% of packets lost on average. 

	if (status<3){
		printf("[server]: dropped packet\n");
		continue;
	}
		
	
	mcount++;
	client_ip=client_addr.sin_addr;
	

	const char *reply="PONG";
	
	if(sendto(sockfd, reply, strlen(reply), 0,
		(struct sockaddr*)&client_addr, client_len) <0){
		perror("sendto fail");
	}
	}

	

	return 0;
}
