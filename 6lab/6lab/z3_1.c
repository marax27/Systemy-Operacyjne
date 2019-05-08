#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define BUFSIZE 512

void try_create_pipe(int pfd[2]);
void close_pipe(int pfd[2]);

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
	int pfd1[2];  /* parent -> child #1 communication */
	int pfd1back[2];  /* child #1 -> parent */
	try_create_pipe(pfd1);
	try_create_pipe(pfd1back);

	/* Create child processes. */
	int pid1, pid2;
	pid1 = fork();
	if(!pid1){
		/*----- Child no. 1 - count all lines -----*/
		close(pfd1[1]);
		close(pfd1back[0]);
		dup2(pfd1back[1], STDOUT_FILENO);

		int all = 0;
		int result = 0;
		char last_char_read = '\0';

		while((nread = read(pfd1[0], buf, sizeof(buf))) > 0){
			/* Count all newlines in the buffer. */
			all += nread;
			for(int i = 0; i != nread; ++i){
				if(buf[i] == '\n')
					++result;
			}
			last_char_read = buf[nread-1];
		}

		/* If newline was the last character in the file, there was
		   no word after it, so there's no need to add 1 to result. */
		printf("%d", (last_char_read == '\n') ? result : (result+1));
		close(pfd1back[1]);
		exit(0);
	}

	int pfd2[2];  /* parent -> child #2 */
	int pfd2back[2];  /* child #2 -> parent */
	try_create_pipe(pfd2);
	try_create_pipe(pfd2back);

	pid2 = fork();
	if(!pid2){
		/*----- Child no. 2 - count lines that contain 'pipe' -----*/
		close_pipe(pfd1);
		close_pipe(pfd1back);
		close(pfd2[1]);
		close(pfd2back[0]);
		dup2(pfd2back[1], STDOUT_FILENO);

		const char *pattern = "pipe";
		int pattern_length = strlen(pattern);
		int pattern_index = 0;
		int result = 0;

		while((nread = read(pfd2[0], buf, sizeof(buf))) > 0){
			/* Naive implementation of pattern searching. */
			for(int i = 0; i != nread; ++i){
				if(buf[i] == pattern[pattern_index]){
					++pattern_index;
					if(pattern_index != pattern_length)
						continue;
					++result;
					
					/* Skip the rest of the current line, in case
					   pattern occurs more than once per line. */
					while(i < nread && buf[i] != '\n')
						++i;
				}
				pattern_index = 0;
			}
		}
		
		printf("%d", result);
		close(pfd1back[1]);
		exit(0);
	}

	/*----- Parent -----*/
	close(pfd1[0]);
	close(pfd2[0]);
	close(pfd1back[1]);
	close(pfd2back[1]);

	while((nread = read(fd, buf, sizeof(buf))) > 0){
		write(pfd1[1], buf, nread);
		write(pfd2[1], buf, nread);
	}
	if(nread == -1){
		perror("dictionary read failure");
		exit(4);
	}

	close(pfd1[1]);
	close(pfd2[1]);

	nread = read(pfd2back[0], buf, sizeof(buf) - 1);
	buf[nread] = '\0';
	printf("Liczba slow z 'pipe': %s\n", buf);

	nread = read(pfd1back[0], buf, sizeof(buf) - 1);
	buf[nread] = '\0';
	printf("Liczba wszystkich slow: %s\n", buf);
}

void try_create_pipe(int pfd[2]){
	if( pipe(pfd) == -1 ){
		perror("pipe failure");
		exit(3);
	}
}

void close_pipe(int pfd[2]){
	close(pfd[0]);
	close(pfd[1]);
}


