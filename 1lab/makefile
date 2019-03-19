FLAGS=-Wall -Wextra -ansi -pedantic

compile: ex1open.o t.o
	gcc $(FLAGS) ex1open.o t.o

ex1open.o: ex1open.c t.h
	gcc $(FLAGS) ex1open.c -c -o ex1open.o

t.o: t.c t.h
	gcc $(FLAGS) t.c -c -o t.o

.PHONY: clean
clean: 
	rm *.o a.out


