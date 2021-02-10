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
    static bool inputLoop();
    static bool parseLine(const std::string &line);
    static bool parseMove(const std::string &move);
    static void setBoard(const std::string &fen);
    static bool ArshaMove();
};

#endif // interface.h