#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include "../grumble.h"

int main (int agrc, char* argv[])
{

    GRUMBLE::Grumble croc("CAT\+");

    // Test the getAllMatches() method
    GRUMBLE::MATCH_VECTOR matches = croc.getAllMatches("PERMUTATIONS");
    
	return 0;
}
