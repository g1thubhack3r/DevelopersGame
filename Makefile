CC := g++
CFLAGS := -std=c++11

all: DevelopersGame

DevelopersGame: src/DevelopersGame.cpp 
	mkdir bin
	$(CC) $(CFLAGS) src/DevelopersGame.cpp -o DevelopersGame
	mv DevelopersGame bin/DevelopersGame

install:
	cp bin/DevelopersGame /usr/bin/DevelopersGame
	cp include/conio.h /usr/include/conio.h

clean:
	rm -rf bin
