#ifndef GRUMBLE_H
#define GRUMBLE_H

#include <iostream>
#include <stack>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <stdlib.h>

namespace GRUMBLE
{

	class Grumble
	{

		private:
			std::string regex;
		public:
			Grumble(std::string);
			~Grumble();
			bool match(std::string);
	};

	Grumble::Grumble(std::string regex)
	{
		this -> regex = regex;
	}

	Grumble::~Grumble()
	{
    	// TODO
	}

	bool Grumble::match(std::string test)
	{
		return true;
	}

	class Node
	{
		public:
			bool start;
			bool end;
			std::vector <Node*> connections;
			Node();
	};

	Node::Node()
	{
		this -> start = false;
		this -> end = false;
		int i;
		for(i = 0; i < 128; i++)
		{
			Node* next;
			this -> connections.push_back(next);
		}
	}

}

#endif /* GRUMBLE_H */

