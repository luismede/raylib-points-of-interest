CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11

all: main

main: src/main.c
	$(CC) src/main.c -o main $(LDFLAGS) $(CFLAGS)

clean:
	rm -f main
