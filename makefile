# Makefile for CPSC 3600 Final Project

CC=clang
CFLAGS=-Wall -Wextra
SRC=client.c server.c
OBJ=$(addprefix src/, $(SRC:.c=.o))

all: server client

server: server.o 

client: client.o

clean:
	rm -f src/*.o
	rm -f tests/mytestprogram
	rm -f tests/main_test

test:	all
	$(CC) $(CFLAGS) tests/mytestprogram.c -o tests/mytestprogram
	cd tests && ./mytestprogram
test2:
	$(CC) $(CFLAGS) tests/main_test.c -o tests/main_test
	cd tests && LD_PRELOAD=../libmyalloc.so ./main_test
