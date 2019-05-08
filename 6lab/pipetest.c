#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define ec_neg1(X) if((X)==-1){ perror("Error"); exit(-1); }

int main(void){
	int pfd[2];
	ssize_t nread;
	char s[100];

	ec_neg1( pipe(pfd) )
	ec_neg1( write(pfd[1], "hello", 6) )
	ec_neg1( nread = read(pfd[0], s, sizeof(s)) )
	if(!nread)
		printf("EOF\n");
	else
		printf("read %ld bytes: %s\n", (long)nread, s);	
	return 0;
}
