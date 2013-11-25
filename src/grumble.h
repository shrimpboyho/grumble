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

			// Contains all the pointers to other Nodes
			std::vector <Node*> connections;

			// Constructor
			Node(int, int);

			// Function that adds a connection
			Node* addConnection(Node*);

			// Function that tests if the given matches the capture
			bool test(int);
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

	Node* Node::addConnection(Node* node)
	{
		this -> connections[(int) node -> capture] = node;
		return node;
	}

	bool Node::test(int given)
	{
		if(given == this -> capture)
			return true;
		else
			return false;
	}

	class Machine
	{
		public:
			// Constructor
			Machine(std::string);

			// Machine current node
			Node* current;

			// Test against input
			bool feed(std::string);

	};

	Machine::Machine(std::string regex)
	{
		// Create the structure of the machine based on the regex
		// TODO: Auto generate this based on the regex
		Node* start = new Node('C');
		start -> start = true;
		Node* middle = new Node('A');
		Node* end = new Node('T');
		end -> end = true;
		start -> addConnection(middle);
		middle -> addConnection(end);

		// Set the start node in the class
		this -> current = start;
	}

	bool Machine::feed(std::string input)
	{
		// Set up the index character of the input string
		int i = 0;
		while(current -> test(input[i]))
		{
			// If the node we've hit is the last node
			// and the entire input is read we have a match
			if(current -> end == true && i == input.size() - 1)
				return true;

			// Get access to next node based on next character
			// in the input string
			i++;
			current = current -> connections[input[i]];
			
			// If that next node is equal to NULL, we don't have a match
			if(current == NULL)
				break;
		}
		return false;
	}

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
		Machine bot(this -> regex);
		return bot.feed(test);
	}


}

#endif /* GRUMBLE_H */

