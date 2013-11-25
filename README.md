[grumble]() - A Regular Expression Engine
=======

```grumble``` is a regular expression engine written in C++.

[sample]("finitemachine.png")

##How It Works

Internally, ```grumble```, consists of two main phases:

- Interpret a regular expression string and create an identical node structure that represents the finite state automata machine.
- Pass a string through the machine and see if it matches.
