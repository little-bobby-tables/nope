CXX=g++
DEBUG=-g
CXXFLAGS=-g -std=c++11
BIN=nope

SRC=$(wildcard *.cpp)
LANG=parser.o lexer.o
OBJ=$(SRC:%.cpp=%.o), $(LANG)

all: $(OBJ)
	$(CXX) -o $(BIN) $^

%.o: %.c
	$(CXX) $@ -c $<

parser: lang/nope.yy
	bison -d -v lang/nope.yy
	$(CXX) $(CXXFLAGS) -c -o parser.o parser.tab.cc

lexer: lang/nope.l
	flex --outfile=lang/nope.yy.cc  $<
	$(CXX) $(CXXFLAGS) -c nope.yy.cc -o lexer.o

clean:
	 rm -f *.o
	 rm $(BIN)

