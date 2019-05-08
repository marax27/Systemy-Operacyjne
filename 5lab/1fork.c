#include <unistd.h>
#include <stdio.h>

int main(void){
	printf("A");
	fork(); fork();
	printf("B");
	return 0;
}