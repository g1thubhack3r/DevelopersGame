CC := g++
CFLAGS := -std=c++11
PREFIX ?= /usr/local

all: DevelopersGame

DevelopersGame: src/DevelopersGame.cpp 
	mkdir bin
	$(CC) $(CFLAGS) src/DevelopersGame.cpp -o DevelopersGame
	mv DevelopersGame bin/DevelopersGame

install:
	cp bin/DevelopersGame $(PREFIX)/bin/DevelopersGame
	cp include/conio.h $(PREFIX)/include/conio.h

clean:
	rm -rf bin
