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

	/* Setup REGEX_MODIFIER used throughout the code */
	class REGEX_MODIFIER{
		public:
			int TYPE;  // The name of the modifier (one or more, any char, zero or more)
			int UPPER; // The upper bound of the modifier
			int LOWER; // The lower bound of the modifier
			int EXACT; // The exact bound of the modifier
	};

	// Token types
	#define ANY_CHAR 0
	#define PURE_CHAR 1
	#define ESCAPED_CHAR 2

	// Modifier types
	#define ZERO_OR_MORE 3
	#define ONE_OR_MORE 4
	#define ZERO_OR_ONE 5

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


	/* Function that does a depth search and creates a new handle */
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

	/* Regular expression token struct for the parser */
	class REGEX_TOKEN {

		public:
			int tokenType; // hold the modification on the token
			std::string tokenValue;       // hold the token value

	};

	/* Parses the regular expression */
	std::vector <REGEX_TOKEN> parseRegex(std::string regex)
	{
		std::vector<REGEX_TOKEN> thing;
		int i;
		for(i = 0; i < regex.size(); i++)
		{
			// Set up info about the character contexts
			char currentChar = regex[i];
			std::string currentCharString = "";
			std::string nextCharString = "";
			currentCharString += regex[i];
			nextCharString += regex[i];

			// Check for any escape sequences
			if(currentChar == '\\')
			{
				REGEX_TOKEN escapeToken;
				escapeToken.tokenType = ESCAPED_CHAR;
				escapeToken.tokenValue = nextCharString;
				thing.push_back(escapeToken);
				i++;
				continue;
			}
			// Check for any ANY_CHAR char
			if(currentChar == '.')
			{
				REGEX_TOKEN anyToken;
				anyToken.tokenType = ANY_CHAR;
				// TODO: DETERMINE MODIFIERS AND QUANTIFIERS
				anyToken.tokenValue = ".";
				thing.push_back(anyToken);
				continue;
			}
			// Check for a pure char
			else
			{
				REGEX_TOKEN pureToken;
				pureToken.tokenType = PURE_CHAR;
				// TODO: DETERMINE MODIFIERS AND QUANTIFIERS
				pureToken.tokenValue = currentCharString;
				thing.push_back(pureToken);
				continue;
			}
		}
		return thing;
	}

	/* Object that respresents the machine that will be constructed for each regex */
	class Machine
	{
		public:
			// Constructor
			Machine(std::string);

			// regular expression the machine represents
			std::string regex;

			// Machine current node
			Node* current;

			// Test against input
			bool feed(std::string);

	};

	/* Constructor sets up machine based on regex provided */
	Machine::Machine(std::string regex)
	{
		
		this -> regex = regex;
		std::vector<REGEX_TOKEN> tokens = parseRegex(regex);

		// Create the intial start node for the machine
		std::vector <Node*> currentNodes;
		Node* newone = new Node();

		// Set as intial node
		this -> current = newone;

		// Shove it in the handle
		currentNodes.push_back(newone);
		
		// begin appending nodes based on regex
		int i;
		for(i = 0; i < tokens.size(); i++)
		{
			REGEX_TOKEN currentToken = tokens[i];
			
			// dot all match
			if(currentToken.tokenType == ANY_CHAR)
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

			// escape character match
			else if (currentToken.tokenType == ESCAPED_CHAR)
			{
				if(currentToken.tokenValue == "\\")
				{
					for(int z = 0; z < currentNodes.size(); z++)
					{
						Node* now = currentNodes[z];
						now -> addConnection(currentToken.tokenValue[0]);
					}
					
				}

				/* TODO: IMPLEMENT MORE ESCAPE CHARACTERS */

				// get handle on new nodes
				currentNodes = getNewHandle(currentNodes);
			}

			// literal character match
			else if (currentToken.tokenType == PURE_CHAR)
			{
				for(int z = 0; z < currentNodes.size(); z++)
				{
					Node* now = currentNodes[z];
					now -> addConnection(currentToken.tokenValue[0]);
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
		int i;

		for(i = 0; i < input.size(); i++)
		{
			char currentChar = input[i];
			std::cout << "On input string char: " << currentChar << std::endl;
			current = current -> getConnection(currentChar);
			if(current == NULL)
				return false;
			std::cout << "On node with capture of: " << (char) current -> capture << std::endl;
			if(current -> end == true && i == input.size() - 1)
				return true;
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
