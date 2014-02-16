#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include "../grumble.h"

int main (int agrc, char* argv[])
{

    GRUMBLE::Grumble croc(".AY");

    // Test given input to regular expression
    while(true)
    {
    	std::string input;
    	std::getline(std::cin,input);
    	
    	// Test the getAllMatches() method
        GRUMBLE::MATCH_VECTOR matches = croc.getAllMatches(input);

        for(int i = 0; i < matches.size(); i++)
        {
    		std::cout << "At index " << matches[i].first << ":" << std::endl;
    		std::cout << matches[i].second << std::endl;
    	}
    }

    
    
	return 0;
}
