CC = clang++

LIB  = libstd.so

SRC  = $(wildcard src/**/*.cpp) $(wildcard src/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp)
OBJ  = $(SRC:.cpp=.o)
BIN = bin

.PHONY: all run clean test

all: dirs main

dirs:
	mkdir -p ./$(BIN)

run: all
	@LD_LIBRARY_PATH="$$LD_LIBRARY_PATH:$(shell pwd)" $(BIN)/main

test: clean run

main: $(LIB) $(OBJ)
	$(CC) -L. -lstd -o $(BIN)/$@ $^ $(LDFLAGS)

%.so: %.cpp
	$(CC) -fPIC -shared  -o $@ $< -lc $(CFLAGS) -Isrc

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS) -Isrc

clean:
	-rm -rf $(BIN) $(OBJ) $(LIB)
