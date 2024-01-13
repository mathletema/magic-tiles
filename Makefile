CC = g++
CFLAGS = -I.\include -L.\lib -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer

default:
	$(CC) main.cpp $(CFLAGS) -o main

vamp:
	$(CC) vamp_test.cpp $(CFLAGS) -o vamp.exe