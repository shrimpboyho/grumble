all:
	make tests
	make repl

tests:
	g++ -std=c++0x src/tests/perm.cpp -o bin/perm
	g++ -std=c++0x src/tests/entire.cpp -o bin/entire

repl:
	g++ -std=c++0x src/main.cpp -o bin/repl
