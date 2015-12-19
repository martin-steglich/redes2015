OBJ= main.o ParserComandos.o
CC= g++
WARNING= -Wall -Werror -g
LIB=

todo: Servidor

ParserComandos.o: ParserComandos.cpp ParserComandos.h
	$(CC) -c ParserComandos.cpp 

main.o: main.cpp ParserComandos.h
	$(CC) -c main.cpp 

Servidor: $(OBJ)
	$(CC) -pthread $(OBJ) -o Servidor $(LIB)

clean:
	rm $(OBJ)
	rm Servidor
