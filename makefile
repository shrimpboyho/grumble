all:
	g++ -std=c++0x src/main.cpp -o bin/thing
	./bin/thing
compile:
	g++ -std=c++0x src/main.cpp -o bin/thing
