
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>

#define bool int
#define true 1
#define false 0

int main(void)
{
	char url[100], s[500], *path = "", *p;
	int fd;
	ssize_t nread;
	// deklaracja odpowiednich zmiennych
	struct hostent *he;
	struct sockaddr_in addr;

	while(true) {
		printf("\n--------------------\nURL: ");
		if (fgets(url, sizeof(url), stdin) == NULL)
			break;
		if ((p = strrchr(url, '\n')) != NULL)
			*p = '\0';
		if ((p = strchr(url, '/')) != NULL) {
			path = p + 1;
			*p = '\0';
		}

		snprintf(s, sizeof(s), "//%s:80", url);

		// otworz polaczenie z serwerem
		if((he = gethostbyname(url)) == NULL){
			// perror("gethostbyname");
			fprintf(stderr, "Error: Invalid address.\n");
			return 1;
		}
		if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
			perror("socket");
			return 1;
		}

		addr.sin_family = AF_INET;
		addr.sin_port = htons(80);
		addr.sin_addr = *((struct in_addr*)he->h_addr);
		memset(addr.sin_zero, '\0', sizeof addr.sin_zero);

		if (connect(fd, (struct sockaddr *)&addr, sizeof addr) == -1) {
			perror("connect");
			return 1;
		}

		snprintf(s, sizeof(s), "GET /%s HTTP/1.0\r\n\r\n", path);

		// wyslij zadanie do serwera
		if(send(fd, s, strlen(s), 0) == -1){
			perror("send");
			return 1;
		}

		//odczytaj odpowiedz z serwera i przekaz wynik na standardowe wyjscie
		while((nread = recv(fd, s, 500-1, 0))){
			if(nread == -1){
				perror("recv");
				return 1;
			}
			s[nread] = '\0';

			printf(s);
		}
		
		//zamknij polaczenie
		close(fd);
	}
}
