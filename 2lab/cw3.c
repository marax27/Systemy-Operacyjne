#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define BUFSIZE 512

const char *fname = "testfile";

void run(){
	int fd1 = -1, fd2 = -1, nbytes = -1;
	char buf[BUFSIZE];
	char readbuf[BUFSIZE];

	// Wypelnij buf danymi.
	for(int i=0; i!=100; ++i)
		buf[i] = 97 + i%26;
	
	// Otworz deskryptory do pliku.
	fd1 = open(fname, O_WRONLY);
	fd2 = open(fname, O_RDONLY);
	if(fd1 == -1 || fd2 == -1){
		perror("open() error");
		return;
	}

	// Dla pewnosci.
	lseek(fd1, 0, SEEK_SET);
	lseek(fd2, 0, SEEK_SET);
	
	// Zapis danych.
	nbytes = write(fd1, buf, 100);
	printf("Zapis: zapisano %d bajtow.\n", nbytes);

	nbytes = read(fd2, readbuf, 100);
	readbuf[nbytes] = '\0';
	printf("1. proba odczytu: odczytano %d bajtow:\n%s\n", nbytes, readbuf);
	
        close(fd1);  //zamnkij deskryptor do zapisu.
	lseek(fd2, 0, SEEK_SET);
	nbytes = read(fd2, readbuf, 100);
	readbuf[nbytes] = '\0';
	printf("2. proba odczytu: odczytano %d bajtow:\n%s\n", nbytes, readbuf);
	
	close(fd2);
	return;
}

void run2(){
	int fd3 = open(fname, O_RDWR);
	int n = -1;
	char buf[BUFSIZE];
	for(int i=0; i!=100; ++i)
		buf[i] = 'A';

	lseek(fd3, 0, SEEK_SET);

	write(fd3, buf, 100);
	printf("Pozycja po zapisie: %ld\n", lseek(fd3, 0, SEEK_CUR));

	read(fd3, buf, 100);
	buf[n] = '\0';
	printf("Odczyt. Aktualna pozycja: %ld. Dane:\n%s\n", lseek(fd3, 0, SEEK_CUR), buf);

	close(fd3);
}

void run3(){
	char buf[BUFSIZE];
	int fd = open(fname, O_RDWR | O_APPEND);
	
	for(int i=0; i!=5; ++i){
		write(fd, "ABCD ", 5);
		lseek(fd, i*10, SEEK_SET);
		buf[ read(fd, buf, 10) ] = '\0';
		printf("[ lseek: %ld | odczyt: %s ]\n", lseek(fd, 0, SEEK_CUR), buf);
	}

	close(fd);
}

int main(int argc, char **argv){
	run3();
	return 0;
}
