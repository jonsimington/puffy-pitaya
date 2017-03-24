#ifndef GAMES_CHESS_AI_HPP
#define GAMES_CHESS_AI_HPP

#include "impl/chess.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "player.hpp"

#include "../../joueur/src/base_ai.hpp"
#include "../../joueur/src/attr_wrapper.hpp"

#include "ai_move.hpp"
#include "ai_piece.hpp"
#include "ai_tile.hpp"

// You can add additional #includes here

namespace cpp_client
{

namespace chess
{

/// <summary>
/// This is the header file for building your Chess AI
/// </summary>
class AI : public Base_ai
{
public:
    /// <summary>
    /// This is a reference to the Game object itself, it contains all the information about the current game
    /// </summary>
    Game game;

    /// <summary>
    /// This is a pointer to your AI's player. This AI class is not a player, but it should command this Player.
    /// </summary>
    Player player;

    // You can add additional class variables here.
  
  //Maybe create board here.
  
  //Variable Declaration:
  int depthLimit;
  std::vector<rep_move> previous_moves;

    /// <summary>
    /// This returns your AI's name to the game server.
    /// Replace the string name.
    /// </summary>
    /// <returns>The name of your AI.</returns>
    virtual std::string get_name() const override;

    /// <summary>
    /// This is automatically called when the game first starts, once the game objects are created
    /// </summary>
    virtual void start() override;

    /// <summary>
    /// This is automatically called when the game ends.
    /// </summary>
    /// <param name="won">true if you won, false otherwise</param>
    /// <param name="reason">An explanation for why you either won or lost</param>
    virtual void ended(bool won, const std::string& reason) override;

    /// <summary>
    /// This is automatically called the game (or anything in it) updates
    /// </summary>
    virtual void game_updated() override;

    /// <summary>
    /// This is called every time it is this AI.player's turn.
    /// </summary>
    /// <returns>Represents if you want to end your turn. True means end your turn, False means to keep your turn going and re-call this function.</returns>
    bool run_turn();

    /// <summary>
    ///  Prints the current board using pretty ASCII art
    /// </summary>
    /// <remarks>
    /// Note: you can delete this function if you wish
    /// </remarks>
    void print_current_board();

    // You can add additional methods here.
  
  //Returns the game board.
  std::vector< std::vector<tile> >  getBoard();
  
  //Returns a vector of all possible moves.
  std::vector<ai_move> action(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, std::vector< std::vector<tile> >  board,
                              std::string myColor, std::string oppColor, int rankDir);
  
  //Returns vector of all possible moves the given piece can make.
  std::vector<ai_move> get_Move(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, ai_piece toMove, int numPiece,
                                std::vector< std::vector<tile> >  board, std::string myColor, std::string oppColor, int rankDir);
  
  //Returns true if the given move on the given board does not put my king in check.
  bool valid_move(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, chess::ai_move nextMove, std::vector< std::vector<tile> > board,
                  std::string myColor, std::string oppColor, int rankDir);
  
  //Pawn movement function.
  std::vector<ai_move> pawn_movement(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, ai_piece toMove, int numPiece, int numFile, int rank,
                                     std::vector< std::vector<tile> >  board, std::string myColor, std::string oppColor, int rankDir);
  
  //Bishop movement function.
  std::vector<ai_move> bishop_movement(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, ai_piece toMove, int numPiece, int numFile, int rank,
                                       std::vector< std::vector<tile> >  board, std::string myColor, std::string oppColor, int rankDir);
  
  //Rook movement function.
  std::vector<ai_move> rook_movement(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, ai_piece toMove, int numPiece, int numFile, int rank,
                                     std::vector< std::vector<tile> >  board, std::string myColor, std::string oppColor, int rankDir);
  
  //Knight movement function.
  std::vector<ai_move> knight_movement(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, ai_piece toMove, int numPiece, int numFile, int rank,
                                       std::vector< std::vector<tile> >  board, std::string myColor, std::string oppColor, int rankDir);
  
  //King movement function.
  std::vector<ai_move> king_movement(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, ai_piece toMove, int numPiece, int numFile, int rank,
                                     std::vector< std::vector<tile> >  board, std::string myColor, std::string oppColor, int rankDir);
  
  //MiniMax function.
  ai_move miniMax(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, int limit, std::vector< std::vector<tile> >  board);
  
  //Min value function for minimax.
  int minVal(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, int limit, std::vector< std::vector<tile> > board, std::vector<int> myCaptured, std::vector<int> oppCaptured);
  
  //Max value function for minimax.
  int maxVal(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, int limit, std::vector< std::vector<tile> > board, std::vector<int> myCaptured, std::vector<int> oppCaptured);
  
  //Returns the value of the pieces based on the given board state.
  int getValue(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, std::vector< std::vector<tile> > board);
  
  //Returns true if the given player is in check.
  bool in_check(std::string myColor, std::string oppColor, std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, std::vector< std::vector<tile> > board, int rankDir);
  
    // ####################
    // Don't edit these!
    // ####################
    /// \cond FALSE
    virtual std::string invoke_by_name(const std::string& name,
                                       const std::unordered_map<std::string, Any>& args) override;
    virtual void set_game(Base_game* ptr) override;
    virtual void set_player(std::shared_ptr<Base_object> obj) override;
    virtual void print_win_loss_info() override;
    /// \endcond
    // ####################
    // Don't edit these!
    // ####################

};

} // CHESS

} // cpp_client

#endif // GAMES_CHESS_AI_HPP
