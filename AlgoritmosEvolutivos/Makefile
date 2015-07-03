GPP=g++
GCC=gcc
FLAGS=  -g2 -Wall -O0 
LIBS= -lGL -lGLU -lglut  
OBJS=Debug/main.o 

all: Debug Debug/AlgoritmosEvolutivos.bin

clean:
	rm -rf ${OBJS} Debug/AlgoritmosEvolutivos.bin

Debug/AlgoritmosEvolutivos.bin: ${OBJS}
	${GPP} ${OBJS} ${LIBS} -o $@

Debug:
	mkdir Debug

Debug/main.o: main.cpp Isla.h Real.h Funcion.h Tour.h TSP.h
	${GPP} ${FLAGS} -c main.cpp -o $@

