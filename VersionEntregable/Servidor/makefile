OBJ= main.o ParserComandos.o
CC= g++
WARNING= -Wall -Werror -g
LIB=

todo: Obligatorio2

ParserComandos.o: ParserComandos.cpp ParserComandos.h
	$(CC) -c ParserComandos.cpp 

main.o: main.cpp ParserComandos.h
	$(CC) -c main.cpp 

Obligatorio2: $(OBJ)
	$(CC) -pthread $(OBJ) -o Obligatorio2 $(LIB)

clean:
	rm $(OBJ)
	rm Obligatorio2
