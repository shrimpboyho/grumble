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

	/* Represents each state in the regex machine */
	class Node
	{
		public:
			// Represents whether the node is the end state
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
			Node* addConnection(int);

			// Function taht returns a connection
			Node* getConnection(int);

			// Function that tests if the given matches the capture
			bool test(int);
	};

	/* Constructor for the node object */
	Node::Node(int capture = 0, int limit = 0)
	{
		this -> end = false;
		this -> limit = limit;
		if(capture != 0)
			this -> capture = capture;
		
		// Load up all the connections as NULL
		int i;
		for(i = 0; i < 128; i++)
		{
			this -> connections.push_back(NULL);
		}
	}

	Node* Node::addConnection(int capture)
	{
		Node* newone = new Node(capture);
		this -> connections[capture] = newone;
		return newone;
	}

	/* Returns a connected node */
	Node* Node::getConnection(int i)
	{
		return this -> connections[i];
	}

	bool Node::test(int given)
	{
		if(given == this -> capture)
			return true;
		else
			return false;
	}


	/* Does a depth search and creates a new handle */
	std::vector <Node*> getNewHandle(std::vector <Node*> oldHandle)
	{
		std::vector <Node*> newHandle;
		for(int i = 0; i < oldHandle.size(); i++)
		{
			std::vector <Node*> connections = oldHandle[i] -> connections;
			for(int k = 0; k < connections.size(); k++)
			{
				if(connections[k] != NULL)
				{
					newHandle.push_back(connections[k]);
				}
			}
		}
		return newHandle;
	}

	/* Object that respresents the machine that will be constructed for each regex */
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

	/* Constructor sets up machine based on regex provided */
	Machine::Machine(std::string regex)
	{
	
		// Create the intial start node
		std::vector <Node*> currentNodes;
		Node* newone = new Node();

		// Set as intial node
		this -> current = newone;

		// Shove it in the handle
		currentNodes.push_back(newone);
		
		// begin appending nodes based on regex
		int i;
		for(i = 0; i < regex.size(); i++)
		{
			char currentRegexChar = regex[i];
			
			// dot all match
			if(currentRegexChar == '.')
			{
				// create nodes to match to all characters
				for(int z = 0; z < currentNodes.size(); z++)
				{
					Node* now = currentNodes[z];
					for(int k = 0; k < 128; k++)
					{
						now -> addConnection(k);
					}
				}

				// get handle on new nodes
				currentNodes = getNewHandle(currentNodes);

			}

			// literal character match
			else
			{
				for(int z = 0; z < currentNodes.size(); z++)
				{
					Node* now = currentNodes[z];
					now -> addConnection(currentRegexChar);
				}

				// get handle on new nodes
				currentNodes = getNewHandle(currentNodes);
			}
			
		}

		// Make last nodes all stop states

		for(int z = 0; z < currentNodes.size(); z++)
		{
			Node* now = currentNodes[z];
			now -> end = true;
		}

		std::cout << "Finished building the machine." << std::endl;

	/*
		Node* start = new Node('C');
		start -> start = true;
		Node* middle = new Node('A');
		Node* end = new Node('T');
		end -> end = true;
		start -> addConnection(middle);
		middle -> addConnection(end);  */
	}

	/* Tests a string of input to see if it passes the machine */
	bool Machine::feed(std::string input)
	{
		// Set up the index character of the input string
		int i = 0;

		while(true)
		{
			char currentChar = input[i];
			std::cout << "On input string char: " << currentChar << std::endl;
			current = current -> getConnection(currentChar);
			if(current == NULL)
				return false;
			std::cout << "On node with capture of: " << (char) current -> capture << std::endl;
			if(current -> end == true && i == input.size() - 1)
				return true;
			i++;
		}
	}

	/* Main library class accessible by the user*/
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
		Machine* bot = new Machine(this -> regex);
		bool result = bot -> feed(test);

		// clean up the machine
		delete bot;
		
		return result;
	}


}

#endif /* GRUMBLE_H */

