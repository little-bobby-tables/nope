CXX=g++
DEBUG=-g
CXXFLAGS=-g -std=c++11
SRC=$(wildcard *.cpp)
LANG=lang/ast_visitor.cpp
LANG_GENERATED=lang/parser.o lang/lexer.o
OBJ=$(SRC:%.cpp=%.o) $(LANG:%.cpp=%.o) $(LANG_GENERATED)
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
	 rm -f $(LANG_GENERATED)
	 rm -f $(GENERATED)
	 rm -f $(BIN)

