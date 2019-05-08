#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define ec_neg1(X) if((X)==-1){ perror("Error"); exit(-1); }

int main(void){
	int pfd[2];
	ssize_t nread;
	char fdstr[10];

	ec_neg1( pipe(pfd) )
	
	switch(fork()){
	case -1:
		perror("fork failure");
		exit(-1);
		break;
	case 0:
		ec_neg1( close(pfd[1]) )
		snprintf(fdstr, sizeof(fdstr), "%d", pfd[0]);
		execlp("./piperead", "piperead", fdstr, (char*)NULL);
		exit(-1);
		break;
	default:
		ec_neg1( close(pfd[0]) )
		ec_neg1( write(pfd[1], "hello", 6) )
	}

	return 0;
}
