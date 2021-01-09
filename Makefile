FLAGS_FINAL = -O3 -march=native#-w
FLAGS_AVISO = -Wall -Wextra -Wconversion -pedantic
FLAGS_DEBUG = -ggdb3 -O0
FLAGS_VALGRIND = --leak-check=full --show-leak-kinds=all --track-origins=yes
FLAGS_SAN = -fsanitize=address
FLAGS_LIGA = -lSDL2 -lSDL2_image -lSDL2_ttf #-lm

CC = gcc


EXE = programa # Final
EXE_V = programaV # Valgrind
EXE_S = programaS # Sanitize

FONTE_PROG = $(wildcard *.c) # Caminho da main
OBJ = $(FONTE_ESTR:.c=.o) $(FONTE_PROG:.c=.o)

#--------------

# Final
all: CFLAGS = $(FLAGS_FINAL)
all: $(EXE) 

$(EXE): $(OBJ) 
	$(CC) $^ -o $@ $(FLAGS_LIGA) 

run: all
	./$(EXE)

#--------------

# Sanitize
s: CFLAGS = $(FLAGS_AVISO) $(FLAGS_DEBUG) 
s: $(EXE_S)
	./$(EXE_S) 

$(EXE_S): $(OBJ)
	$(CC) $(FLAGS_SAN) $^ -o $@ $(FLAGS_LIGA) 

#--------------

# Valgrind
v: CFLAGS = $(FLAGS_AVISO) $(FLAGS_DEBUG) 
v: $(EXE_V)
	valgrind $(FLAGS_VALGRIND) ./$(EXE_V)

$(EXE_V): $(OBJ)
	$(CC) $^ -o $@ $(FLAGS_LIGA)

#--------------

# Emscripten
wasm:
	emcc *.c -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[png]' -s USE_SDL_TTF=2 --preload-file recursos -o programa.html

clean:
	rm -f $(OBJ) 
	rm programa*

.PHONY: all clean
