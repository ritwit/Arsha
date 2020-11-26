#ifndef __PERFTTEST__H__
#define __PERFTTEST__H__

bool perftTestSuite(const std::string &file_name);
long perftTestDepthOne(const std::string &fen);
long perftTestDepthOne(Board &bd);
long perftTest(const std::string &fen, int depth);
long numMovesAtDepth(Board &bd, int depth);


#endif