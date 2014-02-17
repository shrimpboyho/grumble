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

	/* Set up some typedefs */
	typedef std::pair<int,std::string> MATCH;
	typedef std::vector<std::pair<int,std::string>> MATCH_VECTOR;

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
	#define SPECIFICATION_GROUP 6
	#define REGEX_INNER 7

	// Simple modifier types
	#define ZERO_OR_MORE "*"
	#define ONE_OR_MORE "+"
	#define ZERO_OR_ONE "?"

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
			std::string tokenValue; // hold the token value
			
			std::string simpleQuantifier; // holds quantifiers such as +, *, or ?
			std::string exactQuantifier; // holds exact number quantifiers such as {23}
			std::pair <std::string, std::string> boundQuantifier; // holds quantifiers with bounds such as {1,4}
		
		REGEX_TOKEN(){
			simpleQuantifier = "";
			exactQuantifier = "";
			boundQuantifier.first = "";
			boundQuantifier.second = "";
		}
	};

	/* Parses the quantifiers */
	int findQuantifiers(std::string regex, REGEX_TOKEN &token, int i)
	{
		std::cout << "Recieved token: " << token.tokenValue << "\n";
		char current = regex[i];

		if(current == '*')
		{
			token.simpleQuantifier = ZERO_OR_MORE;
			return i;
		}
		if(current == '+')
		{
			token.simpleQuantifier = ONE_OR_MORE;
			return i;
		}
		if(current == '?')
		{
			token.simpleQuantifier = ZERO_OR_ONE;
			return i;
		}
		if(current != '{')
			return -1;

		// TODO: Add code for complex quantifiers

		return -1;

	}

	/* Parses the regular expression */
	std::vector <REGEX_TOKEN> parseRegex(std::string regex)
	{
		std::vector<REGEX_TOKEN> thing; // Represents the regular expression lexicon structure
		int i;
		for(i = 0; i < regex.size(); i++)
		{
			// Set up info about the character contexts
			char currentChar = regex[i];
			std::string currentCharString = "";
			std::string nextCharString = "";
			currentCharString += regex[i];
			if(i != regex.size() - 1)
				nextCharString += regex[i + 1];

			// Check for any escape sequences
			if(currentChar == '\\')
			{
				REGEX_TOKEN escapeToken;
				escapeToken.tokenType = ESCAPED_CHAR;
				escapeToken.tokenValue = nextCharString;
				int r = findQuantifiers(regex, escapeToken, i + 2);
				thing.push_back(escapeToken);
				if(r != -1)
					i = r;
				i++;
				continue;
			}
			// Check for any ANY_CHAR char
			if(currentChar == '.')
			{
				REGEX_TOKEN anyToken;
				anyToken.tokenType = ANY_CHAR;
				anyToken.tokenValue = ".";
				int r = findQuantifiers(regex, anyToken, i + 1);
				thing.push_back(anyToken);
				if(r != -1)
					i = r;
				continue;
			}
			// Check for a pure char
			else
			{
				REGEX_TOKEN pureToken;
				pureToken.tokenType = PURE_CHAR;
				pureToken.tokenValue = currentCharString;
				int r = findQuantifiers(regex, pureToken, i + 1);
				thing.push_back(pureToken);
				if(r != -1)
					i = r;
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

		// Set internal regex
		this -> regex = regex;

		// Obtain an AST of the regular expression
		std::vector<REGEX_TOKEN> tokens = parseRegex(regex);

		// Print the AST
		std::cout << "=========REGEX AST==========" << std::endl;
		for(int i = 0; i < tokens.size(); i++)
		{
			REGEX_TOKEN current = tokens[i];
			std::cout << "tokenType: " << current.tokenType << std::endl;
			std::cout << "tokenValue: " << current.tokenValue << std::endl;
			std::cout << "simpleQuantifier: " << current.simpleQuantifier << std::endl;
			std::cout << "exactQuantifier: " << current.exactQuantifier << std::endl;
			std::cout << "lower boundQuantifier: " << current.boundQuantifier.first << std::endl;
			std::cout << "upper boundQuantifier: " << current.boundQuantifier.second << std::endl;
			std::cout << "============================" << std::endl;
		}

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
				
				// if the escaped character is an escape character
				if(currentToken.tokenValue == "\\")
				{
					for(int z = 0; z < currentNodes.size(); z++)
					{
						Node* now = currentNodes[z];
						now -> addConnection(currentToken.tokenValue[0]);
					}
					
				}

				// if the escaped character is a digit escape char
				if(currentToken.tokenValue == "d")
				{
					for(int z = 0; z < currentNodes.size(); z++)
					{
						Node* now = currentNodes[z];
						for(int k = 48; k < 58; k++)
							now -> addConnection(k);
						
					}
					
				}

				// if the escaped character is a whitespace escape char
				if(currentToken.tokenValue == "s")
				{
					for(int z = 0; z < currentNodes.size(); z++)
					{
						Node* now = currentNodes[z];
						now -> addConnection(' ');
						now -> addConnection('\t');
						now -> addConnection('\n');
						now -> addConnection('\r');
						now -> addConnection('\f');
						now -> addConnection('\v');
						
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
			bool matchEntireString(std::string);
			MATCH_VECTOR getAllMatches(std::string test);
	};

	Grumble::Grumble(std::string regex)
	{
		this -> regex = regex;
	}

	Grumble::~Grumble()
	{
    	// TODO
	}

	// Checks if the entire string matches the regular expression and returns a boolean
	bool Grumble::matchEntireString(std::string test)
	{
		Machine* bot = new Machine(this -> regex);
		bool result = bot -> feed(test);

		// clean up the machine
		delete bot;
		
		return result;
	}

	// Returns a vector full of pairs.
	// Each pair contains index where the match was found, along with the match string
	MATCH_VECTOR Grumble::getAllMatches(std::string test)
	{
		// Vector holds the main data structure that this function will return
		MATCH_VECTOR matches;

		// get all possible substrings of the test string
		for(int i = 0; i < test.size(); i++)
		{
			std::string part = test.substr(i, test.size() + 1);
			for(int k = 0; k < part.size(); k++)
			{
				std::string party = part.substr(0, k + 1);

				// Test out the substring and if it works it is a match
				Grumble thing(this -> regex);
				if(thing.matchEntireString(party))
					matches.push_back(std::pair <int, std::string>(i, party));
			}
		}
		
		return matches;
	}


}

#endif /* GRUMBLE_H */
