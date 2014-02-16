all:
	make tests
	make repl

tests:
	g++ -std=c++0x src/tests/getallmatches.cpp -o bin/getallmatches
	g++ -std=c++0x src/tests/matchentirestring.cpp -o bin/matchentirestring

repl:
	g++ -std=c++0x src/main.cpp -o bin/repl
