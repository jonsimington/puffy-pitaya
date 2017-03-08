// AI
// This is where you build your AI

#include "ai.hpp"

// You can add #includes here for your AI.

namespace cpp_client
{

namespace chess
{

/// <summary>
/// This returns your AI's name to the game server.
/// Replace the string name.
/// </summary>
/// <returns>The name of your AI.</returns>
std::string AI::get_name() const
{
    // REPLACE WITH YOUR TEAM NAME!
    return "Ian Chess";
}

/// <summary>
/// This is automatically called when the game first starts, once the game objects are created
/// </summary>
void AI::start()
{
    // This is a good place to initialize any variables
    srand(time(NULL));
}

/// <summary>
/// This is automatically called the game (or anything in it) updates
/// </summary>
void AI::game_updated()
{
    // If a function you call triggers an update this will be called before it returns.
}

/// <summary>
/// This is automatically called when the game ends.
/// </summary>
/// <param name="won">true if you won, false otherwise</param>
/// <param name="reason">An explanation for why you either won or lost</param>
void AI::ended(bool won, const std::string& reason)
{
    // You can do any cleanup of your AI here.  The program ends when this function returns.
}

/// <summary>
/// This is called every time it is this AI.player's turn.
/// </summary>
/// <returns>Represents if you want to end your turn. True means end your turn, False means to keep your turn going and re-call this function.</returns>
bool AI::run_turn()
{
    // Here is where you'll want to code your AI.

  /*
    // We've provided sample code that:
    //    1) prints the board to the console
    //    2) prints the opponent's last move to the console
    //    3) prints how much time remaining this AI has to calculate moves
    //    4) makes a random (and probably invalid) move.

    // 1) print the board to the console
    print_current_board();

    // 2) print the opponent's last move to the console
    if(game->moves.size() > 0)
    {
        std::cout << "Opponent's Last Move: '" << game->moves[game->moves.size() - 1]->san << "'" << std::endl;
    }

    // 3) print how much time remaining this AI has to calculate moves
    std::cout << "Time Remaining: " << player->time_remaining << " ns" << std::endl;

    // 4) make a random (and probably invalid) move.
    chess::Piece random_piece = player->pieces[rand() % player->pieces.size()];
    std::string random_file(1, 'a' + rand() % 8);
    int random_rank = (rand() % 8) + 1;
    random_piece->move(random_file, random_rank);
  
  */
  std::vector< std::vector<tile> >  gameBoard;
  std::vector<ai_move> allMoves;
  int randNum;
  
  
  gameBoard = getBoard();
  
  allMoves = action(gameBoard);
  
  randNum = (rand() % allMoves.size());
  
  player -> pieces[allMoves[randNum].piece_moved] -> move(allMoves[randNum].destination_file, allMoves[randNum].destination_rank, "Queen");
  
  
  
  
  
  
  
  
  

    return true; // to signify we are done with our turn.
}

/// <summary>
///  Prints the current board using pretty ASCII art
/// </summary>
/// <remarks>
/// Note: you can delete this function if you wish
/// </remarks>
void AI::print_current_board()
{
    for(int rank = 9; rank >= -1; rank--)
    {
        std::string str = "";
        if(rank == 9 || rank == 0) // then the top or bottom of the board
        {
            str = "   +------------------------+";
        }
        else if(rank == -1) // then show the ranks
        {
            str = "     a  b  c  d  e  f  g  h";
        }
        else // board
        {
            str += " ";
            str += std::to_string(rank);
            str += " |";
            // fill in all the files with pieces at the current rank
            for(int file_offset = 0; file_offset < 8; file_offset++)
            {
                std::string file(1, 'a' + file_offset); // start at a, with with file offset increasing the char;
                chess::Piece current_piece = nullptr;
                for(const auto& piece : game->pieces)
                {
                    if(piece->file == file && piece->rank == rank) // then we found the piece at (file, rank)
                    {
                        current_piece = piece;
                        break;
                    }
                }

                char code = '.'; // default "no piece";
                if(current_piece != nullptr)
                {
                    code = current_piece->type[0];

                    if(current_piece->type == "Knight") // 'K' is for "King", we use 'N' for "Knights"
                    {
                        code = 'N';
                    }

                    if(current_piece->owner->id == "1") // the second player (black) is lower case. Otherwise it's upppercase already
                    {
                        code = tolower(code);
                    }
                }

                str += " ";
                str += code;
                str += " ";
            }

            str += "|";
        }

        std::cout << str << std::endl;
    }
}

// You can add additional methods here for your AI to call

//Returns the current state of the chess board.
std::vector< std::vector<tile> >  AI::getBoard()
{
  std::vector< std::vector<tile> > tempBoard;
  std::vector<tile> tempRank;
  tile tempTile;
  int numFile;
  
  tempTile.isOccupied = false;
  
  //Create an empty Rank to push back to the board.
  for (int i=0; i <= 8; i++)
  {
    tempRank.push_back(tempTile);
  }
  
  //Push empty ranks to create the board.
  for (int i=0; i <= 8; i++)
  {
    tempBoard.push_back(tempRank);
  }
  
  //Place my pieces on the board.
  for (int i=0; i < player -> pieces.size(); i++)
  {
    //Get a numerical value for the file so it can be located in the 2D vector of the Board.
    if (player -> pieces[i] -> file == "a")
    {
      numFile = 1;
    } else if (player -> pieces[i] -> file == "b")
    {
      numFile = 2;
    } else if (player -> pieces[i] -> file == "c")
    {
      numFile = 3;
    } else if (player -> pieces[i] -> file == "d")
    {
      numFile = 4;
    } else if (player -> pieces[i] -> file == "e")
    {
      numFile = 5;
    } else if (player -> pieces[i] -> file == "f")
    {
      numFile = 6;
    } else if (player -> pieces[i] -> file == "g")
    {
      numFile = 7;
    } else if (player -> pieces[i] -> file == "h")
    {
      numFile = 8;
    }
    
    //Set board values where the piece is.
    tempBoard[player -> pieces[i] -> rank][numFile].isOccupied = true;
    tempBoard[player -> pieces[i] -> rank][numFile].owner = player -> color;
    tempBoard[player -> pieces[i] -> rank][numFile].piece = i;
    
  } //My pieces.
  
  //Place opponents pieces on the board.
  for (int i=0; i< player -> opponent -> pieces.size(); i++)
  {
    //Get a numerical value for the file so it can be located in the 2D vector of the Board.
    if (player -> opponent -> pieces[i] -> file == "a")
    {
      numFile = 1;
    } else if (player -> opponent -> pieces[i] -> file == "b")
    {
      numFile = 2;
    } else if (player -> opponent -> pieces[i] -> file == "c")
    {
      numFile = 3;
    } else if (player -> opponent -> pieces[i] -> file == "d")
    {
      numFile = 4;
    } else if (player -> opponent -> pieces[i] -> file == "e")
    {
      numFile = 5;
    } else if (player -> opponent -> pieces[i] -> file == "f")
    {
      numFile = 6;
    } else if (player -> opponent -> pieces[i] -> file == "g")
    {
      numFile = 7;
    } else if (player -> opponent -> pieces[i] -> file == "h")
    {
      numFile = 8;
    }
    
    //Set board values where the piece is.
    tempBoard[player -> opponent -> pieces[i] -> rank][numFile].isOccupied = true;
    tempBoard[player -> opponent -> pieces[i] -> rank][numFile].owner = player -> opponent -> color;
    tempBoard[player -> opponent -> pieces[i] -> rank][numFile].piece = i;
    
  } //Opponents pieces.
  
  return tempBoard;
} //Get Board Function.

//Returns a vector of all possible moves.
std::vector<ai_move> AI::action(std::vector< std::vector<tile> >  board)
{
  std::vector<ai_move> allMoves;
  std::vector<ai_move> tempMoves;
  ai_move temp;
  
  allMoves.clear();
  
  //For every piece owned by this AI, get every possible move by that piece and concatenate it with the moves from all
  //the other pieces to create a vector with all possible moves.
  for (int i=0; i < player -> pieces.size(); i++) {
    tempMoves.clear();
    tempMoves = get_Move((Piece &) player -> pieces[i], i, board);
    
    allMoves.insert(allMoves.end(), tempMoves.begin(), tempMoves.end());
  }
  
  return allMoves;
} //Action Function.
    
//Returns vector of all possible moves the given piece can make.
std::vector<ai_move> AI::get_Move(chess::Piece& toMove, int numPiece, std::vector< std::vector<tile> >  board)
{
  std::vector<ai_move> moves_Got;
  ai_move temp;
  int numFile;
  int rank;
  
  moves_Got.clear();
  
  //Get numerical value for the file of the piece.
  if (toMove->file == "a") {
    numFile = 1;
  } else if (toMove->file == "b") {
    numFile = 2;
  } else if (toMove->file == "c") {
    numFile = 3;
  } else if (toMove->file == "d") {
    numFile = 4;
  } else if (toMove->file == "e") {
    numFile = 5;
  } else if (toMove->file == "f") {
    numFile = 6;
  } else if (toMove->file == "g") {
    numFile = 7;
  } else if (toMove->file == "h") {
    numFile = 8;
  }
  
  //Get rank of piece to move.
  rank = toMove -> rank;
  
  ////////////////////////////////////
  //If Pawn, make appropriate moves.//
  ////////////////////////////////////
  
  if (toMove->type == "Pawn") {
    
    //If forward move is legal.
    if (board[toMove->rank + player->rank_direction][numFile].isOccupied == false) {
      temp.destination_file = toMove->file;
      temp.destination_rank = (toMove->rank + player->rank_direction);
      //temp.piece_moved = &toMove;
      temp.piece_moved = numPiece;
      
      if (valid_move(temp, board))
      {
        moves_Got.push_back(temp);
      }
    
    } //Forward move.
  
    //If capturing right diagonal is legal.
    if (numFile + 1 <= 8) {
      if (board[toMove->rank + player->rank_direction][numFile + 1].isOccupied == true) {
        if (board[toMove->rank + player->rank_direction][numFile + 1].owner != player->color) {
          temp.destination_file = ('a' + numFile);
          temp.destination_rank = (toMove->rank + player->rank_direction);
          temp.piece_moved = numPiece;
  
          if (valid_move(temp, board))
          {
            moves_Got.push_back(temp);
          }
        }
      
      }
    
    } //Right diagonal.
  
    //If capturing left diagonal is legal.
    if (numFile - 1 > 0) {
      if (board[toMove->rank + player->rank_direction][numFile - 1].isOccupied == true) {
        if (board[toMove->rank + player->rank_direction][numFile - 1].owner != player->color) {
          temp.destination_file = ('a' + numFile - 2);
          temp.destination_rank = (toMove->rank + player->rank_direction);
          temp.piece_moved = numPiece;
  
          if (valid_move(temp, board))
          {
            moves_Got.push_back(temp);
          }
        }
      
      }
    
    } //Left diagonal.
  
  
  
  
  
  
  } //Pawn movement.
  
  ///////////////////////////////////
  //If Rook, make appropiate moves.//
  ///////////////////////////////////
  
  if (toMove->type == "Rook") {
    bool done;
    done = false;
    
    //Positive rank movement.
    for (int i = rank + 1; i <= 8; i++) {
      if (board[i][numFile].isOccupied == true && done == false) {
        if (board[i][numFile].owner == player->opponent->color) {
          temp.piece_moved = numPiece;
          temp.destination_rank = i;
          temp.destination_file = toMove -> file;
          
          if (valid_move(temp, board)) {
            moves_Got.push_back(temp);
          }
          
        }
        
        done = true;
      } else if (done == false) {
        temp.piece_moved = numPiece;
        temp.destination_rank = i;
        temp.destination_file = toMove -> file;
  
        if (valid_move(temp, board)) {
          moves_Got.push_back(temp);
        }
  
      }
      
    } //Positive Rank Movement.
    
    done = false;
    
    //Negative rank movement.
    for (int i = rank - 1; i > 0; i--) {
      if (board[i][numFile].isOccupied == true && done == false) {
        if (board[i][numFile].owner == player->opponent->color) {
          temp.piece_moved = numPiece;
          temp.destination_rank = i;
          temp.destination_file = toMove -> file;
      
          if (valid_move(temp, board)) {
            moves_Got.push_back(temp);
          }
      
        }
    
        done = true;
      } else if (done == false) {
        temp.piece_moved = numPiece;
        temp.destination_rank = i;
        temp.destination_file = toMove -> file;
  
        if (valid_move(temp, board)) {
          moves_Got.push_back(temp);
        }
  
      }
      
    
      
    } // Negative Rank Movement.
    
    done = false;
    
    //Positive file movement.
    for (int i = numFile + 1; i <= 8; i++) {
      if (board[rank][i].isOccupied == true && done == false) {
        if (board[rank][i].owner == player->opponent->color) {
          temp.piece_moved = numPiece;
          temp.destination_rank = toMove -> rank;
          temp.destination_file = ('a' + i - 1);
      
          if (valid_move(temp, board)) {
            moves_Got.push_back(temp);
          }
      
        }
    
        done = true;
      } else if (done == false) {
        temp.piece_moved = numPiece;
        temp.destination_rank = toMove -> rank;
        temp.destination_file = ('a' + i - 1);
  
        if (valid_move(temp, board)) {
          moves_Got.push_back(temp);
        }
  
      }
      
      
    } //Positive File Movement.
    
    done = false;
    
    //Negative file movement.
    for (int i = numFile - 1; i > 0; i--) {
      if (board[rank][i].isOccupied == true && done == false) {
        if (board[rank][i].owner == player->opponent->color) {
          temp.piece_moved = numPiece;
          temp.destination_rank = toMove -> rank;
          temp.destination_file = ('a' + i - 1);
      
          if (valid_move(temp, board)) {
            moves_Got.push_back(temp);
          }
      
        }
    
        done = true;
      } else if (done == false) {
        temp.piece_moved = numPiece;
        temp.destination_rank = toMove -> rank;
        temp.destination_file = ('a' + i - 1);
  
        if (valid_move(temp, board)) {
          moves_Got.push_back(temp);
        }
  
      }
      
      
    } //Negative File Movement.
    
    
    
    
    
    
  } //Rook Movement.
  
  /////////////////////////////////////
  //If Bishop, make appropiate moves.//
  /////////////////////////////////////
  
  if (toMove->type == "Bishop") {
    
    
    
    
    
  } //Bishop Movement.
  
  /////////////////////////////////////
  //If Knight, make appropiate moves.//
  /////////////////////////////////////
  
  if (toMove->type == "Knight") {
    
    
    
    
    
  } //Knight Movement.
  
  ////////////////////////////////////
  //If Queen, make appropiate moves.//
  ////////////////////////////////////
  
  if (toMove->type == "Queen") {
    
    
    
    
    
  } //Queen Movement.
  
  
  ///////////////////////////////////
  //If King, make appropiate moves.//
  ///////////////////////////////////
  
  if (toMove->type == "King") {
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
  } //King Movement.
  
  return moves_Got;
} //Get Move Function.
    
bool AI::valid_move(chess::ai_move nextMove, std::vector< std::vector<tile> > board)
{
  //Variable Declaration:
  int king;
  int rank;
  int numfile;
  int diagMoves;
  bool valid;
  bool done;
  
  valid = true;
  
  /////////////////////////////////////
  //Apply potential move to the board//
  /////////////////////////////////////
  
  //Get file number for where the piece originates.
  if (player -> pieces[nextMove.piece_moved] -> file == "a") {
    numfile = 1;
  } else if (player -> pieces[nextMove.piece_moved] -> file == "b") {
    numfile = 2;
  } else if (player -> pieces[nextMove.piece_moved] -> file == "c") {
    numfile = 3;
  } else if (player -> pieces[nextMove.piece_moved] -> file == "d") {
    numfile = 4;
  } else if (player -> pieces[nextMove.piece_moved] -> file == "e") {
    numfile = 5;
  } else if (player -> pieces[nextMove.piece_moved] -> file == "f") {
    numfile = 6;
  } else if (player -> pieces[nextMove.piece_moved] -> file == "g") {
    numfile = 7;
  } else if (player -> pieces[nextMove.piece_moved] -> file == "h") {
    numfile = 8;
  }
  
  //Erase piece from previous location on the board.
  board[player -> pieces[nextMove.piece_moved] -> rank][numfile].isOccupied = false;
  board[player -> pieces[nextMove.piece_moved] -> rank][numfile].owner = "";
  board[player -> pieces[nextMove.piece_moved] -> rank][numfile].piece = 0;
  
  //Get file number for where the piece would end up.
  if (nextMove.destination_file == "a") {
    numfile = 1;
  } else if (nextMove.destination_file == "b") {
    numfile = 2;
  } else if (nextMove.destination_file == "c") {
    numfile = 3;
  } else if (nextMove.destination_file == "d") {
    numfile = 4;
  } else if (nextMove.destination_file == "e") {
    numfile = 5;
  } else if (nextMove.destination_file == "f") {
    numfile = 6;
  } else if (nextMove.destination_file == "g") {
    numfile = 7;
  } else if (nextMove.destination_file == "h") {
    numfile = 8;
  }
  
  //Place piece in new location on board.
  board[nextMove.destination_rank][numfile].isOccupied = true;
  board[nextMove.destination_rank][numfile].owner = player -> color;
  board[nextMove.destination_rank][numfile].piece = nextMove.piece_moved;
  
  ///////////////////////////
  //Get King rank and file.//
  ///////////////////////////
  
  //If King is piece moved.
  if (player -> pieces[nextMove.piece_moved] -> type == "King") {
    rank = nextMove.destination_rank;
    if (nextMove.destination_file == "a") {
      numfile = 1;
    } else if (nextMove.destination_file == "b") {
      numfile = 2;
    } else if (nextMove.destination_file == "c") {
      numfile = 3;
    } else if (nextMove.destination_file == "d") {
      numfile = 4;
    } else if (nextMove.destination_file == "e") {
      numfile = 5;
    } else if (nextMove.destination_file == "f") {
      numfile = 6;
    } else if (nextMove.destination_file == "g") {
      numfile = 7;
    } else if (nextMove.destination_file == "h") {
      numfile = 8;
    }
    
  } else {
    //Find King piece.
    for (int l = 0; l < player->pieces.size(); l++) {
      if (player->pieces[l]->type == "King") {
        king = l;
      }
    }
  
    //Get rank and file.
    rank = player->pieces[king]->rank;
  
    if (player->pieces[king]->file == "a") {
      numfile = 1;
    } else if (player->pieces[king]->file == "b") {
      numfile = 2;
    } else if (player->pieces[king]->file == "c") {
      numfile = 3;
    } else if (player->pieces[king]->file == "d") {
      numfile = 4;
    } else if (player->pieces[king]->file == "e") {
      numfile = 5;
    } else if (player->pieces[king]->file == "f") {
      numfile = 6;
    } else if (player->pieces[king]->file == "g") {
      numfile = 7;
    } else if (player->pieces[king]->file == "h") {
      numfile = 8;
    }
  
  } //Get King rank and file.
  
  /////////////////////////////////////////////////////////////////////////////////
  //Check 4 directions and 4 diagonals to see if a piece places my king in check.//
  /////////////////////////////////////////////////////////////////////////////////
  
  ///////////////////////
  //Check 4 directions.//
  ///////////////////////
  
  //Check positive rank.
  done = false;
  for (int i = (rank+1); i <= 8; i++) {
    if (board[i][numfile].isOccupied == true && done == false) {
      if (board[i][numfile].owner == player -> opponent -> color) {
        if (i == (rank+1) && player -> opponent -> pieces[board[i][numfile].piece] -> type == "King") {
          valid = false;
        }
        if (player -> opponent -> pieces[board[i][numfile].piece] -> type == "Rook" ||
            player -> opponent -> pieces[board[i][numfile].piece] -> type == "Queen") {
          valid = false;
        }
        
      }
      
      done = true;
    }
    
  } //Positive rank.
  
  //Check negative rank.
  done = false;
  for (int i = (rank-1); i > 0; i--) {
    if (board[i][numfile].isOccupied == true && done == false) {
      if (board[i][numfile].owner == player -> opponent -> color) {
        if (i == (rank-1) && player -> opponent -> pieces[board[i][numfile].piece] -> type == "King") {
          valid = false;
        }
        if (player -> opponent -> pieces[board[i][numfile].piece] -> type == "Rook" ||
            player -> opponent -> pieces[board[i][numfile].piece] -> type == "Queen") {
          valid = false;
        }
        
      }
      
      done = true;
    }
    
  } //Negative Rank
  
  //Check positive file.
  done = false;
  for (int i = (numfile+1); i <= 8; i++) {
    if (board[rank][i].isOccupied == true && done == false) {
      if (board[rank][i].owner == player -> opponent -> color) {
        if (i == (numfile+1) && player -> opponent -> pieces[board[i][numfile].piece] -> type == "King") {
          valid = false;
        }
        if (player -> opponent -> pieces[board[rank][i].piece] -> type == "Rook" ||
            player -> opponent -> pieces[board[rank][i].piece] -> type == "Queen") {
          valid = false;
        }
        
      }
      
      done = true;
    }
    
  } //Positive File.
  
  
  //Check negative file.
  done = false;
  for (int i = (numfile-1); i > 0; i--) {
    if (board[rank][i].isOccupied == true && done == false) {
      if (board[rank][i].owner == player -> opponent -> color) {
        if (i == (numfile-1) && player -> opponent -> pieces[board[i][numfile].piece] -> type == "King") {
          valid = false;
        }
        if (player -> opponent -> pieces[board[rank][i].piece] -> type == "Rook" ||
            player -> opponent -> pieces[board[rank][i].piece] -> type == "Queen") {
          valid = false;
        }
        
      }
      
      done = true;
    }
    
  } //Negative File.
  
  ////////////////////
  //Check Diagonals.//
  ////////////////////
  
  //Check 1st quadrant diagonal.
  done = false;
  if (rank > numfile) {
    diagMoves = (8 - rank);
  } else {
    diagMoves = (8 - numfile);
  }
  for (int i = 1; i < diagMoves; i++) {
    if (board[rank + i][numfile + i].isOccupied && done == false) {
      if (board[rank + i][numfile + i].owner == player -> opponent -> color) {
      //If a pawn is in striking distance.
      if (i == 1 && player->opponent->rank_direction < 0 &&
          player->opponent->pieces[board[rank + i][numfile + i].piece]->type == "Pawn") {
        valid = false;
      }
      //If a king can attack.
      if (i == 1 && player->opponent->pieces[board[rank + i][numfile + i].piece]->type == "King") {
        valid = false;
      }
      //If a bishop or queen can attack.
      if (player->opponent->pieces[board[rank + i][numfile + i].piece]->type == "Bishop" ||
          player->opponent->pieces[board[rank + i][numfile + i].piece]->type == "Queen") {
        valid = false;
      }
    
    }
  
    done = true;
  }
    
  } //1st Quadrant Diagonal
  
  
  //Check 2nd quadrant diagonal.
  done = false;
  if ((8 - rank) < numfile) {
    diagMoves = (8 - rank);
  } else {
    diagMoves = (numfile - 1);
  }
  for (int i = 1; i < diagMoves; i++) {
    if (board[rank + i][numfile - i].isOccupied && done == false) {
      if (board[rank + i][numfile - i].owner == player -> opponent -> color) {
        //If a pawn is in striking distance.
        if (i == 1 && player->opponent->rank_direction < 0 &&
            player->opponent->pieces[board[rank + i][numfile - i].piece]->type == "Pawn") {
          valid = false;
        }
        //If a king can attack.
        if (i == 1 && player->opponent->pieces[board[rank + i][numfile - i].piece]->type == "King") {
          valid = false;
        }
        //If a bishop or queen can attack.
        if (player->opponent->pieces[board[rank + i][numfile - i].piece]->type == "Bishop" ||
            player->opponent->pieces[board[rank + i][numfile - i].piece]->type == "Queen") {
          valid = false;
        }
        
      }
    
      done = true;
    }
    
  }
  
  //Check 3rd quadrant diagonal.
  done = false;
  if (rank > numfile) {
    diagMoves = (numfile - 1);
  } else {
    diagMoves = (rank - 1);
  }
  for (int i = 1; i < diagMoves; i++) {
    if (board[rank - i][numfile - i].isOccupied && done == false) {
      if (board[rank - i][numfile - i].owner == player -> opponent -> color) {
        //If a pawn is in striking distance.
        if (i == 1 && player->opponent->rank_direction > 0 &&
            player->opponent->pieces[board[rank - i][numfile - i].piece]->type == "Pawn") {
          valid = false;
        }
        //If a king can attack.
        if (i == 1 && player->opponent->pieces[board[rank - i][numfile - i].piece]->type == "King") {
          valid = false;
        }
        //If a bishop or queen can attack.
        if (player->opponent->pieces[board[rank - i][numfile - i].piece]->type == "Bishop" ||
            player->opponent->pieces[board[rank - i][numfile - i].piece]->type == "Queen") {
          valid = false;
        }
        
      }
      
      done = true;
    }
    
  }
  
  //Check 4th quadrant diagonal.
  done = false;
  if (numfile <= (8 - rank)) {
    diagMoves = (rank - 1);
  } else {
    diagMoves = (8 - numfile);
  }
  for (int i = 1; i < diagMoves; i++) {
    if (board[rank - i][numfile + i].isOccupied && done == false) {
      if (board[rank - i][numfile + i].owner == player -> opponent -> color) {
        //If a pawn is in striking distance.
        if (i == 1 && player->opponent->rank_direction > 0 &&
            player->opponent->pieces[board[rank - i][numfile + i].piece]->type == "Pawn") {
          valid = false;
        }
        //If a king can attack.
        if (i == 1 && player->opponent->pieces[board[rank - i][numfile + i].piece]->type == "King") {
          valid = false;
        }
        //If a bishop or queen can attack.
        if (player->opponent->pieces[board[rank - i][numfile + i].piece]->type == "Bishop" ||
            player->opponent->pieces[board[rank - i][numfile + i].piece]->type == "Queen") {
          valid = false;
        }
  
      }
      
      done = true;
    }
    
  }
  
  //Check for possible knights placing the king in check.
  
  /////////////////////
  //Check for Knights//
  /////////////////////
  
  //+1 +2
  if (rank + 1 <= 8 && numfile + 2 <= 8) {
    if (board[rank + 1][numfile + 2].isOccupied) {
      if (board[rank + 1][numfile + 2].owner == player->opponent->color) {
        if (player->opponent->pieces[board[rank + 1][numfile + 2].piece]->type == "Knight") {
          valid = false;
        }
      
      }
    
    }
    
  }
  
  //+2 +1
  if (rank + 2 <= 8 && numfile + 1 <= 8) {
    if (board[rank + 2][numfile + 1].isOccupied) {
      if (board[rank + 2][numfile + 1].owner == player->opponent->color) {
        if (player->opponent->pieces[board[rank + 2][numfile + 1].piece]->type == "Knight") {
          valid = false;
        }
      
      }
    
    }
  
  }
  
  //+1 -2
  if (rank + 1 <= 8 && numfile - 2 > 0) {
    if (board[rank + 1][numfile - 2].isOccupied) {
      if (board[rank + 1][numfile - 2].owner == player->opponent->color) {
        if (player->opponent->pieces[board[rank + 1][numfile - 2].piece]->type == "Knight") {
          valid = false;
        }
      
      }
    
    }
  
  }
  
  //+2 -1
  if (rank + 2 <= 8 && numfile - 1 > 0) {
    if (board[rank + 2][numfile - 1].isOccupied) {
      if (board[rank + 2][numfile - 1].owner == player->opponent->color) {
        if (player->opponent->pieces[board[rank + 2][numfile - 1].piece]->type == "Knight") {
          valid = false;
        }
      
      }
    
    }
  
  }
  
  //-1 -2
  if (rank - 1 > 0 && numfile - 2 > 0) {
    if (board[rank - 1][numfile - 2].isOccupied) {
      if (board[rank - 1][numfile - 2].owner == player->opponent->color) {
        if (player->opponent->pieces[board[rank - 1][numfile - 2].piece]->type == "Knight") {
          valid = false;
        }
      
      }
    
    }
  
  }
  
  //-2 -1
  if (rank - 2 > 0 && numfile - 1 > 0) {
    if (board[rank - 2][numfile - 1].isOccupied) {
      if (board[rank - 2][numfile - 1].owner == player->opponent->color) {
        if (player->opponent->pieces[board[rank - 2][numfile - 1].piece]->type == "Knight") {
          valid = false;
        }
      
      }
    
    }
  
  }
  
  //-1 +2
  if (rank - 1 > 0 && numfile + 2 > 0) {
    if (board[rank - 1][numfile + 2].isOccupied) {
      if (board[rank - 1][numfile + 2].owner == player->opponent->color) {
        if (player->opponent->pieces[board[rank - 1][numfile + 2].piece]->type == "Knight") {
          valid = false;
        }
      
      }
    
    }
  
  }
  
  //-2 +1
  if (rank - 2 > 0 && numfile + 1 > 0) {
    if (board[rank - 2][numfile + 1].isOccupied) {
      if (board[rank - 2][numfile + 1].owner == player->opponent->color) {
        if (player->opponent->pieces[board[rank - 2][numfile + 1].piece]->type == "Knight") {
          valid = false;
        }
      
      }
    
    }
  
  }
  
  //Return value of valid
  return valid;
} //Valid Move Function.
    
    
    

} // chess

} // cpp_client
