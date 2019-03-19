#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "t.h"
 
#define BUFSIZE 1024
 
int main (int argc, char **argv) {
    int f1, c, res;
    char b[BUFSIZE], *n1;
 
    c = 10;
    n1 = argv[1];

    timestart();
 
    f1 = open (n1, O_RDONLY);
    if(f1 == -1){
        perror("Otwarcie pliku zakonczone niepowodzeniem");
        return 1;
    }

    res = read (f1, b, c);
    if(res == -1){
        perror("Wczytanie danych z pliku nie powiodlo sie");
        return 1;
    }

    timestop("Mymessage");

    b[res] = '\0';
    printf("%s: Przeczytano %d znakow z pliku %s: \"%s\"\n",
	   argv[0], c, n1, b);

    close(f1); 
    return(0);
}
