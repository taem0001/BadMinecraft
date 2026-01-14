CC = clang
CXX = clang++
CXXFLAGS = -std=c++20 -g -Wall -Wextra
CXXFLAGS += -Ilib/glad/include 
LDFLAGS = lib/glad/src/glad.o -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa -framework QuartzCore -framework Metal
GLFW_CFLAGS := $(shell pkg-config --cflags glfw3)
GLFW_LIBS   := $(shell pkg-config --libs glfw3)

CXXFLAGS += $(GLFW_CFLAGS)
LDFLAGS += $(GLFW_LIBS)

SRC = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)
OBJ  = $(SRC:.cpp=.o)
BIN = bin

.PHONY: all clean

all: dirs libs game

libs:
	cd lib/glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c

dirs:
	mkdir -p ./$(BIN)

run: all
	$(BIN)/game

game: $(OBJ)
	$(CXX) -o $(BIN)/game -Iinclude $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean: 
	rm -rf $(BIN) $(OBJ)
