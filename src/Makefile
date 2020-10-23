all: scanner parser
	g++ -std=c++17 -Wall -O3 -g main.cpp scanner.cpp parser.cpp compiler.cpp sigma_prod.cpp forall.cpp gen_node.cpp -o test -ll 

scanner: scanner.l
		flex -o scanner.cpp scanner.l

parser: parser.yy
		bison -o parser.cpp parser.yy

clean_scanner: scanner.cpp
			   rm -rf scanner.cpp	

clean_parser: parser.cpp parser.hpp 
			   rm -rf parser.cpp parser.hpp location.hh position.hh stack.hh	

clean_all:
	rm -f scanner.cpp
	rm -f parser.cpp parser.hpp location.hh position.hh stack.hh
	rm -f test
