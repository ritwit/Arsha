#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>

#include "interface.h"
#include "debug.h"
#include "movegen.h"
#include "search.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

const std::string Interface::fen_str = "fen";
const std::string Interface::move_str = "move";
const std::string Interface::startfen_str = STARTFEN;
Board Interface::bd_state;


bool Interface::inputLoop()
{
    cout << "Arsha >>> ";
    for(string line; std::getline(cin, line); )
    {
        string command = line;
        int status = SUCCESS;

        if(line == "exit")
            return status;

        else if(line == "start")
        {
            cout << "Starting new game..." << endl;
            command = fen_str + " " + startfen_str;
        }

        status = parseLine(command);

        if(status == SUCCESS)
        {
            bd_state.printBoard();
            print_separator();
            cout << "Arsha is thinking..." << endl;
            ArshaMove();
            print_separator();
            bd_state.printBoard();
        }

        print_double_separator();
        cout << "Arsha >>> ";
    }

    return false;
}

bool Interface::ArshaMove()
{
    Move best_move;
    int score;
    const int depth = 5;
    Negamax search;
    std::tie(best_move, score) = search.findBestMove(bd_state, depth);
    best_move.applyMove(bd_state);
    cout << "Arsha moves: " << best_move.getString() << endl;
    return true;
}

int Interface::parseLine(const string &line)
{
    //cout << "Parsing line: " << line << endl;
    int state = SUCCESS;

    auto space = line.find(' ');
    if (space == string::npos)
    {
        cout << "Unable to parse command; Please retry." << endl;
        return false;
    }

    string command = line.substr(0, space);
    if(command == move_str)
    {
        state = parseMove(line.substr(space + 1, string::npos));

    }
    else if(command == fen_str)
    {
        setBoard(line.substr(space, string::npos));
    }
    else
    {
        cout << "Unable to parse instruction: " << command << endl;
        state = FAIL;
    }
    return state;
}

void Interface::setBoard(const string &fen)
{
    bd_state.setBoardFromFEN(fen);
}

int Interface::parseMove(const string &move)
{
    string input_move = move;
    // make upercase
    std::transform(input_move.begin(),
                   input_move.end(),
                   input_move.begin(),
                   ::toupper);
    // remove spaces
    input_move.erase(std::remove(input_move.begin(), input_move.end(), ' '),
                     input_move.end());

    //cout << "Parsing move" << input_move << endl;
    MoveGenerator mvgen;
    mvgen.generateMoves(bd_state);

    // List all moves
    if(input_move == "LIST")
    {
        for(Move &mv: mvgen.MoveList)
            cout << mv.getString() << endl;
        return SKIP;
    }

    for(Move &mv: mvgen.MoveList)
    {
        //cout << mv.getString() << endl;
        if(input_move == mv.getString())
        {
            mv.applyMove(bd_state);
            return SUCCESS;
        }
    }
    cout << "Unable to make move: "<< input_move << endl;
    return FAIL;
}
