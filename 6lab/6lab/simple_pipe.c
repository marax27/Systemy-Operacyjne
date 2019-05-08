#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	int pfd[2];
	size_t nread;
	char buf[100];

	pipe(pfd);

	/* 1.4. Istotne, gdy zalezy nam na atomicznosci operacji odczytu z bufora. */
	printf("fpathconf: in = %ld, out = %ld\n", fpathconf(pfd[0], _PC_PIPE_BUF), fpathconf(pfd[1], _PC_PIPE_BUF));

	/* 1.3. Create a child process. Child will then send a message to its parent.*/
	switch(fork()){
	case -1:
		perror("fork failure");
		exit(1);
	case 0: /* child process */
		close(pfd[0]);
		const char *child_msg = "Message from the child process.";
		write(pfd[1], child_msg, strlen(child_msg));
		exit(0);
		break;
	default: /* parent process */
		close(pfd[1]);
		printf("Waiting for a message from child...\n");
		nread = read(pfd[0], buf, sizeof(buf));
		(nread!=0)?printf("Received: \"%s\" (%ld bytes)\n",buf,(long)nread):printf("No data\n");
	}

	return 0;
}
