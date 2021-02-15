# Arsha
Rithwik's Chess Engine in C++.

Email: rtom (AT) andrew.cmu.edu

## Installation
You'll need:

1. A Unix(-like) machine.

2. A C++ compiler which supports C++11 standard.

3. Makefile build system.

Download the source code and run:

`make`

This will create a binary named arsha.x.

## How to Play
Launch the program first:

`./arsha.x`

To start a game enter:

`start`

Arsha plays white. She will make the first move.
You need to type in move commands. Moves are entered in an
algebraic chess notation. Squares are uniquely identified using
a system of coordinates. The rows/ranks are labelled from 1-8
and columns/files are labelled from A-H.

For example, if want to move
the black pawn from E7 Square to E5 Square, you need to enter:

`move e7e5`

Castle moves are represented by movement of the King. For example,
if you want to perform black kingside/short castle, enter:

`move e8g8`

A list of all possible moves can be found by:

`move list`

## Bugs
Please report bugs if you find any. Use the 'issues' tab on GitHub.

## Dedicated to

* Deep Thought/ Deep Blue team - Feng-hsiung Hsu, Murray Campbell, Thomas Anantharaman.

* Allen Newell and Herbert Simon.

* Claude Shannon.
