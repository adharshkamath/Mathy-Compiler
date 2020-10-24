LANG = cpp

CXX = g++

CFLAGS = -std=c++17 -c

CCFLAGS = -std=c++17 -Wall -O3 -g

DEPS = compiler.o sigma_prod.o forall.o gen_node.o helper.o main.o

SCANNER = scanner

PARSER = parser

LEX_TOOL = flex

PARSE_TOOL = bison

OUTPUT = test

.PHONY: all clean

all: $(PARSER).o $(SCANNER).o $(DEPS)
	$(CXX) $(CCFLAGS) $^ -o $(OUTPUT) -ll


$(SCANNER).o: $(SCANNER).l
		$(LEX_TOOL) -o $(SCANNER).$(LANG) $(SCANNER).l
		$(CXX) $(CFLAGS) $(SCANNER).$(LANG) -o $@

$(PARSER).o: $(PARSER).yy
		$(PARSE_TOOL) -o $(PARSER).$(LANG) $(PARSER).yy
		$(CXX) $(CFLAGS) $(PARSER).$(LANG) -o $@

$(DEPS): %.o: %.cpp
		$(CXX) $(CFLAGS) $^ -o $@

clean:
	rm -f scanner.cpp parser.cpp parser.hpp location.hh position.hh stack.hh test compiler
	rm -f *.o *.out *.s *.i *.gch