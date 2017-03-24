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
  /*
  //Ask user for depth limit.
  std::cout<<"Please enter a depth limit for ID DLM."<<std::endl;
  std::cin>>depthLimit;
   */
  depthLimit = 4;
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
  
  //Variable Declaration:
  std::vector< std::vector<tile> >  gameBoard;
  std::vector<ai_move> allMoves;
  ai_move toMove;
  int randNum;
  
  //Variable Declaration:
  std::vector<ai_piece> myPieces;
  std::vector<ai_piece> oppPieces;
  ai_piece tempPiece;
  rep_move tempMove;
  
  //Populate myPieces and oppPieces.
  myPieces.clear();
  oppPieces.clear();
  for (int j = 0; j < player -> pieces.size(); j++) {
    tempPiece.id = player -> pieces[j] -> id;
    tempPiece.rank = player -> pieces[j] -> rank;
    tempPiece.file = player -> pieces[j] -> file;
    tempPiece.type = player -> pieces[j] -> type;
    tempPiece.captured = player -> pieces[j] -> captured;
    myPieces.push_back(tempPiece);
  }
  for (int j = 0; j < player -> opponent -> pieces.size(); j++) {
    tempPiece.id = player -> opponent -> pieces[j] -> id;
    tempPiece.rank = player -> opponent -> pieces[j] -> rank;
    tempPiece.file = player -> opponent -> pieces[j] -> file;
    tempPiece.type = player -> opponent -> pieces[j] -> type;
    tempPiece.captured = player -> opponent -> pieces[j] -> captured;
    oppPieces.push_back(tempPiece);
  }
  
  //Get a copy of the gameboard for the current turn.
  gameBoard = getBoard();
  
  //Call the MiniMax function.
  toMove = miniMax(myPieces, oppPieces, depthLimit, gameBoard);
  
  //Output the move made to the terminal.
  std::cout<<player -> pieces[toMove.piece_moved] -> type<<" "<<player -> pieces[toMove.piece_moved] -> file<<player -> pieces[toMove.piece_moved] -> rank<<" : "
           <<toMove.destination_file<<toMove.destination_rank<<std::endl;
  
  //Place the move to be made in the vector of previous moves.
  tempMove.piece_moved = toMove.piece_moved;
  tempMove.origin_file = player -> pieces[toMove.piece_moved] -> file;
  tempMove.origin_rank = player -> pieces[toMove.piece_moved] -> rank;
  tempMove.destination_file = toMove.destination_file;
  tempMove.destination_rank = toMove.destination_rank;
  previous_moves.push_back(tempMove);
  
  //If size is greater that 3, erase the front (oldest) move.
  if (previous_moves.size() > 3) {
    previous_moves.erase(previous_moves.begin());
  }
  
  //Make the move selected by MiniMax
  player -> pieces[toMove.piece_moved] -> move(toMove.destination_file, toMove.destination_rank, "Queen");
  
  /*
  //Get all the possible moves for this turn.
  allMoves = action(myPieces, oppPieces, gameBoard, player -> color, player -> opponent -> color, player -> rank_direction);
  
  //Get a random number for picking the move.
  randNum = (rand() % allMoves.size());
  
  //Print out starting position of piece to move.
  std::cout<<player -> pieces[allMoves[randNum].piece_moved] -> type<<" "<<player -> pieces[allMoves[randNum].piece_moved] -> file<<player -> pieces[allMoves[randNum].piece_moved] -> rank<<": ";
  
  //Make the move selected by the random number.
  player -> pieces[allMoves[randNum].piece_moved] -> move(allMoves[randNum].destination_file, allMoves[randNum].destination_rank, "Queen");
  
  //Print out all possible moves for the piece that was moved.
  for (int i = 0; i < allMoves.size(); i++) {
    if (player -> pieces[allMoves[i].piece_moved] -> id == player -> pieces[allMoves[randNum].piece_moved] -> id) {
      std::cout<<allMoves[i].destination_file<<allMoves[i].destination_rank<<" ";
    }
  }
  std::cout<<std::endl;
  */
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
std::vector< std::vector<tile> >  AI::getBoard() {
  //Variable Declaration:
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
  for (int i=0; i < player -> opponent -> pieces.size(); i++)
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
std::vector<ai_move> AI::action(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, std::vector< std::vector<tile> >  board,
                                std::string myColor, std::string oppColor, int rankDir) {
  //Variable Declaration:
  std::vector<ai_move> allMoves;
  std::vector<ai_move> tempMoves;
  ai_move temp;
  
  allMoves.clear();
  
  //For every piece owned by this AI, get every possible move by that piece and concatenate it with the moves from all
  //the other pieces to create a vector with all possible moves.
  for (int i = 0; i < myPieces.size(); i++) {
    if (myPieces[i].captured == false) {
      tempMoves.clear();
      
      tempMoves = get_Move(myPieces, oppPieces, myPieces[i], i, board, myColor, oppColor, rankDir);
      
      allMoves.insert(allMoves.end(), tempMoves.begin(), tempMoves.end());
    }
    
  }
  
  return allMoves;
} //Action Function.

//Returns vector of all possible moves the given piece can make.
std::vector<ai_move> AI::get_Move(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, ai_piece toMove, int numPiece,
                                  std::vector< std::vector<tile> >  board, std::string myColor, std::string oppColor, int rankDir) {
  //Variable Declaration:
  std::vector<ai_move> moves_Got;
  std::vector<ai_move> tempMoves;
  ai_move temp;
  int numFile;
  int rank;
  
  moves_Got.clear();
  
  //Get numerical value for the file of the piece.
  if (toMove.file == "a") {
    numFile = 1;
  } else if (toMove.file == "b") {
    numFile = 2;
  } else if (toMove.file == "c") {
    numFile = 3;
  } else if (toMove.file == "d") {
    numFile = 4;
  } else if (toMove.file == "e") {
    numFile = 5;
  } else if (toMove.file == "f") {
    numFile = 6;
  } else if (toMove.file == "g") {
    numFile = 7;
  } else if (toMove.file == "h") {
    numFile = 8;
  }
  
  //Get rank of piece to move.
  rank = toMove.rank;
  
  ////////////////////////////////////
  //If Pawn, make appropriate moves.//
  ////////////////////////////////////
  
  if (toMove.type == "Pawn") {
    tempMoves.clear();
    
    tempMoves = pawn_movement(myPieces, oppPieces, toMove, numPiece, numFile, rank, board, myColor, oppColor, rankDir);
    moves_Got.insert(moves_Got.end(), tempMoves.begin(), tempMoves.end());
    
  } //Pawn movement.
  
  ///////////////////////////////////
  //If Rook, make appropiate moves.//
  ///////////////////////////////////
  
  if (toMove.type == "Rook") {
    tempMoves.clear();
    
    tempMoves = rook_movement(myPieces, oppPieces, toMove, numPiece, numFile, rank, board, myColor, oppColor, rankDir);
    moves_Got.insert(moves_Got.end(), tempMoves.begin(), tempMoves.end());
    
  } //Rook Movement.
  
  /////////////////////////////////////
  //If Bishop, make appropiate moves.//
  /////////////////////////////////////
  
  if (toMove.type == "Bishop") {
    tempMoves.clear();
    
    tempMoves = bishop_movement(myPieces, oppPieces, toMove, numPiece, numFile, rank, board, myColor, oppColor, rankDir);
    moves_Got.insert(moves_Got.end(), tempMoves.begin(), tempMoves.end());
    
  } //Bishop Movement.
  
  /////////////////////////////////////
  //If Knight, make appropiate moves.//
  /////////////////////////////////////
  
  if (toMove.type == "Knight") {
    tempMoves.clear();
    
    tempMoves = knight_movement(myPieces, oppPieces, toMove, numPiece, numFile, rank, board, myColor, oppColor, rankDir);
    moves_Got.insert(moves_Got.end(), tempMoves.begin(), tempMoves.end());
    
  } //Knight Movement.
  
  ////////////////////////////////////
  //If Queen, make appropiate moves.//
  ////////////////////////////////////
  
  if (toMove.type == "Queen") {
    tempMoves.clear();
    
    tempMoves = rook_movement(myPieces, oppPieces, toMove, numPiece, numFile, rank, board, myColor, oppColor, rankDir);
    moves_Got.insert(moves_Got.end(), tempMoves.begin(), tempMoves.end());
    
    tempMoves.clear();
    
    tempMoves = bishop_movement(myPieces, oppPieces, toMove, numPiece, numFile, rank, board, myColor, oppColor, rankDir);
    moves_Got.insert(moves_Got.end(), tempMoves.begin(), tempMoves.end());
    
  } //Queen Movement.
  
  ///////////////////////////////////
  //If King, make appropiate moves.//
  ///////////////////////////////////
  
  if (toMove.type == "King") {
    tempMoves.clear();
    
    tempMoves = king_movement(myPieces, oppPieces, toMove, numPiece, numFile, rank, board, myColor, oppColor, rankDir);
    
    moves_Got.insert(moves_Got.end(), tempMoves.begin(), tempMoves.end());
    
  } //King Movement.
  
  return moves_Got;
} //Get Move Function.

//Returns true if the proposed move is valid.
bool AI::valid_move(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, chess::ai_move nextMove, std::vector< std::vector<tile> > board,
                    std::string myColor, std::string oppColor, int rankDir) {
  //Variable Declaration:
  int king;
  int rank;
  int numfile;
  //int destfile;
  int diagMoves;
  bool valid;
  bool done;
  
  valid = true;
  
  /////////////////////////////////////
  //Apply potential move to the board//
  /////////////////////////////////////
  /*
  //Get file number for where the piece originates.
  if (myPieces[nextMove.piece_moved].file == "a") {
    numfile = 1;
  } else if (myPieces[nextMove.piece_moved].file == "b") {
    numfile = 2;
  } else if (myPieces[nextMove.piece_moved].file == "c") {
    numfile = 3;
  } else if (myPieces[nextMove.piece_moved].file == "d") {
    numfile = 4;
  } else if (myPieces[nextMove.piece_moved].file == "e") {
    numfile = 5;
  } else if (myPieces[nextMove.piece_moved].file == "f") {
    numfile = 6;
  } else if (myPieces[nextMove.piece_moved].file == "g") {
    numfile = 7;
  } else if (myPieces[nextMove.piece_moved].file == "h") {
    numfile = 8;
  }
  
  //Get file number for where the piece would end up.
  if (nextMove.destination_file == "a") {
    destfile = 1;
  } else if (nextMove.destination_file == "b") {
    destfile = 2;
  } else if (nextMove.destination_file == "c") {
    destfile = 3;
  } else if (nextMove.destination_file == "d") {
    destfile = 4;
  } else if (nextMove.destination_file == "e") {
    destfile = 5;
  } else if (nextMove.destination_file == "f") {
    destfile = 6;
  } else if (nextMove.destination_file == "g") {
    destfile = 7;
  } else if (nextMove.destination_file == "h") {
    destfile = 8;
  }
  
  //Place piece in new location on board.
  board[nextMove.destination_rank][destfile].isOccupied = true;
  board[nextMove.destination_rank][destfile].owner = board[myPieces[nextMove.piece_moved].rank][numfile].owner;
  board[nextMove.destination_rank][destfile].piece = nextMove.piece_moved;
  
  //Erase piece from previous location on the board.
  board[myPieces[nextMove.piece_moved].rank][numfile].isOccupied = false;
  board[myPieces[nextMove.piece_moved].rank][numfile].owner = "";
  board[myPieces[nextMove.piece_moved].rank][numfile].piece = 0;
  */
  
  //Get file number for where the piece originates.
  if (myPieces[nextMove.piece_moved].file == "a") {
    numfile = 1;
  } else if (myPieces[nextMove.piece_moved].file == "b") {
    numfile = 2;
  } else if (myPieces[nextMove.piece_moved].file == "c") {
    numfile = 3;
  } else if (myPieces[nextMove.piece_moved].file == "d") {
    numfile = 4;
  } else if (myPieces[nextMove.piece_moved].file == "e") {
    numfile = 5;
  } else if (myPieces[nextMove.piece_moved].file == "f") {
    numfile = 6;
  } else if (myPieces[nextMove.piece_moved].file == "g") {
    numfile = 7;
  } else if (myPieces[nextMove.piece_moved].file == "h") {
    numfile = 8;
  }
  
  //Erase piece from previous location on the board.
  board[myPieces[nextMove.piece_moved].rank][numfile].isOccupied = false;
  board[myPieces[nextMove.piece_moved].rank][numfile].owner = "";
  board[myPieces[nextMove.piece_moved].rank][numfile].piece = 0;
  
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
  board[nextMove.destination_rank][numfile].owner = myColor;
  board[nextMove.destination_rank][numfile].piece = nextMove.piece_moved;
  
  ///////////////////////////
  //Get King rank and file.//
  ///////////////////////////
  
  //If King is piece moved.
  if (myPieces[nextMove.piece_moved].type == "King") {
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
    for (int l = 0; l < myPieces.size(); l++) {
      if (myPieces[l].type == "King") {
        king = l;
      }
    }
  
    //Get rank and file.
    rank = myPieces[king].rank;
  
    if (myPieces[king].file == "a") {
      numfile = 1;
    } else if (myPieces[king].file == "b") {
      numfile = 2;
    } else if (myPieces[king].file == "c") {
      numfile = 3;
    } else if (myPieces[king].file == "d") {
      numfile = 4;
    } else if (myPieces[king].file == "e") {
      numfile = 5;
    } else if (myPieces[king].file == "f") {
      numfile = 6;
    } else if (myPieces[king].file == "g") {
      numfile = 7;
    } else if (myPieces[king].file == "h") {
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
      if (board[i][numfile].owner == oppColor) {
        if (i == (rank+1) && oppPieces[board[i][numfile].piece].type == "King") {
          valid = false;
        }
        if (oppPieces[board[i][numfile].piece].type == "Rook" ||
            oppPieces[board[i][numfile].piece].type == "Queen") {
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
      if (board[i][numfile].owner == oppColor) {
        if (i == (rank-1) && oppPieces[board[i][numfile].piece].type == "King") {
          valid = false;
        }
        if (oppPieces[board[i][numfile].piece].type == "Rook" ||
            oppPieces[board[i][numfile].piece].type == "Queen") {
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
      if (board[rank][i].owner == oppColor) {
        if (i == (numfile+1) && oppPieces[board[rank][i].piece].type == "King") {
          valid = false;
        }
        if (oppPieces[board[rank][i].piece].type == "Rook" ||
            oppPieces[board[rank][i].piece].type == "Queen") {
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
      if (board[rank][i].owner == oppColor) {
        if (i == (numfile-1) && oppPieces[board[rank][i].piece].type == "King") {
          valid = false;
        }
        if (oppPieces[board[rank][i].piece].type == "Rook" ||
            oppPieces[board[rank][i].piece].type == "Queen") {
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
  for (int i = 1; i <= diagMoves; i++) {
    if (board[rank + i][numfile + i].isOccupied && done == false) {
      if (board[rank + i][numfile + i].owner == oppColor) {
      //If a pawn is in striking distance.
      if (i == 1 && (rankDir * -1) < 0 &&
          oppPieces[board[rank + i][numfile + i].piece].type == "Pawn") {
        valid = false;
      }
      //If a king can attack.
      if (i == 1 && oppPieces[board[rank + i][numfile + i].piece].type == "King") {
        valid = false;
      }
      //If a bishop or queen can attack.
      if (oppPieces[board[rank + i][numfile + i].piece].type == "Bishop" ||
          oppPieces[board[rank + i][numfile + i].piece].type == "Queen") {
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
  for (int i = 1; i <= diagMoves; i++) {
    if (board[rank + i][numfile - i].isOccupied && done == false) {
      if (board[rank + i][numfile - i].owner == oppColor) {
        //If a pawn is in striking distance.
        if (i == 1 && (rankDir * -1) < 0 &&
            oppPieces[board[rank + i][numfile - i].piece].type == "Pawn") {
          valid = false;
        }
        //If a king can attack.
        if (i == 1 && oppPieces[board[rank + i][numfile - i].piece].type == "King") {
          valid = false;
        }
        //If a bishop or queen can attack.
        if (oppPieces[board[rank + i][numfile - i].piece].type == "Bishop" ||
            oppPieces[board[rank + i][numfile - i].piece].type == "Queen") {
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
  for (int i = 1; i <= diagMoves; i++) {
    if (board[rank - i][numfile - i].isOccupied && done == false) {
      if (board[rank - i][numfile - i].owner == oppColor) {
        //If a pawn is in striking distance.
        if (i == 1 && (rankDir * -1) > 0 &&
         oppPieces[board[rank - i][numfile - i].piece].type == "Pawn") {
          valid = false;
        }
        //If a king can attack.
        if (i == 1 && oppPieces[board[rank - i][numfile - i].piece].type == "King") {
          valid = false;
        }
        //If a bishop or queen can attack.
        if (oppPieces[board[rank - i][numfile - i].piece].type == "Bishop" ||
            oppPieces[board[rank - i][numfile - i].piece].type == "Queen") {
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
  for (int i = 1; i <= diagMoves; i++) {
    if (board[rank - i][numfile + i].isOccupied && done == false) {
      if (board[rank - i][numfile + i].owner == oppColor) {
        //If a pawn is in striking distance.
        if (i == 1 && (rankDir * -1) > 0 &&
         oppPieces[board[rank - i][numfile + i].piece].type == "Pawn") {
          valid = false;
        }
        //If a king can attack.
        if (i == 1 && oppPieces[board[rank - i][numfile + i].piece].type == "King") {
          valid = false;
        }
        //If a bishop or queen can attack.
        if (oppPieces[board[rank - i][numfile + i].piece].type == "Bishop" ||
         oppPieces[board[rank - i][numfile + i].piece].type == "Queen") {
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
      if (board[rank + 1][numfile + 2].owner == oppColor) {
        if (oppPieces[board[rank + 1][numfile + 2].piece].type == "Knight") {
          valid = false;
        }
      
      }
    
    }
    
  }
  
  //+2 +1
  if (rank + 2 <= 8 && numfile + 1 <= 8) {
    if (board[rank + 2][numfile + 1].isOccupied) {
      if (board[rank + 2][numfile + 1].owner == oppColor) {
        if (oppPieces[board[rank + 2][numfile + 1].piece].type == "Knight") {
          valid = false;
        }
      
      }
    
    }
  
  }
  
  //+1 -2
  if (rank + 1 <= 8 && numfile - 2 > 0) {
    if (board[rank + 1][numfile - 2].isOccupied) {
      if (board[rank + 1][numfile - 2].owner == oppColor) {
        if (oppPieces[board[rank + 1][numfile - 2].piece].type == "Knight") {
          valid = false;
        }
      
      }
    
    }
  
  }
  
  //+2 -1
  if (rank + 2 <= 8 && numfile - 1 > 0) {
    if (board[rank + 2][numfile - 1].isOccupied) {
      if (board[rank + 2][numfile - 1].owner == oppColor) {
        if (oppPieces[board[rank + 2][numfile - 1].piece].type == "Knight") {
          valid = false;
        }
      
      }
    
    }
  
  }
  
  //-1 -2
  if (rank - 1 > 0 && numfile - 2 > 0) {
    if (board[rank - 1][numfile - 2].isOccupied) {
      if (board[rank - 1][numfile - 2].owner == oppColor) {
        if (oppPieces[board[rank - 1][numfile - 2].piece].type == "Knight") {
          valid = false;
        }
      
      }
    
    }
  
  }
  
  //-2 -1
  if (rank - 2 > 0 && numfile - 1 > 0) {
    if (board[rank - 2][numfile - 1].isOccupied) {
      if (board[rank - 2][numfile - 1].owner == oppColor) {
        if (oppPieces[board[rank - 2][numfile - 1].piece].type == "Knight") {
          valid = false;
        }
      
      }
    
    }
  
  }
  
  //-1 +2
  if (rank - 1 > 0 && numfile + 2 <= 8) {
    if (board[rank - 1][numfile + 2].isOccupied) {
      if (board[rank - 1][numfile + 2].owner == oppColor) {
        if (oppPieces[board[rank - 1][numfile + 2].piece].type == "Knight") {
          valid = false;
        }
        
      }
      
    }
    
  }
  
  //-2 +1
  if (rank - 2 > 0 && numfile + 1 <= 8) {
    if (board[rank - 2][numfile + 1].isOccupied) {
      if (board[rank - 2][numfile + 1].owner == oppColor) {
        if (oppPieces[board[rank - 2][numfile + 1].piece].type == "Knight") {
          valid = false;
        }
      
      }
    
    }
  
  }
  
  //Return value of valid
  return valid;
} //Valid Move Function.

//Pawn Movement Function.
std::vector<ai_move> AI::pawn_movement(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, ai_piece toMove, int numPiece, int numFile, int rank,
                                       std::vector< std::vector<tile> >  board, std::string myColor, std::string oppColor, int rankDir) {
  //Variable Declaration:
  ai_move temp;
  std::vector<ai_move> moves_Got;
  
  //If forward move is legal.
  if (board[toMove.rank + rankDir][numFile].isOccupied == false) {
    temp.destination_file = toMove.file;
    temp.destination_rank = (toMove.rank + rankDir);
    temp.piece_moved = numPiece;
    
    if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir))
    {
      moves_Got.push_back(temp);
    }
    
  } //Forward move.
  
  //If double forward move is legal.
  if ( ( (rankDir > 0) && (rank == 2) ) || ( (rankDir < 0) && (rank == 7) ) ) {
    if ((board[toMove.rank + (rankDir * 2)][numFile].isOccupied == false) && (board[toMove.rank + rankDir][numFile].isOccupied == false)) {
      temp.destination_file = toMove.file;
      temp.destination_rank = (toMove.rank + (rankDir * 2));
      temp.piece_moved = numPiece;
  
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir))
      {
        moves_Got.push_back(temp);
      }
      
    }
    
  } //Double Forward Move.
  
  //If capturing right diagonal is legal.
  if (numFile + 1 <= 8) {
    if (board[toMove.rank + rankDir][numFile + 1].isOccupied == true) {
      if (board[toMove.rank + rankDir][numFile + 1].owner != myColor) {
        temp.destination_file = ('a' + numFile);
        temp.destination_rank = (toMove.rank + rankDir);
        temp.piece_moved = numPiece;
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir))
        {
          moves_Got.push_back(temp);
        }
      }
      
    }
    
  } //Right diagonal.
  
  //If capturing left diagonal is legal.
  if (numFile - 1 > 0) {
    if (board[toMove.rank + rankDir][numFile - 1].isOccupied == true) {
      if (board[toMove.rank + rankDir][numFile - 1].owner != myColor) {
        temp.destination_file = ('a' + numFile - 2);
        temp.destination_rank = (toMove.rank + rankDir);
        temp.piece_moved = numPiece;
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir))
        {
          moves_Got.push_back(temp);
        }
      }
      
    }
    
  } //Left diagonal.
  
  return moves_Got;
} //Pawn Movement Function.

//Bishop Movement Function.
std::vector<ai_move> AI::bishop_movement(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, ai_piece toMove, int numPiece, int numFile, int rank,
                                         std::vector< std::vector<tile> >  board, std::string myColor, std::string oppColor, int rankDir) {
  //Variable Declaration:
  ai_move temp;
  std::vector<ai_move> moves_Got;
  bool done;
  int diagMoves;
  
  //Check 1st quadrant diagonal.
  done = false;
  if (rank > numFile) {
    diagMoves = (8 - rank);
  } else {
    diagMoves = (8 - numFile);
  }
  for (int i = 1; i <= diagMoves; i++) {
    if (board[rank + i][numFile + i].isOccupied && done == false) {
      if (board[rank + i][numFile + i].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank + i;
        temp.destination_file = ('a' + numFile - 1 + i);
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
      }
      
      done = true;
    } else if (done == false) {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank + i;
      temp.destination_file = ('a' + numFile - 1 + i);
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  } //1st Quadrant Diagonal
  
  //Check 2nd quadrant diagonal.
  done = false;
  if ((8 - rank) < numFile) {
    diagMoves = (8 - rank);
  } else {
    diagMoves = (numFile - 1);
  }
  for (int i = 1; i <= diagMoves; i++) {
    if (board[rank + i][numFile - i].isOccupied && done == false) {
      if (board[rank + i][numFile - i].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank + i;
        temp.destination_file = ('a' + numFile - 1 - i);
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
      }
      
      done = true;
    } else if (done == false) {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank + i;
      temp.destination_file = ('a' + numFile - 1 - i);
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  } //2nd Quadrant Diagonal
  
  //Check 3rd quadrant diagonal.
  done = false;
  if (rank > numFile) {
    diagMoves = (numFile - 1);
  } else {
    diagMoves = (rank - 1);
  }
  for (int i = 1; i <= diagMoves; i++) {
    if (board[rank - i][numFile - i].isOccupied && done == false) {
      if (board[rank - i][numFile - i].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank - i;
        temp.destination_file = ('a' + numFile - 1 - i);
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
      }
      
      done = true;
    } else if (done == false) {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank - i;
      temp.destination_file = ('a' + numFile - 1 - i);
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  } //3rd Quadrant Diagonal.
  
  //Check 4th quadrant diagonal.
  done = false;
  if (numFile <= (8 - rank)) {
    diagMoves = (rank - 1);
  } else {
    diagMoves = (8 - numFile);
  }
  for (int i = 1; i <= diagMoves; i++) {
    if (board[rank - i][numFile + i].isOccupied && done == false) {
      if (board[rank - i][numFile + i].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank - i;
        temp.destination_file = ('a' + numFile - 1 + i);
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
      }
      
      done = true;
    } else if (done == false) {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank - i;
      temp.destination_file = ('a' + numFile - 1 + i);
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  } //4th Quadrant Diagonal.
  
  return moves_Got;
} //Bishop Movement Function.

//Rook Movement Function.
std::vector<ai_move> AI::rook_movement(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, ai_piece toMove, int numPiece, int numFile, int rank,
                                       std::vector< std::vector<tile> >  board, std::string myColor, std::string oppColor, int rankDir) {
  //Variable Declaration:
  ai_move temp;
  std::vector<ai_move> moves_Got;
  bool done;
  done = false;
  
  //Positive rank movement.
  for (int i = rank + 1; i <= 8; i++) {
    if (board[i][numFile].isOccupied == true && done == false) {
      if (board[i][numFile].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = i;
        temp.destination_file = toMove.file;
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
      }
      
      done = true;
    } else if (done == false) {
      temp.piece_moved = numPiece;
      temp.destination_rank = i;
      temp.destination_file = toMove.file;
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  } //Positive Rank Movement.
  
  done = false;
  
  //Negative rank movement.
  for (int i = rank - 1; i > 0; i--) {
    if (board[i][numFile].isOccupied == true && done == false) {
      if (board[i][numFile].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = i;
        temp.destination_file = toMove.file;
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
      }
      
      done = true;
    } else if (done == false) {
      temp.piece_moved = numPiece;
      temp.destination_rank = i;
      temp.destination_file = toMove.file;
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  } // Negative Rank Movement.
  
  done = false;
  
  //Positive file movement.
  for (int i = numFile + 1; i <= 8; i++) {
    if (board[rank][i].isOccupied == true && done == false) {
      if (board[rank][i].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = toMove.rank;
        temp.destination_file = ('a' + i - 1);
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
      }
      
      done = true;
    } else if (done == false) {
      temp.piece_moved = numPiece;
      temp.destination_rank = toMove.rank;
      temp.destination_file = ('a' + i - 1);
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  } //Positive File Movement.
  
  done = false;
  
  //Negative file movement.
  for (int i = numFile - 1; i > 0; i--) {
    if (board[rank][i].isOccupied == true && done == false) {
      if (board[rank][i].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = toMove.rank;
        temp.destination_file = ('a' + i - 1);
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
      }
      
      done = true;
    } else if (done == false) {
      temp.piece_moved = numPiece;
      temp.destination_rank = toMove.rank;
      temp.destination_file = ('a' + i - 1);
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  } //Negative File Movement.
  
  return moves_Got;
} //Rook Movement Function.

//Knight Movement Function.
std::vector<ai_move> AI::knight_movement(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, ai_piece toMove, int numPiece, int numFile, int rank,
                                         std::vector< std::vector<tile> >  board, std::string myColor, std::string oppColor, int rankDir) {
  //Variable Declaration:
  ai_move temp;
  std::vector<ai_move> moves_Got;
  
  //+1 +2
  if (rank + 1 <= 8 && numFile + 2 <= 8) {
    if (board[rank + 1][numFile + 2].isOccupied) {
      if (board[rank + 1][numFile + 2].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank + 1;
        temp.destination_file = 'a' + numFile - 1 + 2;
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
      }
      
    } else {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank + 1;
      temp.destination_file = 'a' + numFile - 1 + 2;
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  }
  
  //+2 +1
  if (rank + 2 <= 8 && numFile + 1 <= 8) {
    if (board[rank + 2][numFile + 1].isOccupied) {
      if (board[rank + 2][numFile + 1].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank + 2;
        temp.destination_file = 'a' + numFile - 1 + 1;
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
        
      }
      
    } else {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank + 2;
      temp.destination_file = 'a' + numFile - 1 + 1;
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  }
  
  //+1 -2
  if (rank + 1 <= 8 && numFile - 2 > 0) {
    if (board[rank + 1][numFile - 2].isOccupied) {
      if (board[rank + 1][numFile - 2].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank + 1;
        temp.destination_file = 'a' + numFile - 1 - 2;
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
      }
      
    } else {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank + 1;
      temp.destination_file = 'a' + numFile - 1 - 2;
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  }
  
  //+2 -1
  if (rank + 2 <= 8 && numFile - 1 > 0) {
    if (board[rank + 2][numFile - 1].isOccupied) {
      if (board[rank + 2][numFile - 1].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank + 2;
        temp.destination_file = 'a' + numFile - 1 - 1;
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
        
      }
      
    } else {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank + 2;
      temp.destination_file = 'a' + numFile - 1 - 1;
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  }
  
  //-1 -2
  if (rank - 1 > 0 && numFile - 2 > 0) {
    if (board[rank - 1][numFile - 2].isOccupied) {
      if (board[rank - 1][numFile - 2].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank - 1;
        temp.destination_file = 'a' + numFile - 1 - 2;
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
        
      }
      
    } else {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank - 1;
      temp.destination_file = 'a' + numFile - 1 - 2;
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  }
  
  //-2 -1
  if (rank - 2 > 0 && numFile - 1 > 0) {
    if (board[rank - 2][numFile - 1].isOccupied) {
      if (board[rank - 2][numFile - 1].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank - 2;
        temp.destination_file = 'a' + numFile - 1 - 1;
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
        
      }
      
    } else {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank - 2;
      temp.destination_file = 'a' + numFile - 1 - 1;
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  }
  
  //-1 +2
  if (rank - 1 > 0 && numFile + 2 <= 8) {
    if (board[rank - 1][numFile + 2].isOccupied) {
      if (board[rank - 1][numFile + 2].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank - 1;
        temp.destination_file = 'a' + numFile - 1 + 2;
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
        
      }
      
    } else {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank - 1;
      temp.destination_file = 'a' + numFile - 1 + 2;
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  }
  
  //-2 +1
  if (rank - 2 > 0 && numFile + 1 <= 8) {
    if (board[rank - 2][numFile + 1].isOccupied) {
      if (board[rank - 2][numFile + 1].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank - 2;
        temp.destination_file = 'a' + numFile - 1 + 1;
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
        
      }
      
    } else {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank - 2;
      temp.destination_file = 'a' + numFile - 1 + 1;
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  }
  
  return moves_Got;
} //Knight Movement Function.

//King Movement Function.
std::vector<ai_move> AI::king_movement(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, ai_piece toMove, int numPiece, int numFile, int rank,
                                       std::vector< std::vector<tile> >  board, std::string myColor, std::string oppColor, int rankDir) {
  //Variable Declaration:
  ai_move temp;
  std::vector<ai_move> moves_Got;
  
  //+1 +0
  if (rank + 1 <= 8) {
    if (board[rank + 1][numFile ].isOccupied == true) {
      if (board[rank + 1][numFile ].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank + 1;
        temp.destination_file = ('a' + numFile - 1 );
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
      }
      
    } else {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank + 1;
      temp.destination_file = ('a' + numFile - 1 );
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  }
  
  //-1 +0
  if (rank - 1 > 0) {
    if (board[rank - 1][numFile ].isOccupied == true) {
      if (board[rank - 1][numFile ].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank - 1;
        temp.destination_file = ('a' + numFile - 1 );
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
      }
      
    } else {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank - 1;
      temp.destination_file = ('a' + numFile - 1 );
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  }
  
  //+0 +1
  if (numFile + 1 <= 8) {
    if (board[rank ][numFile + 1].isOccupied == true) {
      if (board[rank ][numFile + 1].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank ;
        temp.destination_file = ('a' + numFile - 1 + 1);
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
      }
      
    } else {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank ;
      temp.destination_file = ('a' + numFile - 1 + 1);
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  }
  
  //+0 -1
  if (numFile - 1 > 0) {
    if (board[rank ][numFile - 1].isOccupied == true) {
      if (board[rank ][numFile - 1].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank ;
        temp.destination_file = ('a' + numFile - 1 - 1);
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
      }
      
    } else {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank ;
      temp.destination_file = ('a' + numFile - 1 - 1);
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  }
  
  //+1 +1
  if (rank + 1 <= 8 && numFile + 1 <= 8) {
    if (board[rank + 1][numFile + 1].isOccupied == true) {
      if (board[rank + 1][numFile + 1].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank + 1;
        temp.destination_file = ('a' + numFile - 1 + 1);
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
      }
      
    } else {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank + 1;
      temp.destination_file = ('a' + numFile - 1 + 1);
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  }
  
  //+1 -1
  if (rank + 1 <= 8 && numFile - 1 > 0) {
    if (board[rank + 1][numFile - 1].isOccupied == true) {
      if (board[rank + 1][numFile - 1].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank + 1;
        temp.destination_file = ('a' + numFile - 1 - 1);
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
      }
      
    } else {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank + 1;
      temp.destination_file = ('a' + numFile - 1 - 1);
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  }
  
  //-1 -1
  if (rank - 1 > 0 && numFile - 1 > 0) {
    if (board[rank - 1][numFile - 1].isOccupied == true) {
      if (board[rank - 1][numFile - 1].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank - 1;
        temp.destination_file = ('a' + numFile - 1 - 1);
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
      }
      
    } else {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank - 1;
      temp.destination_file = ('a' + numFile - 1 - 1);
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  }
  
  //-1 +1
  if (rank - 1 > 0 && numFile + 1 <= 8) {
    if (board[rank - 1][numFile + 1].isOccupied == true) {
      if (board[rank - 1][numFile + 1].owner == oppColor) {
        temp.piece_moved = numPiece;
        temp.destination_rank = rank - 1;
        temp.destination_file = ('a' + numFile - 1 + 1);
        
        if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
          moves_Got.push_back(temp);
        }
        
      }
      
    } else {
      temp.piece_moved = numPiece;
      temp.destination_rank = rank - 1;
      temp.destination_file = ('a' + numFile - 1 + 1);
      
      if (valid_move(myPieces, oppPieces, temp, board, myColor, oppColor, rankDir)) {
        moves_Got.push_back(temp);
      }
      
    }
    
  }
  
  return moves_Got;
} //King Movement Function.

//MiniMax function.
ai_move AI::miniMax(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, int limit, std::vector< std::vector<tile> >  board) {
  //Variable Declaration:
  int maxMove;
  int maxValue;
  int minValue;
  int numfile;
  ai_move toMove;
  std::vector<ai_move> allMoves;
  ai_piece tempPiece;
  std::vector<int> myCaptured;
  std::vector<int> oppCaptured;
  std::vector< std::vector<tile> > tempBoard;
  rep_move tempMove;
  bool repeat;
  
  //Initialize variables.
  myCaptured.clear();
  oppCaptured.clear();
  maxValue = -100;
  repeat = false;
  
  //Get all possible moves at this node.
  allMoves = action(myPieces, oppPieces, board, player -> color, player -> opponent -> color, player -> rank_direction);
  
  //Call the minVal function on each move.
  for (int i = 0; i < allMoves.size(); i++) {
    
    //
    tempMove.piece_moved = allMoves[i].piece_moved;
    tempMove.origin_file = myPieces[allMoves[i].piece_moved].file;
    tempMove.origin_rank = myPieces[allMoves[i].piece_moved].rank;
    tempMove.destination_file = allMoves[i].destination_file;
    tempMove.destination_rank = allMoves[i].destination_rank;
    repeat = false;
    
    for (int j = 0; j < previous_moves.size(); j++) {
      if ((tempMove.piece_moved == previous_moves[j].piece_moved) && (tempMove.origin_file == myPieces[allMoves[i].piece_moved].file) && (tempMove.origin_rank == myPieces[allMoves[i].piece_moved].rank)
          && (tempMove.destination_file == allMoves[i].destination_file) && (tempMove.destination_rank == allMoves[i].destination_rank) ) {
        repeat = true;
      }
    }
    
    
    //////////////////////////////////////////////
    //Create a temp board with the move applied.//
    //////////////////////////////////////////////
    
    //Create a temp board.
    tempBoard = board;
    
    //Get file number for where the piece originates.
    if (myPieces[allMoves[i].piece_moved].file == "a") {
      numfile = 1;
    } else if (myPieces[allMoves[i].piece_moved].file == "b") {
      numfile = 2;
    } else if (myPieces[allMoves[i].piece_moved].file == "c") {
      numfile = 3;
    } else if (myPieces[allMoves[i].piece_moved].file == "d") {
      numfile = 4;
    } else if (myPieces[allMoves[i].piece_moved].file == "e") {
      numfile = 5;
    } else if (myPieces[allMoves[i].piece_moved].file == "f") {
      numfile = 6;
    } else if (myPieces[allMoves[i].piece_moved].file == "g") {
      numfile = 7;
    } else if (myPieces[allMoves[i].piece_moved].file == "h") {
      numfile = 8;
    }
    
    //Erase piece from previous location on the board.
    tempBoard[myPieces[allMoves[i].piece_moved].rank][numfile].isOccupied = false;
    tempBoard[myPieces[allMoves[i].piece_moved].rank][numfile].owner = "";
    tempBoard[myPieces[allMoves[i].piece_moved].rank][numfile].piece = 0;
    
    //Get file number for where the piece would end up.
    if (allMoves[i].destination_file == "a") {
      numfile = 1;
    } else if (allMoves[i].destination_file == "b") {
      numfile = 2;
    } else if (allMoves[i].destination_file == "c") {
      numfile = 3;
    } else if (allMoves[i].destination_file == "d") {
      numfile = 4;
    } else if (allMoves[i].destination_file == "e") {
      numfile = 5;
    } else if (allMoves[i].destination_file == "f") {
      numfile = 6;
    } else if (allMoves[i].destination_file == "g") {
      numfile = 7;
    } else if (allMoves[i].destination_file == "h") {
      numfile = 8;
    }
    
    //Update captured flag on piece.
    if (tempBoard[allMoves[i].destination_rank][numfile].isOccupied == true) {
      oppPieces[tempBoard[allMoves[i].destination_rank][numfile].piece].captured = true;
    }
    
    //Place piece in new location on board.
    tempBoard[allMoves[i].destination_rank][numfile].isOccupied = true;
    tempBoard[allMoves[i].destination_rank][numfile].owner = player -> color;
    tempBoard[allMoves[i].destination_rank][numfile].piece = allMoves[i].piece_moved;
    
    ///////////////////////////////////////////////////
    //END: Create a temp board with the move applied.//
    ///////////////////////////////////////////////////
    
    //MinVal function call.
    minValue = minVal(myPieces, oppPieces, (limit-1), tempBoard, myCaptured, oppCaptured);
    
    //Penalize the move 20 points if it is a repeat one of the last 3 moves.
    if (repeat) {
      minValue = minValue - 20;
    }
    if (minValue > maxValue) {
      maxMove = i;
      maxValue = minValue;
    }
    
  }
  
  //Once the best move is found, return that move.
  toMove.piece_moved = allMoves[maxMove].piece_moved;
  toMove.destination_rank = allMoves[maxMove].destination_rank;
  toMove.destination_file = allMoves[maxMove].destination_file;
  
  return toMove;
} //MiniMax Function.

//Min value function for minimax.
int AI::minVal(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, int limit, std::vector< std::vector<tile> >  board, std::vector<int> myCaptured, std::vector<int> oppCaptured) {
  //Variable Declaration:
  int value;
  int maxValue;
  int minValue;
  int numfile;
  std::vector<ai_move> allMoves;
  ai_piece tempPiece;
  std::vector< std::vector<tile> > tempBoard;
  
  //Initialize Variables:
  minValue = 100;
  
  //If depth limit reached, return the value for the pieces on the board.
  if (limit <= 0) {
    value = getValue(myPieces, oppPieces, board);
    
    return value;
  }
  
  //Get all possible moves at this node.
  allMoves = action(oppPieces, myPieces, board, player -> opponent -> color, player -> color, player -> opponent -> rank_direction);
  
  //Call the maxVal function on each move.
  for (int i = 0; i < allMoves.size(); i++) {
    
    //////////////////////////////////////////////
    //Create a temp board with the move applied.//
    //////////////////////////////////////////////
    
    //Create a temp board.
    tempBoard = board;
    
    //Get file number for where the piece originates.
    if (oppPieces[allMoves[i].piece_moved].file == "a") {
      numfile = 1;
    } else if (oppPieces[allMoves[i].piece_moved].file == "b") {
      numfile = 2;
    } else if (oppPieces[allMoves[i].piece_moved].file == "c") {
      numfile = 3;
    } else if (oppPieces[allMoves[i].piece_moved].file == "d") {
      numfile = 4;
    } else if (oppPieces[allMoves[i].piece_moved].file == "e") {
      numfile = 5;
    } else if (oppPieces[allMoves[i].piece_moved].file == "f") {
      numfile = 6;
    } else if (oppPieces[allMoves[i].piece_moved].file == "g") {
      numfile = 7;
    } else if (oppPieces[allMoves[i].piece_moved].file == "h") {
      numfile = 8;
    }
    
    //Erase piece from previous location on the board.
    tempBoard[oppPieces[allMoves[i].piece_moved].rank][numfile].isOccupied = false;
    tempBoard[oppPieces[allMoves[i].piece_moved].rank][numfile].owner = "";
    tempBoard[oppPieces[allMoves[i].piece_moved].rank][numfile].piece = 0;
    
    //Get file number for where the piece would end up.
    if (allMoves[i].destination_file == "a") {
      numfile = 1;
    } else if (allMoves[i].destination_file == "b") {
      numfile = 2;
    } else if (allMoves[i].destination_file == "c") {
      numfile = 3;
    } else if (allMoves[i].destination_file == "d") {
      numfile = 4;
    } else if (allMoves[i].destination_file == "e") {
      numfile = 5;
    } else if (allMoves[i].destination_file == "f") {
      numfile = 6;
    } else if (allMoves[i].destination_file == "g") {
      numfile = 7;
    } else if (allMoves[i].destination_file == "h") {
      numfile = 8;
    }
  
    //Update captured flag on piece.
    if (tempBoard[allMoves[i].destination_rank][numfile].isOccupied == true) {
      myPieces[tempBoard[allMoves[i].destination_rank][numfile].piece].captured = true;
    }
    
    //Place piece in new location on board.
    tempBoard[allMoves[i].destination_rank][numfile].isOccupied = true;
    tempBoard[allMoves[i].destination_rank][numfile].owner = player -> opponent -> color;
    tempBoard[allMoves[i].destination_rank][numfile].piece = allMoves[i].piece_moved;
    
    ///////////////////////////////////////////////////
    //END: Create a temp board with the move applied.//
    ///////////////////////////////////////////////////
    
    //MaxVal function call.
    maxValue = maxVal(myPieces, oppPieces, (limit-1), tempBoard, myCaptured, oppCaptured);
    if (maxValue < minValue) {
      
      minValue = maxValue;
    }
    
  }
  
  //Once the lowest value has been found, return that value.
  return minValue;
} //MinVal Function.

//Max value function for minimax.
int AI::maxVal(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, int limit, std::vector< std::vector<tile> >  board, std::vector<int> myCaptured, std::vector<int> oppCaptured) {
  //Variable Declaration:
  int value;
  int maxValue;
  int minValue;
  int numfile;
  std::vector<ai_move> allMoves;
  ai_piece tempPiece;
  std::vector< std::vector<tile> > tempBoard;
  
  //Initialize Variables:
  maxValue = -100;
  
  //If depth limit reached, return the value for the pieces on the board.
  if (limit <= 0) {
    value = getValue(myPieces, oppPieces, board);
    
    return value;
  }
  
  //Get all possible moves at this node.
  allMoves = action(myPieces, oppPieces, board, player -> color, player -> opponent -> color, player -> rank_direction);
  
  //Call the maxVal function on each move.
  for (int i = 0; i < allMoves.size(); i++) {
    
    //////////////////////////////////////////////
    //Create a temp board with the move applied.//
    //////////////////////////////////////////////
    
    //Create a temp board.
    tempBoard = board;
    
    //Get file number for where the piece originates.
    if (myPieces[allMoves[i].piece_moved].file == "a") {
      numfile = 1;
    } else if (myPieces[allMoves[i].piece_moved].file == "b") {
      numfile = 2;
    } else if (myPieces[allMoves[i].piece_moved].file == "c") {
      numfile = 3;
    } else if (myPieces[allMoves[i].piece_moved].file == "d") {
      numfile = 4;
    } else if (myPieces[allMoves[i].piece_moved].file == "e") {
      numfile = 5;
    } else if (myPieces[allMoves[i].piece_moved].file == "f") {
      numfile = 6;
    } else if (myPieces[allMoves[i].piece_moved].file == "g") {
      numfile = 7;
    } else if (myPieces[allMoves[i].piece_moved].file == "h") {
      numfile = 8;
    }
    
    //Erase piece from previous location on the board.
    tempBoard[myPieces[allMoves[i].piece_moved].rank][numfile].isOccupied = false;
    tempBoard[myPieces[allMoves[i].piece_moved].rank][numfile].owner = "";
    tempBoard[myPieces[allMoves[i].piece_moved].rank][numfile].piece = 0;
    
    //Get file number for where the piece would end up.
    if (allMoves[i].destination_file == "a") {
      numfile = 1;
    } else if (allMoves[i].destination_file == "b") {
      numfile = 2;
    } else if (allMoves[i].destination_file == "c") {
      numfile = 3;
    } else if (allMoves[i].destination_file == "d") {
      numfile = 4;
    } else if (allMoves[i].destination_file == "e") {
      numfile = 5;
    } else if (allMoves[i].destination_file == "f") {
      numfile = 6;
    } else if (allMoves[i].destination_file == "g") {
      numfile = 7;
    } else if (allMoves[i].destination_file == "h") {
      numfile = 8;
    }
  
    //Update captured flag on piece.
    if (tempBoard[allMoves[i].destination_rank][numfile].isOccupied == true) {
      oppPieces[tempBoard[allMoves[i].destination_rank][numfile].piece].captured = true;
    }
    
    //Place piece in new location on board.
    tempBoard[allMoves[i].destination_rank][numfile].isOccupied = true;
    tempBoard[allMoves[i].destination_rank][numfile].owner = player -> color;
    tempBoard[allMoves[i].destination_rank][numfile].piece = allMoves[i].piece_moved;
    
    ///////////////////////////////////////////////////
    //END: Create a temp board with the move applied.//
    ///////////////////////////////////////////////////
    
    //MinVal function call.
    minValue = minVal(myPieces, oppPieces, (limit-1), tempBoard, myCaptured, oppCaptured);
    if (minValue > maxValue) {
      
      maxValue = minValue;
    }
    
  }
  
  //Once the lowest value has been found, return that value.
  return maxValue;
} //MaxVal Function.

//Returns the value of the given board.
int AI::getValue(std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, std::vector< std::vector<tile> > board) {
  //Variable Declaration:
  int value;
  
  value = 0;
  //Iterate through the board, and calculate the value of the pieces on it.
  for (int i = 1; i <= 8; i++) {
    for (int j = 1; j <= 8; j++) {
      //If tile is occupied.
      if (board[i][j].isOccupied == true) {
        //If piece is owned by the player.
        if (board[i][j].owner == player->color) {
          if (player->pieces[board[i][j].piece]->type == "Pawn") {
            value = value + 1;
            
          } else if (player->pieces[board[i][j].piece]->type == "Bishop") {
            value = value + 3;
            
          } else if (player->pieces[board[i][j].piece]->type == "Rook") {
            value = value + 5;
            
          } else if (player->pieces[board[i][j].piece]->type == "Knight") {
            value = value + 3;
            
          } else if (player->pieces[board[i][j].piece]->type == "Queen") {
            value = value + 9;
            
          }
          
        } //If player color
        
        //If piece is owned by the opponent.
        if (board[i][j].owner == player -> opponent -> color) {
          if (player->pieces[board[i][j].piece]->type == "Pawn") {
            value = value - 1;
            
          } else if (player->pieces[board[i][j].piece]->type == "Bishop") {
            value = value - 3;
            
          } else if (player->pieces[board[i][j].piece]->type == "Rook") {
            value = value - 5;
            
          } else if (player->pieces[board[i][j].piece]->type == "Knight") {
            value = value - 3;
            
          } else if (player->pieces[board[i][j].piece]->type == "Queen") {
            value = value - 9;
            
          }
          
        } //If opponent color.
        
      } //If tile is occupied.
      
    }
    
  } //Board iteration.
  
  //If player is in check, subtract 7 from the score.  If opponent is in check, add 3 to the score.
  if (in_check(player -> color, player -> opponent -> color, myPieces, oppPieces, board, player -> rank_direction)) {
    value = value - 7;
  }
  if (in_check(player -> opponent -> color, player -> color, oppPieces, myPieces, board, player -> rank_direction)) {
    value = value + 3;
  }
  
  return value;
} //Get Value Function.

//Returns true if the given player is in check.
bool AI::in_check(std::string myColor, std::string oppColor, std::vector<ai_piece> myPieces, std::vector<ai_piece> oppPieces, std::vector< std::vector<tile> > board, int rankDir) {
  //Variable Declaration:
  int king;
  int rank;
  int numfile;
  int destfile;
  int diagMoves;
  bool valid;
  bool done;
  
  valid = true;
  
  ///////////////////////////
  //Get King rank and file.//
  ///////////////////////////
  
  //Find King piece.
  for (int l = 0; l < myPieces.size(); l++) {
    if (myPieces[l].type == "King") {
      king = l;
    }
  }
  
  //Get rank and file.
  rank = myPieces[king].rank;
  
  if (myPieces[king].file == "a") {
    numfile = 1;
  } else if (myPieces[king].file == "b") {
    numfile = 2;
  } else if (myPieces[king].file == "c") {
    numfile = 3;
  } else if (myPieces[king].file == "d") {
    numfile = 4;
  } else if (myPieces[king].file == "e") {
    numfile = 5;
  } else if (myPieces[king].file == "f") {
    numfile = 6;
  } else if (myPieces[king].file == "g") {
    numfile = 7;
  } else if (myPieces[king].file == "h") {
    numfile = 8;
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
      if (board[i][numfile].owner == oppColor) {
        if (i == (rank+1) && oppPieces[board[i][numfile].piece].type == "King") {
          valid = false;
        }
        if (oppPieces[board[i][numfile].piece].type == "Rook" ||
            oppPieces[board[i][numfile].piece].type == "Queen") {
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
      if (board[i][numfile].owner == oppColor) {
        if (i == (rank-1) && oppPieces[board[i][numfile].piece].type == "King") {
          valid = false;
        }
        if (oppPieces[board[i][numfile].piece].type == "Rook" ||
            oppPieces[board[i][numfile].piece].type == "Queen") {
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
      if (board[rank][i].owner == oppColor) {
        if (i == (numfile+1) && oppPieces[board[rank][i].piece].type == "King") {
          valid = false;
        }
        if (oppPieces[board[rank][i].piece].type == "Rook" ||
            oppPieces[board[rank][i].piece].type == "Queen") {
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
      if (board[rank][i].owner == oppColor) {
        if (i == (numfile-1) && oppPieces[board[rank][i].piece].type == "King") {
          valid = false;
        }
        if (oppPieces[board[rank][i].piece].type == "Rook" ||
            oppPieces[board[rank][i].piece].type == "Queen") {
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
  for (int i = 1; i <= diagMoves; i++) {
    if (board[rank + i][numfile + i].isOccupied && done == false) {
      if (board[rank + i][numfile + i].owner == oppColor) {
        //If a pawn is in striking distance.
        if (i == 1 && (rankDir * -1) < 0 &&
            oppPieces[board[rank + i][numfile + i].piece].type == "Pawn") {
          valid = false;
        }
        //If a king can attack.
        if (i == 1 && oppPieces[board[rank + i][numfile + i].piece].type == "King") {
          valid = false;
        }
        //If a bishop or queen can attack.
        if (oppPieces[board[rank + i][numfile + i].piece].type == "Bishop" ||
            oppPieces[board[rank + i][numfile + i].piece].type == "Queen") {
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
  for (int i = 1; i <= diagMoves; i++) {
    if (board[rank + i][numfile - i].isOccupied && done == false) {
      if (board[rank + i][numfile - i].owner == oppColor) {
        //If a pawn is in striking distance.
        if (i == 1 && (rankDir * -1) < 0 &&
            oppPieces[board[rank + i][numfile - i].piece].type == "Pawn") {
          valid = false;
        }
        //If a king can attack.
        if (i == 1 && oppPieces[board[rank + i][numfile - i].piece].type == "King") {
          valid = false;
        }
        //If a bishop or queen can attack.
        if (oppPieces[board[rank + i][numfile - i].piece].type == "Bishop" ||
            oppPieces[board[rank + i][numfile - i].piece].type == "Queen") {
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
  for (int i = 1; i <= diagMoves; i++) {
    if (board[rank - i][numfile - i].isOccupied && done == false) {
      if (board[rank - i][numfile - i].owner == oppColor) {
        //If a pawn is in striking distance.
        if (i == 1 && (rankDir * -1) > 0 &&
            oppPieces[board[rank - i][numfile - i].piece].type == "Pawn") {
          valid = false;
        }
        //If a king can attack.
        if (i == 1 && oppPieces[board[rank - i][numfile - i].piece].type == "King") {
          valid = false;
        }
        //If a bishop or queen can attack.
        if (oppPieces[board[rank - i][numfile - i].piece].type == "Bishop" ||
            oppPieces[board[rank - i][numfile - i].piece].type == "Queen") {
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
  for (int i = 1; i <= diagMoves; i++) {
    if (board[rank - i][numfile + i].isOccupied && done == false) {
      if (board[rank - i][numfile + i].owner == oppColor) {
        //If a pawn is in striking distance.
        if (i == 1 && (rankDir * -1) > 0 &&
            oppPieces[board[rank - i][numfile + i].piece].type == "Pawn") {
          valid = false;
        }
        //If a king can attack.
        if (i == 1 && oppPieces[board[rank - i][numfile + i].piece].type == "King") {
          valid = false;
        }
        //If a bishop or queen can attack.
        if (oppPieces[board[rank - i][numfile + i].piece].type == "Bishop" ||
            oppPieces[board[rank - i][numfile + i].piece].type == "Queen") {
          valid = false;
        }
        
      }
      
      done = true;
    }
    
  }
  
  /////////////////////
  //Check for Knights//
  /////////////////////
  
  //Check for possible knights placing the king in check.
  
  //+1 +2
  if (rank + 1 <= 8 && numfile + 2 <= 8) {
    if (board[rank + 1][numfile + 2].isOccupied) {
      if (board[rank + 1][numfile + 2].owner == oppColor) {
        if (oppPieces[board[rank + 1][numfile + 2].piece].type == "Knight") {
          valid = false;
        }
        
      }
      
    }
    
  }
  
  //+2 +1
  if (rank + 2 <= 8 && numfile + 1 <= 8) {
    if (board[rank + 2][numfile + 1].isOccupied) {
      if (board[rank + 2][numfile + 1].owner == oppColor) {
        if (oppPieces[board[rank + 2][numfile + 1].piece].type == "Knight") {
          valid = false;
        }
        
      }
      
    }
    
  }
  
  //+1 -2
  if (rank + 1 <= 8 && numfile - 2 > 0) {
    if (board[rank + 1][numfile - 2].isOccupied) {
      if (board[rank + 1][numfile - 2].owner == oppColor) {
        if (oppPieces[board[rank + 1][numfile - 2].piece].type == "Knight") {
          valid = false;
        }
        
      }
      
    }
    
  }
  
  //+2 -1
  if (rank + 2 <= 8 && numfile - 1 > 0) {
    if (board[rank + 2][numfile - 1].isOccupied) {
      if (board[rank + 2][numfile - 1].owner == oppColor) {
        if (oppPieces[board[rank + 2][numfile - 1].piece].type == "Knight") {
          valid = false;
        }
        
      }
      
    }
    
  }
  
  //-1 -2
  if (rank - 1 > 0 && numfile - 2 > 0) {
    if (board[rank - 1][numfile - 2].isOccupied) {
      if (board[rank - 1][numfile - 2].owner == oppColor) {
        if (oppPieces[board[rank - 1][numfile - 2].piece].type == "Knight") {
          valid = false;
        }
        
      }
      
    }
    
  }
  
  //-2 -1
  if (rank - 2 > 0 && numfile - 1 > 0) {
    if (board[rank - 2][numfile - 1].isOccupied) {
      if (board[rank - 2][numfile - 1].owner == oppColor) {
        if (oppPieces[board[rank - 2][numfile - 1].piece].type == "Knight") {
          valid = false;
        }
        
      }
      
    }
    
  }
  
  //-1 +2
  if (rank - 1 > 0 && numfile + 2 <= 8) {
    if (board[rank - 1][numfile + 2].isOccupied) {
      if (board[rank - 1][numfile + 2].owner == oppColor) {
        if (oppPieces[board[rank - 1][numfile + 2].piece].type == "Knight") {
          valid = false;
        }
        
      }
      
    }
    
  }
  
  //-2 +1
  if (rank - 2 > 0 && numfile + 1 <= 8) {
    if (board[rank - 2][numfile + 1].isOccupied) {
      if (board[rank - 2][numfile + 1].owner == oppColor) {
        if (oppPieces[board[rank - 2][numfile + 1].piece].type == "Knight") {
          valid = false;
        }
        
      }
      
    }
    
  }
  
  //Return value of valid
  return valid;
}

} // chess

} // cpp_client
