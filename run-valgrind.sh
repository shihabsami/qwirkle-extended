make clean
make
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./qwirkle