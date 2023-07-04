CC := gcc
CFLAGS := -Wall 

NAME := readBMP
SRC := $(NAME).c
DEST := ./build/$(NAME)

all: $(SRC)
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(DEST)

debug: $(SRC)
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -g -o $(DEST)