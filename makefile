

CC = gcc
FLAGS =
RM = rm
SRC = src/main.c src/drawMap.c src/init_sdl.c 
NAME = run
LIBDIR = -L C:\\mingw_dev_lib\lib 
LIBS = -lmingw32 -lSDL2main -lSDL2
HEADERS =  -I C:\\mingw_dev_lib\include\SDL2 

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(HEADERS) $(LIBDIR) -o $(NAME) $(SRC) $(LIBS)

clean:
	$(RM)  $(NAME)