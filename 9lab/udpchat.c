/*
** talker.c -- a datagram "client" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFSIZE 1024

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in addr; // connector's address information
	struct hostent *he;
	int numbytes;
	socklen_t addr_len;

	if (argc != 4) {
		fprintf(stderr,"usage: %s hostname yourPort otherPort\n", argv[0]);
		exit(1);
	}

	if ((he=gethostbyname(argv[1])) == NULL) {  // get the host info
		perror("gethostbyname");
		exit(1);
	}

	addr_len = sizeof addr;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	char buffer[BUFSIZE];
	int pid = fork();

	if(pid > 0){  //parent - writer.

		int other_port_number = atoi(argv[3]);

		addr.sin_family = AF_INET;	 // host byte order
		addr.sin_port = htons(other_port_number); // short, network byte order
		addr.sin_addr = *((struct in_addr *)he->h_addr);
		memset(addr.sin_zero, '\0', sizeof addr.sin_zero);

		while(strcmp(buffer, "bye\n")){
			size_t maxdatasize = BUFSIZE;
			char *b = buffer;
			numbytes = getline(&b, &maxdatasize, stdin);

			// if ((numbytes = sendto(sockfd, buffer, numbytes, 0,
					// (struct sockaddr *)&addr, addr_len)) == -1) {
			if((numbytes = write(sockfd, buffer, numbytes)) == -1){
				perror("sendto");
				close(sockfd);
				exit(1);
			}

			printf("[snt]: %s\n", buffer);
		}

		if(kill(pid, SIGTERM) == -1){
			perror("Kill");
			close(sockfd);
			exit(1);
		}

	}else if(!pid){  //child - listener.

		int your_port_number = atoi(argv[2]);

		addr.sin_family = AF_INET;		 
		addr.sin_port = htons(your_port_number);
		addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
		memset(addr.sin_zero, '\0', sizeof addr.sin_zero);

		if (bind(sockfd, (struct sockaddr *)&addr, addr_len) == -1) {
			perror("bind");
			exit(1);
		}

		// Infinite loop; parent is responsible for child's death.
		while(1){
			if((numbytes = read(sockfd, buffer, BUFSIZE-1)) == -1){
			//recvfrom(sockfd, buffer, BUFSIZE-1, 0,
				// (struct sockaddr*)&addr, &addr_len)) == -1){
				perror("recvfrom");
				close(sockfd);
				exit(1);
			}

			buffer[numbytes] = '\0';
			printf("[rcvd]: %s", buffer);
		}

	}else{
		perror("fork");
		close(sockfd);
		exit(1);
	}

	close(sockfd);

	return 0;
}
