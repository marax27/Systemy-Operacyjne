#include <unistd.h>
#include <stdio.h>

int main(void){
	int f;
	f = fork();
	if(!f){
		execl("/bin/ls", "ls", NULL);
		perror("execl");
	}
	f = fork();
	printf("A");
	return 0;
}
