CC=gcc
BDIR=./bin
NAME=Connect4

Connect4: main.c
	$(CC) -o $(NAME) main.c
	clear
