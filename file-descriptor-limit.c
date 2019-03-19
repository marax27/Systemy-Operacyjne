#include <stdio.h>
#include <unistd.h>

int main(void){
	printf("_SC_OPEN_MAX = %1d\n", sysconf(_SC_OPEN_MAX));
	return 0;
}
