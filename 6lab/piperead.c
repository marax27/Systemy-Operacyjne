#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define ec_neg1(X) if((X)==-1){ perror("Error"); exit(-1); }

int main(int argc, char **argv){
	int fd;
	ssize_t nread;
	char s[100];

	fd = atoi(argv[1]);
	printf("Reading file descriptor %d\n", fd);
	ec_neg1( nread = read(fd, s, sizeof(s)) )

	if(!nread)
		printf("EOF\n");
	else
		printf("read %ld bytes: %s\n", (long)nread, s);
	exit(0);
}

