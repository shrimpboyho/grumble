all:
	make test
	make repl

test:
	g++ -std=c++0x src/tests/perm.cpp -o bin/perm
	g++ -std=c++0x src/tests/entire.cpp -o bin/entire
	cd bin
	./perm
	./entire

repl:
	g++ -std=c++0x src/main.cpp -o bin/repl
