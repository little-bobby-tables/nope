CXX=g++
DEBUG=-g
CXXFLAGS=-g -std=c++14
SRC=$(wildcard *.cpp)
VM=vm/ast_instruction_builder.cpp vm/loop.cpp
OBJ_GENERATED=lang/parser.o lang/lexer.o
OBJ=$(SRC:%.cpp=%.o) $(VM:%.cpp=%.o) $(OBJ_GENERATED)
BIN=peridot

GENERATED=lang/lexer.yy.cc lang/parser.tab.cc lang/parser.tab.hh lang/location.hh lang/position.hh lang/stack.hh lang/parser.output

all: $(OBJ)
	$(CXX) -o $(BIN) $^

%.o: %.c
	$(CXX) $@ -c $<

parser: lang/peridot.yy
	bison -o lang/parser.tab.cc -d -v lang/peridot.yy
	$(CXX) $(CXXFLAGS) -c -o lang/parser.o lang/parser.tab.cc

lexer: lang/peridot.l
	flex --outfile=lang/lexer.yy.cc  $<
	$(CXX) $(CXXFLAGS) -c -o lang/lexer.o lang/lexer.yy.cc

clean:
	 rm -f *.o
	 rm -f vm/*.o
	 rm -f lang/*.o
	 rm -f $(GENERATED)
	 rm -f $(BIN)

