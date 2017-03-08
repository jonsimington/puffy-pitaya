#ifndef CPP_CLIENT_AI_TILE_HPP
#define CPP_CLIENT_AI_TILE_HPP

struct tile
{
  bool isOccupied;
  std::string owner; //Owner is stored as the color.
  int piece;
};

#endif //CPP_CLIENT_AI_TILE_HPP
