vpath %.h% include
vpath tiny_% src

YACC=bison
LEX=flex

CXX=g++

tiny_parser: tiny_lexer.yy.cpp tiny_parser.tab.cc tiny_parser.tab.hh
	$(CXX) -o tiny_parser -g -O3 src/tiny_lexer.yy.cpp src/tiny_parser.tab.cc src/tiny.cpp

tiny_parser.tab.cc tiny_parser.tab.hh: tiny_parser.y
	$(YACC) -t -v -d -o src/tiny_parser.tab.cc src/tiny_parser.y
	mv src/*.h* src/*.output include

tiny_lexer.yy.cpp: tiny_lexer.l tiny_parser.tab.hh
	$(LEX) -o src/tiny_lexer.yy.cpp src/tiny_lexer.l

clean:
	-rm tiny_parser include/*.output