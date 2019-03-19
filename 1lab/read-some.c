#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUFSIZE 16

int main(int argc, char **argv){
	int f, x, bytesToRead;
	char buf[BUFSIZE];

	if(argc != 2){
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		return 1;
	}

	/* Open the file. */
	f = open(argv[1], O_RDONLY);
	if(f == -1){
		fprintf(stderr, "Failed to open '%s'. Error: %s\n", argv[1], strerror(errno));
		return 2;
	}

	/* Read some data. */
	bytesToRead = BUFSIZE;
	x = read(f, buf, bytesToRead);
	if(x == -1){
		fprintf(stderr, "Failed to read from '%s'. Error: %s\n", argv[1], strerror(errno));
		return 3;
	}

	/* Display data. */
	printf("Data: ");
	write(1, buf, x);

	x = close(f);
	if(x == -1)
		perror("Failed to close file descriptor properly");

	return 0;
}
