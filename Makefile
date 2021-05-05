.default: all

all: qwirkle

clean:
	rm -rf qwirkle *.o *.dSYM

qwirkle: Tile.o Node.o LinkedList.o TileBag.o qwirkle.o	PlayerHand.o
	g++ -Wall -Werror -std=c++14 -g -o $@ $^

%.o: %.cpp
	g++ -Wall -Werror -std=c++14 -g -c $^
