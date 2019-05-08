
main: srvfifo cntfifo srvsv cntsv srvpsx cntpsx ;

srvfifo: srvfifo.o
	gcc srvfifo.o -o srvfifo
cntfifo: cntfifo.o
	gcc cntfifo.o -o cntfifo
srvsv: srvsv.o
	gcc srvsv.o -o srvsv
cntsv: cntsv.o
	gcc cntsv.o -o cntsv
srvpsx: srvpsx.o
	gcc srvpsx.o -lrt -o srvpsx
cntpsx: cntpsx.o
	gcc cntpsx.o -lrt -o cntpsx
eliza: eliza.o
	gcc eliza.o -o eliza

srvfifo.o: srvfifo.c fifo.h
	gcc -c srvfifo.c
cntfifo.o: cntfifo.c fifo.h
	gcc -c cntfifo.c
srvsv.o: srvsv.c sv.h
	gcc -c srvsv.c
cntsv.o: cntsv.c sv.h
	gcc -c cntsv.c
srvpsx.o: srvpsx.c psx.h
	gcc -c srvpsx.c -lrt
cntpsx.o: cntpsx.c psx.h
	gcc -c cntpsx.c -lrt

eliza.o: eliza.c
	gcc -c eliza.c

.PROXY:
clean:
	rm *.o srvfifo cntfifo srvsv cntsv srvpsx cntpsx