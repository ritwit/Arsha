#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "board.h"

class Interface
{
private:
    static Board bd_state;
    static const std::string fen_str;
    static const std::string move_str;
    static const std::string startfen_str;

public:
    static void inputLoop();
    static void parseLine(const std::string &line);
    static void parseMove(const std::string &move);
    static void setBoard(const std::string &fen);
};

const std::string Interface::fen_str = "fen";
const std::string Interface::move_str = "move";
const std::string Interface::startfen_str = STARTFEN;
Board Interface::bd_state;


#endif // interface.h