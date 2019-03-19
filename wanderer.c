/* Open a file and wander around. */

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#define BUFSIZE 64

int main(int argc, char **argv){
	int fd = -1, res;
	char *fname;
	char buf[BUFSIZE];

	if(argc != 2){
		printf("Usage: %s filename\n", argv[0]);
		return 1;
	}

	fname = argv[1];
	fd = open(fname, O_RDONLY);
	if(fd == -1){
		perror("Failed to open the file");
		return 2;
	}

	res = lseek(fd, -1, SEEK_SET);
	printf("Attempting to read file size: %d\n", res);
	if(res == -1){
		perror("Failed to reach the end of the file");
		return 3;
	}

	res = close(fd);
	if(res == -1){
		perror("Failed to close a file properly");
	}

	return 0;
}
