UNAME_S = $(shell uname -s)

CC = clang++
#CFLAGS = -Isrc
# CFLAGS = -std=c11 -O3 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
# CFLAGS += -Wno-pointer-arith -Wno-newline-eof -Wno-unused-parameter -Wno-gnu-statement-expression
# CFLAGS += -Wno-gnu-compound-literal-initializer -Wno-gnu-zero-variadic-macro-arguments
# CFLAGS += -Ilib/cglm/include -Ilib/glad/include -Ilib/glfw/include -Ilib/stb -Ilib/noise -fbracket-depth=1024
# LDFLAGS = lib/glad/src/glad.o lib/cglm/libcglm.a lib/glfw/src/libglfw3.a lib/noise/libnoise.a -lm

# GLFW required frameworks on OSX
# ifeq ($(UNAME_S), Darwin)
# 	LDFLAGS += -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa
# endif

# ifeq ($(UNAME_S), Linux)
# 	LDFLAGS += -ldl -lpthread
# endif

LIB  = libstd.so

SRC  = $(wildcard src/**/*.cpp) $(wildcard src/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp)
OBJ  = $(SRC:.cpp=.o)
BIN = bin

.PHONY: all run clean test

all: dirs main.out

dirs:
	mkdir -p ./$(BIN)

run: all
	@LD_LIBRARY_PATH="$$LD_LIBRARY_PATH:$(shell pwd)" $(BIN)/main.out

test: clean run

main.out: $(LIB) $(OBJ)
	$(CC) -L. -lstd -o $(BIN)/$@ $^ $(LDFLAGS)

%.so: %.cpp
	$(CC) -fPIC -shared  -o $@ $< -lc $(CFLAGS) -Isrc

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS) -Isrc

clean:
	-rm -rf $(BIN) $(OBJ) $(LIB)
