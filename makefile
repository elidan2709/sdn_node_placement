COMPILER=g++
FLAGS = -std=c++11 -O2 -ftrapv -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wmissing-field-initializers -Wcast-qual -Wwrite-strings -Wuninitialized
LINK_COMPILER_FLAGS = -fsanitize=address,undefined -g -o

main: DenseMatrix.o WebGraph.o node.o Printer.o main.o
	$(COMPILER) $(LINK_COMPILER_FLAGS) $@ $^

DenseMatrix.o: DenseMatrix.cpp DenseMatrix.hpp
	$(COMPILER) $(FLAGS) -c DenseMatrix.cpp
node.o: node.cpp
	$(COMPILER) $(FLAGS) -c node.cpp
WebGraph.o: WebGraph.cpp 
	$(COMPILER) $(FLAGS) -c WebGraph.cpp
Printer.o: Printer.cpp
	$(COMPILER) $(FLAGS) -c Printer.cpp
main.o: main.cpp
	$(COMPILER) $(FLAGS) -c main.cpp

clear: 
	rm *.o 
	rm main

clear_csv:
	rm *.csv