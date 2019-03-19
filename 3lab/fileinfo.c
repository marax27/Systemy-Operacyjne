/* Get metadata of a specific file. */

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/statvfs.h>

int main(int argc, char **argv){
	struct stat info;
	int i, res;

	for(i = 1; i < argc; ++i){
		char *fname = argv[i];
		res = stat(fname, &info);

		if(res == -1)
			fprintf(stderr, "Failed to process '%s': %s\n", fname, strerror(errno));
		else{
			printf("-=( %s )=-\n", fname);
			printf("\tDevice ID: %lu\n", info.st_dev);
			printf("\tinode: %lu\n", info.st_ino);
			printf("\tSize: %lu\n\n", info.st_size);
		}
	}

	return 0;
}
