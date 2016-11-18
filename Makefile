CXX=g++
DEBUG=-g
CXXFLAGS=-g -std=c++11
SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o), $(LANG)

GENERATED=lang/lexer.yy.cc lang/parser.tab.cc lang/parser.tab.hh lang/location.hh lang/position.hh lang/stack.hh lang/parser.output

all: $(OBJ)
	$(CXX) -o $(BIN) $^

%.o: %.c
	$(CXX) $@ -c $<

parser: lang/nope.yy
	bison -o lang/parser.tab.cc -d -v lang/nope.yy
	$(CXX) $(CXXFLAGS) -c -o parser.o lang/parser.tab.cc

lexer: lang/nope.l
	flex --outfile=lang/lexer.yy.cc  $<
	$(CXX) $(CXXFLAGS) -c -o lexer.o lang/lexer.yy.cc

clean:
	 rm -f *.o
	 rm $(GENERATED)
	 rm $(BIN)
