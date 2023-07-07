CC := g++
CFLAGS := -std=c++11

all: DevelopersGame

DevelopersGame: DevelopersGame.cpp 
	mkdir bin
	cd src
	$(CC) $(CFLAGS) DevelopersGame.cpp -o DevelopersGame
	mv DevelopersGame ../bin/DevelopersGame

install:
	cd bin
	mv DevelopersGame /usr/bin/DevelopersGame
