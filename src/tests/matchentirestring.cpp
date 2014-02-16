#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include "../grumble.h"

int main (int agrc, char* argv[])
{

    GRUMBLE::Grumble croc("CAT\+");

    // Test given input to regular expression
    while(true)
    {
    	std::string input;
    	std::getline(std::cin,input);
    	std::cout << croc.matchEntireString(input) << std::endl;
    }
    	
	return 0;
}
