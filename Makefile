CC=gcc
OPCOES_COMPILADOR=-I. -I/../SOIL -lglut -lGLU -lGLEW -lGL -lSDL -lSDL_mixer -lSOIL -lm
TODOS_ARQUIVOS_PONTOH =
TODOS_ARQUIVOS_OBJ = main.o glm.o glmimg.o glm_util.o

%.o: ../%.c $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	gcc -o main $^ $(OPCOES_COMPILADOR)

run: all
	./main

clean:
	rm *.o main
