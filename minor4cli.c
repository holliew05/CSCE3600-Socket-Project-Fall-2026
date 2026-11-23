#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/select.h>

#define MAXLINE 256

int main(int argc, char *argv[]) //arg check
{
	int sockfd;
	char buffer[MAXLINE];
	struct sockaddr_in serveraddr;
	socklen_t len = sizeof(serveraddr);
	struct timeval start_time, end_time;
	double rtt;
	int psent=0;
	int prec=0;
	int plost=0;
	double minrtt=0;
	double maxrtt=0;
	double totalrtt=0;
	double avgrtt=0;
	
	memset(&serveraddr, 0, sizeof(serveraddr)); //initialize server addr
	
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port = htons(atoi(argv[2]));
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	sockfd=socket(AF_INET, SOCK_DGRAM, 0); //create socket

	if(sockfd<0){
		perror("sock failed");
		return 1;
	}

	printf("[client]: UDP socket created successfully.\n"); //socket verification

	char message[]="PING";
	for(int i=0;i<10;i++){ //send 10 pings max
	printf("%d: ", i+1);
	gettimeofday(&start_time, NULL);
	if(sendto(sockfd, message, strlen(message), 0, 
		(struct sockaddr*)&serveraddr, len)<0){
		perror("sendto fail");
		return 1;
	}
	psent++;

	fd_set readfs;
	struct timeval timeout;

	FD_ZERO(&readfs);
	FD_SET(sockfd, &readfs);

	

	int activity;
	timeout.tv_sec =1; //1 second time limit to wait for PONG
	timeout.tv_usec=0;

	activity=select(sockfd+1, &readfs, NULL, NULL, &timeout);


	if(activity<0){ // begin rtt measure
		perror("select failed");
	}
	else if(activity==0){
		printf("Timed out.\n");
		plost++;
	}
	else{
		ssize_t brec;
		brec=recvfrom(sockfd, buffer, sizeof(buffer)-1, 0,
			(struct sockaddr*)&serveraddr, &len);
	if(brec<0){
		perror("recvfrom failed.\n");
	}
	else{ //updating rtt statistics
		buffer[brec]='\0';
		gettimeofday(&end_time, NULL);
		rtt=(end_time.tv_sec - start_time.tv_sec) *1000.0;
		rtt+=(end_time.tv_usec - start_time.tv_usec)/1000.0;
		totalrtt+=rtt;
		if(prec==0){
			minrtt=rtt;
			maxrtt=rtt;
			}
		else{
			if(rtt<minrtt){
				minrtt=rtt;
				}
			if(rtt>maxrtt){
			maxrtt=rtt;
			}
				}
		prec++;
		
		printf("RTT: %.3f ms\n", rtt);
	}
	}
	}
	
	if(prec>0){
		avgrtt=totalrtt/prec;
		}
	double ploss=0;
		if(psent>0){
			ploss=(double)plost/psent*100;
			}
			
	printf("\n~~ PING Statistics ~~\n"); //display summary!
	printf("Packets sent: %d\n", psent);
	printf("Packets received: %d\n", prec);
	printf("Packets lost: %d\n", plost);
	printf("Packet loss: %f\n", ploss);
	
	if(prec>0){
		printf("Minimum RTT: %.3f ms\n", minrtt);
		printf("Maximum RTT: %.3f ms\n", maxrtt);
		printf("Average RTT: %.3f ms\n", avgrtt);
		}
		else{
			printf("No PONG Received.\n");
			}
}
