CC = clang
CFLAGS = -g -Wall

SRC = src
BIN = bin
OBJ = obj

SRCS = $(wildcard $(SRC)/*.c)
HDRS = $(wildcard $(SRC)/*.h)
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
BIN = huff

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ)/main.o: $(SRC)/main.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/minheap.o: $(SRC)/minheap.c $(SRC)/minheap.h $(OBJ)/huffman_tree.o
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/huffman_tree.o: $(SRC)/huffman_tree.c $(SRC)/huffman_tree.h $(OBJ)/stack.o
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJ)/* $(BIN)
