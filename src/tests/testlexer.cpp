#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include "../grumble.h"

int main (int agrc, char* argv[])
{

    // For examining the ASTs generated from various regexps
    while(true)
    {
    	std::string input;
    	std::getline(std::cin,input);
    	GRUMBLE::Grumble croc(input);
        croc.matchEntireString("FOO");
    }
    	
	return 0;
}
