/* Get some information about the filesystem. */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/statvfs.h>

int main(void){
	char buffer[257];
	int i;
	int res = -1;

	for(i=0; i!=256; ++i)
		buffer[i] = 97 + (i % 26);
	buffer[256] = '\0';

	printf("Attempting to create '%s'...\n", buffer);
	res = open(buffer, O_RDWR, S_IRWXU);
	if(res == -1){
		perror("Failed to create a file");
		return 1;
	}
	close(res);

	return 0;
}
