#include <iostream>
#include <string>

#include "interface.h"
#include "debug.h"
#include "movegen.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;


void Interface::inputLoop()
{
    for(string line; std::getline(cin, line); )
    {
        if(line == "exit")
            return;
        else if(line == "start")
            parseLine(fen_str + startfen_str);
    }
}

void Interface::parseLine(const string &line)
{
    auto space = line.find(' ');
    if (space == string::npos)
        cout << "Unable to parse command; Please retry." << endl;

    string command = line.substr(0, space);
    if(command == move_str)
        parseMove(line.substr(space, string::npos));
    if(command == fen_str)
        setBoard(line.substr(space, string::npos));
}

void Interface::setBoard(const string &fen)
{
    bd_state.setBoardFromFEN(fen);
    bd_state.printBoard();
}

void Interface::parseMove(const string &move)
{
    MoveGenerator mvgen;
    mvgen.generateMoves(bd_state);
    for (Move &mv: mvgen.MoveList)
    {

    }
}
