all:
	flex -o scanner.cpp scanner.ll
	bison -o parser.cpp parser.yy
	g++ -std=c++17 -Wall -O3 -g main.cpp scanner.cpp parser.cpp compiler.cpp sigma_prod.cpp forall.cpp gen_node.cpp -o test -ll 

clean:
	rm -rf scanner.cpp
	rm -rf parser.cpp parser.hpp location.hh position.hh stack.hh
	rm -rf test
