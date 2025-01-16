CC=gcc
CFLAGS=-I D:/SDL_2/SDL_TEMP/src/include
LDFLAGS=-L D:/SDL_2/SDL_TEMP/src/lib
LIBS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer

SRC= 

OBJ=$(SRC:.c=.o)

all: main

main: $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ) main