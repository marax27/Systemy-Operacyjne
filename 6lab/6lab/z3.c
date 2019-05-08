#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define BUFSIZE 512


int main(int argc, char **argv){
	int fd;
	ssize_t nread;
	char buf[BUFSIZE];

	/* Argv check. */
	if(argc != 2){
		printf("Usage: %s filename\n", argv[0]);
		return 1;
	}

	/* Open a dictionary. */
	fd = open(argv[1], O_RDONLY);
	if(fd == -1){
		perror("Failed to open file");
		return 2;
	}

	/* Pipes. */
	int pfd1[2];
	int pfd2[2];
	if(pipe(pfd1) == -1){
		perror("pipe() failure");
		return 3;
	}
	if(pipe(pfd2) == -1){
		perror("pipe() failure");
		return 3;
	} 

	/* Create child processes. */
	int pid;
	pid = fork();
	if(!pid){
		/* Child no. 1 */
		dup2(pfd1[0], 0);
		close(pfd1[0]);
		close(pfd1[1]);
		close(pfd2[0]);
		close(pfd2[1]);

		while((nread = read(pfd1[0], buf, sizeof(buf))) > 0){
			/* TODO */
		}

		write(2, "Child no.1", 10);
		exit(0);

	}else{
		pid = fork();
		if(!pid){
			/* Child no. 2 */
			dup2(pfd2[0], 0);
			close(pfd1[0]);
			close(pfd1[1]);
			close(pfd2[0]);
			close(pfd2[1]);
			while((nread = read(pfd2[0], buf, sizeof(buf))) > 0){
				/* TODO */
			}

			write(2, "Child no.2", 10);
			exit(0);
		}else{
			/* Parent */
			close(pfd1[0]);
			close(pfd2[0]);

			while((nread = read(fd, buf, sizeof(buf))) > 0){
				write(pfd1[1], buf, nread);
				write(pfd2[1], buf, nread);
				write(2, "() ", 3);
			}
			if(nread == -1){
				perror("dictionary read failure");
				exit(4);
			}

			close(pfd1[1]);
			// read(pfd1[0], buf, sizeof(buf));
			// printf("Liczba wszystkich slow: %s\n", buf);

			close(pfd2[1]);
			// read(pfd2[0], buf, sizeof(buf));
			// printf("Liczba slow z 'pipe': %s\n", buf);

			write(2, "xxx\n", 4);
			sleep(5);
			write(2, "yyy\n", 4);
		}
	}

	close(fd);
	return 0;
}


