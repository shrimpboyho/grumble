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
			// Represents whether the node is the start or end
			bool start;
			bool end;

			// Sets a recursion limit
			int limit;

			// The character the Node is set to capture
			int capture;

			// The character the Node will test against.
			int given;

			// Contains all the pointers to other Nodes
			std::vector <Node*> connections;

			// Constructor
			Node(int, int);

			// Function that adds a connection
			Node* addConnection(int, Node*);
	};

	Node::Node(int capture, int limit = 0)
	{
		this -> start = false;
		this -> end = false;
		this -> limit = limit;
		this -> capture = capture;
		
		// Load up all the connections as NULL
		int i;
		for(i = 0; i < 128; i++)
		{
			this -> connections.push_back(NULL);
		}
	}

	Node* Node::addConnection(int index, Node* node)
	{
		this -> connections[index] = node;
		return node;
	}

}

#endif /* GRUMBLE_H */

