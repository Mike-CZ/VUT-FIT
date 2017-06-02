SERVER = server
CLIENT = client

CC=g++

CFLAGS=-std=c++98 -pedantic -Wextra -Wall

all: client server

client: client.cpp
	$(CC) $(CFLAGS) client.cpp -o client

server: server.cpp
	$(CC) $(CFLAGS) server.cpp -o server

