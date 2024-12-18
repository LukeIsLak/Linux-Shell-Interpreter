all: ssi

CC = gcc
CFLAGS = -Wall -std=c18 -o

ssi: main.c linkedlists.h token.h background.h command.h helperfunctions.h
	$(CC) $(CFLAGS) ssi main.c -lreadline

clean:
	rm -f *.o
	rm -f ssi
