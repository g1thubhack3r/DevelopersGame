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
	cp DevelopersGame /usr/bin/DevelopersGame
	cd ../include
	cp conio.h /usr/include/conio.h

clean:
	rm -rf bin
