[grumble]() - A Regular Expression Engine
=======
[![Build Status](https://drone.io/github.com/shrimpboyho/grumble/status.png)](https://drone.io/github.com/shrimpboyho/grumble/latest)
```grumble``` is a regular expression engine written in C++.
The following is a representation of the underlying data structure the engine is dependent on. Each node represents a state. Each state has several connections that connect to other nodes. The following shows a machine structure that will accept the string ```"CATZ"```. Although not displayed, each node has 128 connection (for the 128 ASCII characters), many of which are ```NULL```.

<img src = "finitemachine.png">
##How It Works

Internally, ```grumble```, consists of two main phases:

- Interpret a regular expression string and create an identical node structure that represents the finite state automata machine.
- Pass a string through the machine and see if it matches.

```grumble``` currently can support simple regular expressions, such as
```CAT.``` which matches a string starting with ```CAT``` and followed by any one random character.

##Features

There are several features which are not implemented yet. In order to implement more advanced features such as groupings, quantifiers, and more, a better tokenizer is needed for the regular expression itself. This extra lexing and parsing phase is necessary in order to construct more complex finite state machines.