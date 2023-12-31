#ifndef CPP_CLIENT_AI_MOVE_HPP
#define CPP_CLIENT_AI_MOVE_HPP

#include "piece.hpp"

namespace cpp_client
{

namespace chess
{

class ai_move
{
public:
  int piece_moved;
  std::string destination_file;
  int destination_rank;
  
};

class rep_move
{
public:
  int piece_moved;
  std::string origin_file;
  int origin_rank;
  std::string destination_file;
  int destination_rank;
};

} //cpp_client

} //chess

#endif //CPP_CLIENT_AI_MOVE_HPP
