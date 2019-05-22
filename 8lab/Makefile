CC=gcc -Wall

zad4_2: z4.2-server.c z4.2-client.c
	$(CC) z4.2-server.c -o server
	$(CC) z4.2-client.c -o client

zad4_3: z4.3-server.c z4.3-client.c
	$(CC) z4.3-server.c -o server
	$(CC) z4.3-client.c -o client

zad5_2: z5.2-browser.c
	$(CC) z5.2-browser.c -o browser

.PHONY:
clean:
	rm -f server client browser
