#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include "grumble.h"

int main (int agrc, char* argv[])
{

    GRUMBLE::Grumble croc("s,d+");
    std::cout << croc.match("asd") << std::endl;

	return 0;
}
