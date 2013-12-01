#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include "grumble.h"

int main (int agrc, char* argv[])
{

    GRUMBLE::Grumble croc(".AT");
    
    while(true)
    {
    	std::string input;
    	std::getline(std::cin,input);
    	std::cout << croc.match(input) << std::endl;
    }
    	
	return 0;
}
