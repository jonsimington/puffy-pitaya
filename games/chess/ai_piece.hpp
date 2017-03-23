#ifndef CPP_CLIENT_AI_PIECE_HPP
#define CPP_CLIENT_AI_PIECE_HPP

//#include

namespace cpp_client
{

namespace chess
{

class ai_piece
{
public:
  std::string id;
  int rank;
  std::string file;
  std::string type;
  bool captured;
  
};
  
} //cpp_client
  
} //chess

#endif //CPP_CLIENT_AI_PIECE_HPP
