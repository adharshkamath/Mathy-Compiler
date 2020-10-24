all: parser scanner compiler sigma_prod forall gen_node helper main
	g++ -std=c++17 -Wall -O3 -g main.o scanner.o parser.o compiler.o sigma_prod.o forall.o gen_node.o helper.o -o test -ll

main: main.cpp
	  g++ -std=c++17 -c main.cpp -o main.o

scanner: scanner.l
		flex -o scanner.cpp scanner.l
		g++ -std=c++17 -c scanner.cpp -o scanner.o

parser: parser.yy
		bison -o parser.cpp parser.yy
		g++ -std=c++17 -c parser.cpp -o parser.o

compiler: compiler.cpp
		  g++ -std=c++17 -c compiler.cpp -o compiler.o

sigma_prod: sigma_prod.cpp
			g++ -std=c++17 -c sigma_prod.cpp -o sigma_prod.o

forall: forall.cpp
		g++ -std=c++17 -c forall.cpp -o forall.o

gen_node: gen_node.cpp
		  g++ -std=c++17 -c gen_node.cpp -o gen_node.o

helper: helper.cpp
		g++ -std=c++17 -c helper.cpp -o helper.o

clean:
	rm -f scanner.cpp parser.cpp parser.hpp location.hh position.hh stack.hh test compiler
	rm -f *.o *.out *.s *.i *.gch