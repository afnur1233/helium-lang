CC=clang -std=c23
CFLAGS=-w -lm
DIR_INCLUDE=./include
DIR_BIN=./bin
DIR_SRC=./src
ENTRY_FILE=$(DIR_SRC)/main.c
OUT_EXEC=$(DIR_BIN)/helium

run:
	@$(OUT_EXEC)
build:
	@$(CC) -o $(OUT_EXEC) $(ENTRY_FILE) $(DIR_INCLUDE:%=-I%) $(CFLAGS)