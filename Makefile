FILE=main

########
#   Directories
S_DIR=src
B_DIR=bin

########
#   Output
EXEC=$(B_DIR)/$(FILE)

# default build settings
#CC_OPTS=-c -pipe -Wall -Wno-switch -ggdb -g3 -std=c++17 -fopenmp
CC_OPTS=-c -pipe -Wall -Wno-switch -O3 -ffast-math -std=c++17 -fopenmp
LN_OPTS=
CC=g++

########
#       SDL options
SDL_CFLAGS := $(shell sdl2-config --cflags)
GLM_CFLAGS := -I./glm/
SDL_LDFLAGS := $(shell sdl2-config --libs)
OMP_LDFLAGS = -lgomp

########
#   This is the default action
all:build


########
#   Object list
#
OBJ = $(B_DIR)/$(FILE).o


########
#   Objects
$(B_DIR)/$(FILE).o : $(S_DIR)/$(FILE).cpp
	$(CC) $(CC_OPTS) -o $(B_DIR)/$(FILE).o $(S_DIR)/$(FILE).cpp $(SDL_CFLAGS) $(GLM_CFLAGS)


########
#   Main build rule
build : $(OBJ) Makefile
	$(CC) $(LN_OPTS) -o $(EXEC) $(OBJ) $(SDL_LDFLAGS) $(OMP_LDFLAGS)


clean:
	rm -f $(B_DIR)/*

run:
	make clean
	make
	./bin/main
