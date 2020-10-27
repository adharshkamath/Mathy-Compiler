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

BISON_DEBUG = -Wcounterexamples --report=itemset

OUTPUT = test

.PHONY: all clean rebuild install

all: $(PARSER).o $(SCANNER).o $(OBJ) $(MAIN).o
	@echo "\n----- Putting it all together -----"
	@$(CXX) $(CCFLAGS) $^ -o $(OUTPUT) -ll
	@echo "\nExecutable \"$(OUTPUT)\" ready!"
	@touch output.c

$(MAIN).o: $(MAIN).cpp $(patsubst %, %.h, $(ADD_HEADERS))
			@$(CXX) $(CFLAGS) $< -o $@

$(SCANNER).o: $(SCANNER).l $(SCANNER).h $(patsubst %, %.h, $(ADD_HEADERS))
		@echo "\n----- Building lexer -----"
		@$(LEX_TOOL) -o $(SCANNER).$(LANG) $(SCANNER).l
		@$(CXX) $(CFLAGS) $(SCANNER).$(LANG) -o $@
		@echo "\n----- Building utilitites -----"

$(PARSER).o: $(PARSER).yy $(patsubst %, %.h, $(ADD_HEADERS))
		@echo "----- Building parser -----"
		@$(PARSE_TOOL) $(BISON_DEBUG) -o $(PARSER).$(LANG) $(PARSER).yy
		@$(CXX) $(CFLAGS) $(PARSER).$(LANG) -o $@

$(OBJ): %.o: %.h %.cpp
		@$(CXX) $(CFLAGS) $*.cpp -o $@

clean:
	@rm -f scanner.cpp parser.cpp parser.hpp location.hh position.hh stack.hh test compiler
	@rm -f *.o *.out *.s *.i *.gch *.output

rebuild: clean all

install: all 
		@rm -f *.o *.out *.s *.i *.gch *.output scanner.cpp parser.cpp parser.hpp location.hh position.hh stack.hh