/* Get some information about the filesystem. */

#include <stdio.h>
#include <fcntl.h>
#include <sys/statvfs.h>

int main(void){
	struct statvfs data;
	int res = -1;

	res = statvfs("/", &data);
	if(res == -1){
		perror("Failed to obtain filesystem information");
		return 1;
	}

	printf("-=( Filesystem information )=-\n");
	printf("\tBlock size:      %ld\n", data.f_bsize);
	printf("\tFragment size:   %ld\n", data.f_frsize);
	printf("\tFilesystem size: %ld units\n", data.f_blocks);
	printf("\tFree blocks:     %ld\n", data.f_bfree);
	printf("\tFree blocks for the unprivileged: %ld\n", data.f_bavail);

	printf("\t%ld inodes\n", data.f_files);
	printf("\t%ld free inodes\n", data.f_ffree);
	printf("\t%ld free inodes for the unprivileged\n", data.f_favail);

	printf("\tFS ID:               %lu\n", data.f_fsid);
	printf("\tMount flags:       0x%lx\n", data.f_flag);
	printf("\tMax filename length: %lu\n", data.f_namemax);

	return 0;
}
