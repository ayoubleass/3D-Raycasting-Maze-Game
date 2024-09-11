CC = gcc
FLAGS = -Wall -Werror -Wextra -pedantic
RM = rm
SRC = src/1-texture.c  src/rotate.c src/essential.c src/init_sdl.c  src/init-texture_path.c src/move.c
NAME = maze
LIBDIR = -L .\lib
LIBS = -lmingw32 -lSDL2main -lSDL2
HEADERS =  -I ./include\SDL2 

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(HEADERS) $(LIBDIR) -o $(NAME) $(SRC) $(LIBS)

clean:
	$(RM)  $(NAME)