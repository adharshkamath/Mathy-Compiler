LANG = cpp

CXX = g++

CFLAGS = -std=c++17 -c

CCFLAGS = -std=c++17 -Wall -O3 -g

OBJ = compiler.o sigma_prod.o forall.o gen_node.o helper.o

MAIN = main

ADD_HEADERS = bounds files

SCANNER = scanner

PARSER = parser

LEX_TOOL = flex

PARSE_TOOL = bison

OUTPUT = test

.PHONY: all clean rebuild

all: $(PARSER).o $(SCANNER).o $(OBJ) $(MAIN).o
	$(CXX) $(CCFLAGS) $^ -o $(OUTPUT) -ll

$(MAIN).o: $(MAIN).cpp $(patsubst %, %.h, $(ADD_HEADERS))
			$(CXX) $(CFLAGS) $< -o $@

$(SCANNER).o: $(SCANNER).l $(SCANNER).h $(patsubst %, %.h, $(ADD_HEADERS))
		$(LEX_TOOL) -o $(SCANNER).$(LANG) $(SCANNER).l
		$(CXX) $(CFLAGS) $(SCANNER).$(LANG) -o $@

$(PARSER).o: $(PARSER).yy $(patsubst %, %.h, $(ADD_HEADERS))
		$(PARSE_TOOL) -o $(PARSER).$(LANG) $(PARSER).yy
		$(CXX) $(CFLAGS) $(PARSER).$(LANG) -o $@

$(OBJ): %.o: %.h %.cpp
		$(CXX) $(CFLAGS) $*.cpp -o $@

clean:
	rm -f scanner.cpp parser.cpp parser.hpp location.hh position.hh stack.hh output.c test compiler
	rm -f *.o *.out *.s *.i *.gch

rebuild: clean all