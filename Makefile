CC=clang -std=c23
CFLAGS=-w -lm
DIR_INCLUDE=./src
DIR_BIN=./bin
DIR_SRC=./src
DIR_TESTS=./tests
ENTRY_FILE=$(DIR_TESTS)/all.c
OUT_EXEC=$(DIR_BIN)/helium

run:
	@$(OUT_EXEC)
build:
	@mkdir -p $(DIR_BIN)
	@$(CC) -o $(OUT_EXEC) $(ENTRY_FILE) $(DIR_INCLUDE:%=-I%) $(CFLAGS)
clean:
	@mkdir -p $(DIR_BIN)
	@rm -rf $(DIR_BIN)
	@mkdir $(DIR_BIN)