CC = gcc
FLAGS = -Wall -Werror -Wextra -pedantic
RM = rm
SRC = src/maze.c  src/rotate.c src/floor.c src/init_sdl.c  src/init-texture_path.c src/move.c src/dda.c src/wall.c src/ceil.c 
NAME = maze
LIBDIR = -L .\lib
LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

HEADERS =  -I ./include\SDL2 

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(HEADERS) $(LIBDIR) -o $(NAME) $(SRC) $(LIBS)

clean:
	$(RM)  $(NAME)