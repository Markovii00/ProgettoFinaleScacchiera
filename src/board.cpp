/**
 *@file board.cpp
 *@author  Francesco Colla - 2007968 (francesco.colla.2@studenti.unipd.it)
 *@brief
 *
 **/

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cctype>
#include "include/board.h"

board::board()
{
    possibleEnPassant = false;
    freezeCordinateEnPassant.first = 0;
    freezeCordinateEnPassant.second = 0;
    moveCounter = 0;

    for (unsigned short cCol = 0; cCol < 8; ++cCol)
    {
        for (unsigned short cRow = 0; cRow < 8; ++cRow)
        {
            chessboard[cRow][cCol] = nullptr;
        }
    }

    // allocating all white pawns and pieces
    for (unsigned short cCol = 0; cCol < 8; ++cCol)
    {
        chessboard[6][cCol] = new pawn('p');
        whiteSet.push_back(std::make_pair(6,cCol)); //adding all pawn
        whiteSet.push_back(std::make_pair(7,cCol)); //adding all the other pieces
    }
    chessboard[7][0] = new rook('t');
    chessboard[7][1] = new knight('c');
    chessboard[7][2] = new bishop('a');
    chessboard[7][3] = new queen('d');
    chessboard[7][4] = new king('r');
    chessboard[7][5] = new bishop('a');
    chessboard[7][6] = new knight('c');
    chessboard[7][7] = new rook('t');

    // allocating all black pawns and pieces
    for (unsigned short cCol = 0; cCol < 8; cCol++)
    {
        chessboard[1][cCol] = new pawn('P');
        blackSet.push_back(std::make_pair(6,cCol)); //adding all pawn
        blackSet.push_back(std::make_pair(7,cCol)); //adding all the other pieces
    }
    chessboard[0][0] = new rook('T');
    chessboard[0][1] = new knight('C');
    chessboard[0][2] = new bishop('A');
    chessboard[0][3] = new queen('D');
    chessboard[0][4] = new king('R');
    chessboard[0][5] = new bishop('A');
    chessboard[0][6] = new knight('C');
    chessboard[0][7] = new rook('T');
}

board::~board()
{
    for (unsigned short cCol = 0; cCol < 8; ++cCol)
    {
        for (unsigned short cRow = 0; cRow < 8; ++cRow)
        {
            // This if avoid the segfault in case chessman is null
            if (chessboard[cRow][cCol] not_eq nullptr)
            {
                delete (chessboard[cRow][cCol]);
                chessboard[cRow][cCol] = nullptr;
            }
        }
    }
}

bool board::kingInCheck(std::pair<unsigned short, unsigned short> king_coordinates, bool requestColor)
{
    //king is black, look for white pieces that can eat it
    if(requestColor == true) {
        for(unsigned int i = 0; i < blackSet.size(); i++) {
            if(chessboard[blackSet[i].first][blackSet[i].second]->isLegalMove(blackSet[i].first, blackSet[i].second, king_coordinates.first, king_coordinates.second) && clearPath(blackSet[i].first, blackSet[i].second, king_coordinates.first, king_coordinates.second, *(chessboard[blackSet[i].first][blackSet[i].second])->getChar())
                return true;
        }
    }
    else {
        for(unsigned int i = 0; i < whiteSet.size(); i++) {
            if(chessboard[whiteSet[i].first][whiteSet[i].second]->isLegalMove(whiteSet[i].first, whiteSet[i].second, king_coordinates.first, king_coordinates.second) && clearPath(whiteSet[i].first, whiteSet[i].second, king_coordinates.first, king_coordinates.second, *(chessboard[whiteSet[i].first][whiteSet[i].second])->getChar()))
                return true;
        }
    }
    return false;
}

bool board::acceptableMove(unsigned short& fromRow, unsigned short& fromCol, unsigned short& toRow, unsigned short& toCol, char& fromPieceId) const
{
    if (fromPieceId == 0)
        return false;
    bool fromColor = isBlack(fromPieceId); //true is black

    char toId = getName(toRow, toCol);
    bool toColor = isBlack(toId); //true is black
    if (toRow < 8 && toCol < 8)
    {
        if (toId == 0 || fromColor != toColor)
        {
            return true;
        }
    }
    return false;
}

template<typename Type>
bool is(const chessman &data) {
    if( &data == NULL ) return false;

    if (typeid(Type) == typeid(rook)) return tolower(data.getChar()) == 't';
    if (typeid(Type) == typeid(knight)) return tolower(data.getChar()) == 'c';
    if (typeid(Type) == typeid(bishop)) return tolower(data.getChar()) == 'a';
    if (typeid(Type) == typeid(king)) return tolower(data.getChar()) == 'r';
    if (typeid(Type) == typeid(queen)) return tolower(data.getChar()) == 'd';
    if (typeid(Type) == typeid(pawn)) return tolower(data.getChar()) == 'p';

    return false;
}

template<typename Type>
std::pair<unsigned short, unsigned short> board::search(bool& requestColor) const {
    if(requestColor == true) {
        for(std::pair<unsigned short, unsigned short> i : blackSet) {
            if(is<Type>(*(chessboard[i.first][i.second])))
                return make_pair(i.first, i.second)
        }
    }
    else {
        for(std::pair<unsigned short, unsigned short> i : whiteSet) {
            if(is<Type>(*(chessboard[i.first][i.second])))
                return make_pair(i.first, i.second)
        }
    }
    return make_pair(-1, -1);
}

template<typename Type>
bool board::find(bool& requestColor) const {
    if(requestColor == true) {
        for(std::pair<unsigned short, unsigned short> i : blackSet) {
            if(is<Type>(*(chessboard[i.first][i.second])))
                return true;
        }
    }
    else {
        for(std::pair<unsigned short, unsigned short> i : whiteSet) {
            if(is<Type>(*(chessboard[i.first][i.second])))
                return true;
        }
    }
    return false;
}

std::pair<bool,bool> board::move(unsigned short& fromRow, unsigned short& fromCol, unsigned short& toRow, unsigned short& toCol, bool& pieceToMoveColor) // controllare complessità
{ 
    if(checkTie()) {
        return std::make_pair(false, false); //is over baby
    }

    char fromPieceId = chessboard[fromRow][fromCol]->getChar();
    bool fromPieceColor = isBlack(fromPieceId);

    // check if the coordinates are within the board limits and whether the destination tile is empty or there is an opponent chessman
    if(!acceptableMove(fromRow, fromCol, toRow, toCol, fromPieceId))
    {
        std::cout << "The selected move is out of bounds, select a new one\n";
        return std::make_pair(false,true);
    }
    if(fromPieceColor != pieceToMoveColor)
    {
        std::cout << "You can't move an opponent's piece, select a new move\n";
        return std::make_pair(false,true);
    }
    if(!chessboard[fromRow][fromCol]->isLegalMove(fromRow, fromCol, toRow, toCol))
    {
        std::cout << "The selected move is illegal, select a new one\n";
        return std::make_pair(false,true);
    }
    if(!clearPath(fromRow, fromCol, toRow, toCol, fromPieceId))
    {
        std::cout << "There is a piece in the way of your move, select a new one\n";
        return std::make_pair(false,true);
    }

    std::pair<short, short> kingCoords = search<king>(fromPieceColor);

    // Control on the king
    if (kingInCheck(kingCoords, fromPieceColor))
    {
        std::vector<std::pair<unsigned short, unsigned short>> PMoves;
        PMoves = KingPossibleMoves(kingCoords, fromPieceColor); 
        for (std::pair<unsigned short, unsigned short> x : PMoves)
        {
            if (!kingInCheck(x.first, x.second, fromPieceColor))
            {
                std::cout << "Your king is in check, move it";
                return false;
            }
        }
        // all the possible king moves are covered, GG!
        endGame();
    }
    
    //PAWN HANDLING
    bool isChangedLastTurn = possibleEnPassant;
    if (isPawn(fromPieceId))
    {
        // first move && 2 tile row movement
        if (((pawn *)chessboard[fromRow][fromCol])->PartialEnPassantConditions(fromRow, toRow))
        {
            // sure to be next to opponent pawn
            if(fromPieceId == 'p' && ((chessboard[toRow][toCol + 1]->getChar() == 'P') || (chessboard[toRow][toCol - 1]->getChar() == 'P')))
            {
                possibleEnPassant = true;
                freezeCordinateEnPassant.first = toRow;
                freezeCordinateEnPassant.second = toCol;
                isChangedLastTurn = true;
            }
            else if (fromPieceId == 'P' && ((chessboard[toRow][toCol + 1]->getChar() == 'p') || (chessboard[toRow][toCol - 1]->getChar() == 'p')))
            {
                possibleEnPassant = true;
                freezeCordinateEnPassant.first = toRow;
                freezeCordinateEnPassant.second = toCol;
                isChangedLastTurn = true;
            }
        }
        return movePawn(fromRow, fromCol, toRow, toCol, fromPieceColor);
    }
    
    //Castling Management
    if(isKing(fromPieceId) && castling(fromRow, fromCol, toRow, toCol, fromPieceColor)) 
        return true;
    
    //General Move
    executeMove(fromRow, fromCol, toRow, toCol);
    
    //Keep those variables updated
    if(isChangedLastTurn != false) 
    {
        possibleEnPassant = false;
        freezeCordinateEnPassant.first = 0;
        freezeCordinateEnPassant.second = 0;
    }
    return true;
}

std::vector<std::pair<unsigned short, unsigned short>> board::KingPossibleMoves(std::pair<unsigned short, unsigned short> kCords, char fromPieceId) const
{
    std::vector<std::pair<unsigned short, unsigned short>> PMoves;
    // i rows j colums
    for (short i = -1; i < 2; i++)
    {
        for (short j = -1; j < 2; j++)
        {
            if (i == 0 && j == 0)
                continue;
            unsigned short possRow = kCords.first + i;
            unsigned short possCol = kCords.second + j;
            if (acceptableMove(kCords.first, kCords.second, possRow, possCol, fromPieceId))
            {
                std::pair<unsigned short, unsigned short> amogus;
                amogus.first = possRow;
                amogus.second = possCol;
                PMoves.push_back(amogus);
            }
        }
    }
    return PMoves;
}

void board::executeMove(short fromRow, short fromCol, short toRow, short toCol)
{
    if (getName(toRow, toCol) != 0)
    {
        delete chessboard[toRow][toCol];
    }
    
    chessboard[toRow][toCol] = chessboard[fromRow][fromCol];
    chessboard[fromRow][fromCol] = nullptr;
    char toId = getName(toRow, toCol);
    if(isRook(toId) && !((rook*)chessboard[toRow][toCol])->hasMoved()) 
        ((rook*)chessboard[toRow][toCol])->setMoved();
    else if(isKing(toId) && !((king*)chessboard[toRow][toCol])->hasMoved()) 
        ((king*)chessboard[toRow][toCol])->setMoved();
    else if(isPawn(toId) && !((pawn*)chessboard[toRow][toCol])->hasMoved()) 
        ((pawn*)chessboard[toRow][toCol])->setMoved();
    //un if all'inizio per vedere hasMoved, creare un nuovo oggetto e ri-settarlo come all'inizio in modo da non fare merda
}

bool board::movePawn(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol, bool fromPieceColor)
{
    // check the validity for diagonal move (implemented here for handling the En passant special move)
    char toPieceId = chessboard[toRow][toCol]->getChar();
    bool toPieceColor = isBlack(toPieceId);
    if (abs(toRow - fromRow) == 1 && abs(toCol - fromCol) == 1)
    {
        if (toPieceColor != fromPieceColor && toPieceId != 0) // chessman to eat diagonally
        {
            executeMove(fromRow, fromCol, toRow, toCol);
            return true;
        }
        else if (CheckOnEnPassant(fromRow, fromCol, toRow, toCol)) // if not chessman to eat diagonally, might be an en passant
        {
            EnPassant(fromRow, fromCol, toRow, toCol);
            return true;
        }
        else // if not none of the above, ILLEGAL MOVE
            return false;
    }
    else //all other legit moves
    {
        if (toRow == 0 || toRow == 7)
        {
            executeMove(fromRow, fromCol, toRow, toCol);
            promotion(toRow, toCol);
            return true;
        }  
        else
        {
            if(toPieceId != 0) //check if one/two tile/s ahead there aren't chessmans 
                return false;
            executeMove(fromRow, fromCol, toRow, toCol);
            return true;
        }     
    }
    return false;
}

bool board::CheckOnEnPassant(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol) { return possibleEnPassant && freezeCordinateEnPassant.first == fromRow && ((freezeCordinateEnPassant.second == fromCol + 1) || (freezeCordinateEnPassant.second == fromCol - 1)) && abs(freezeCordinateEnPassant.first - toRow) == 1 && freezeCordinateEnPassant.second == toCol; }

void board::EnPassant(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol)
{
    chessboard[toRow][toCol] = chessboard[fromRow][fromCol];
    chessboard[fromRow][fromCol] = nullptr;
    delete chessboard[fromRow][toCol];
    chessboard[fromRow][toCol] = nullptr;
}

char board::getName(short row, short col) const
{
    if (chessboard[row][col] == nullptr)
        return 0;
    return chessboard[row][col]->getChar();
}

bool board::isBlack(char& request) const
{
    if (request >= 'A' && request <= 'Z')
        return true;
    else
        return false;
}
//awoo
bool board::checkTie()
{
    unsigned short blackSetSize = blackSet.size();
    unsigned short whiteSetSize = whiteSet.size();
    bool isBlack = true;
    if(moveCounter > 150){                                               
        std::cout << "75 turns have been done, the game is a tie!"
        return true;
    }
    else if(whiteSetSize == 1 && blackSetSize == 1)                     
    {
        if(find<king>(isBlack) && find<king>(!isBlack))
        {
            std::cout << "No more moves are possible, the game is tie!";
            return true;
        }
    }
    else if(whiteSetSize == 2 && blackSetSize == 1)                      
    {
        if((find<king>(!isBlack) && find<bishop>(!isBlack)) && find<king>(isBlack))
        {
            std::cout << "No more moves are possible, the game is tie!";
            return true;
        }
    }
    else if(whiteSetSize == 1 && blackSetSize == 2)
    {
        if((find<king>(isBlack) && find<bishop>(isBlack)) && find<king>(!isBlack))
        {
            std::cout << "No more moves are possible, the game is tie!";
            return true;
        }
    }
    else if(whiteSetSize == 2 && blackSetSize == 1)                       
    {
        if((find<king>(!isBlack) && find<knight>(!isBlack)) && find<king>(isBlack))
        {
            std::cout << "No more moves are possible, the game is tie!";
            return true;
        }
    }
    else if(whiteSetSize == 1 && blackSetSize == 2)
    {
        if((find<king>(isBlack) && find<knight>(isBlack)) && find<king>(!isBlack))
        {
            std::cout << "No more moves are possible, the game is tie!";
            return true;
        }
    }                                                                     
    return false;
}

void board::promotion(unsigned short pawnRow, unsigned short pawnCol)
{
    char newPiece;
    char id = chessboard[pawnRow][pawnCol]->getChar();
    bool isPawnBlack = isBlack(id);

    std::cout << "La pedina in posizione " << pawnRow + 1 << pawnCol + 1 << " può essere promossa, inserire il tipo di pedina desiderato";
    std::cin >> newPiece;
    while (!isQueen(newPiece) || !isBishop(newPiece) || !isKnight(newPiece) || !isQueen(newPiece) || !isRook(newPiece))
    {
        std::cout << "Pedina richiesta non valida";
        std::cin >> newPiece;
    }
    
    delete chessboard[pawnRow][pawnCol];
    if (isPawnBlack == true)
    {
        if (isQueen(newPiece))
            chessboard[pawnRow][pawnCol] = new queen('D');
        else if (isBishop(newPiece))
            chessboard[pawnRow][pawnCol] = new bishop('A');
        else if (isKnight(newPiece))
            chessboard[pawnRow][pawnCol] = new knight('C');
        else if (isRook(newPiece))
            chessboard[pawnRow][pawnCol] = new rook('T');
    }
    else if (isPawnBlack == false)
    {
        if (isQueen(newPiece))
            chessboard[pawnRow][pawnCol] = new queen('d');
        else if (isBishop(newPiece))
            chessboard[pawnRow][pawnCol] = new bishop('a');
        else if (isKnight(newPiece))
            chessboard[pawnRow][pawnCol] = new knight('c');
        else if (isRook(newPiece))
            chessboard[pawnRow][pawnCol] = new rook('t');
    }
}

bool board::castling(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol, bool kingIsBlack)
{

    if (kingIsBlack)
    {
        if (fromRow == toRow && toCol == fromCol + 2 && getName(7, 7) == 'T')
        {
            if (((rook *)chessboard[7][7])->hasMoved() == false)
            {
                short counter = fromCol + 1;
                while (counter < 7)
                {
                    if (getName(fromRow, counter) != 0)
                        return false;
                    counter++;
                }
                executeMove(fromRow, fromCol, toRow, toCol);
                executeMove(7, 7, 7, 5);
                return true;
            }
        }
        else if (fromRow == toRow && toCol == fromCol - 2 && getName(7, 0) == 'T')
        {
            if (((rook *)chessboard[7][0])->hasMoved() == false)
            {
                short counter = fromCol - 1;
                while (counter > 0)
                {
                    if (getName(fromRow, counter) != 0)
                        return false;
                    counter--;
                }

                executeMove(fromRow, fromCol, toRow, toCol);
                executeMove(7, 0, 7, 3);
                return true;
            }
        }
    }
    else if (!kingIsBlack)
    {
        if (fromRow == toRow && toCol == fromCol + 2 && getName(0, 7) == 't')
        {
            if (((rook *)chessboard[0][7])->hasMoved() != false)
            {
                short counter = fromCol + 1;
                while (counter < 7)
                {
                    if (getName(fromRow, counter) != 0)
                        return false;
                    counter++;
                }
                executeMove(fromRow, fromCol, toRow, toCol);
                executeMove(0, 7, 0, 5);
                return true;
            }
        }
        else if (fromRow == toRow && toCol == fromCol - 2 && getName(0, 0) == 't')
        {
            if (((rook *)chessboard[0][0])->hasMoved() != false)
            {
                short fromTCol = 0;
                short fromTRow = 0;
                short toTRow = 0;
                short toTCol = 3;
                unsigned short counter = fromCol - 1;
                while (counter > 0)
                {
                    if (getName(fromRow, counter) != 0)
                        return false;
                    counter--;
                }

                executeMove(fromRow, fromCol, toRow, toCol);
                executeMove(fromTRow, fromTCol, toTRow, toTCol);
                return true;
            }
        }
    }
    return false;
}

bool board::endGame() {return false;}

void board::printBoard()
{
    for (unsigned short iRow = 0; iRow < 8; ++iRow)
    {
        std::cout << 8 - iRow << "    ";
        for (unsigned short iCol = 0; iCol < 8; ++iCol)
        {
            if (chessboard[iRow][iCol] not_eq nullptr)
                std::cout << " " << chessboard[iRow][iCol]->getChar() << (iCol == 7 ? "": " |");
            else
            {
                std::cout << (iCol == 7 ? "": "   |");
            }
        }
        std::cout << "\n" << (iRow == 7 ? "\n" : "     -------------------------------\n");
    }
    std::cout << "      A   B   C   D   E   F   G   H";
}

std::string board::to_string(bool fixed_allignment = false)
{
    std::string bb;
    for (unsigned short iRow = 0; iRow < 8; ++iRow)
    {
        bb += (fixed_allignment ? "\t\t\t       " : "");
        bb += std::to_string(8 - iRow) + "    ";
        for (unsigned short iCol = 0; iCol < 8; ++iCol)
        {
            if (chessboard[iRow][iCol] not_eq nullptr)
            {   bb += " ";
                bb += ((chessboard[iRow][iCol]->getChar()));
                bb += ((iCol == 7 ? "": " |"));
            }
            else
            {
                bb += (iCol == 7 ? "": "   |");
            }
        }
        bb += ((fixed_allignment ? "\n\t\t\t       " : "\n"));
        bb += (iRow == 7 ? "\n" : "     -------------------------------\n");
    }
    bb += (fixed_allignment ? "\n\t\t\t       " : "\n");
    bb += "      A   B   C   D   E   F   G   H";

    return bb;
}

bool board::isVertical(unsigned short& fromRow, unsigned short& fromCol, unsigned short& toRow, unsigned short& toCol) const { return fromCol == toCol && fromRow != toRow;}

bool board::isHorizontal(unsigned short& fromRow, unsigned short& fromCol, unsigned short& toRow, unsigned short& toCol) const {return fromRow==toRow&&fromCol!=toCol;}

bool board::isDiagonal(unsigned short& fromRow, unsigned short& fromCol, unsigned short& toRow, unsigned short& toCol) const { return abs(toRow - fromRow) == abs(toCol - fromCol); }



bool board::clearPath(unsigned short& fromRow, unsigned short& fromCol, unsigned short& toRow, unsigned short& toCol, char& fromPieceId) const
{
    fromPieceId = tolower(fromPieceId);
    short dR = toRow - fromRow;
    short dC = toCol - fromCol;

    short rMov = fromRow;
    short cMov = fromCol;

    bool vertical = isVertical( fromRow, fromCol, toRow, toCol);
    bool horizontal = isHorizontal( fromRow, fromCol, toRow, toCol);
    bool diagonal = isDiagonal( fromRow, fromCol, toRow, toCol);

    switch (fromPieceId)
    {
        case 'c' : return true;
        case 'a' : 
        {
            //movement Sud-Est
            if(dR > 0 && dC > 0) 
            {
                while (rMov < toRow && cMov < toCol)
                {
                    rMov += 1;
                    cMov += 1;
                    if(chessboard[rMov][cMov] not_eq nullptr) return false;
                }
                return true;
            }
            //movement Nord-Est
            else if(dR < 0 && dC > 0)
            {
                while (rMov > toRow && cMov < toCol)
                {
                    rMov -= 1;
                    cMov += 1;
                    if(chessboard[rMov][cMov] not_eq nullptr) return false;
                }
                return true;
            }
            //movement Nord-West
            else if(dR < 0 && dC < 0)
            {
                while (rMov > toRow && cMov > toCol)
                {
                    rMov -= 1;
                    cMov -= 1;
                    if(chessboard[rMov][cMov] not_eq nullptr) return false;
                }
                return true;
            }
            //movement Sud-West
            else if(dR > 0 && dC < 0)
            {
                while (rMov < toRow && cMov > toCol)
                {
                    rMov += 1;
                    cMov -= 1;
                    if(chessboard[rMov][cMov] not_eq nullptr) return false;
                }
                return true;
            }
        }

        case 'd' :
        {
            if(vertical)
            {
                if(dR > 0) 
                {
                    while (rMov < toRow)
                    {
                        rMov += 1;
                        if(chessboard[rMov][fromCol] not_eq nullptr) return false;
                    }
                    return true;
                }
                else
                {
                    while (rMov > toRow)
                    {
                        rMov -= 1;
                        if(chessboard[rMov][fromCol] not_eq nullptr) return false;
                    }
                    return true;
                }
            }
            else if(horizontal) 
            {
                if(dC > 0) 
                {
                    while (cMov < toCol)
                    {
                        cMov += 1;
                        if(chessboard[fromRow][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }
                else
                {
                    while (cMov > toCol)
                    {
                        cMov -= 1;
                        if(chessboard[fromRow][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }

            }
            else //diagonal movement
            {
                //movement Sud-Est
                if(dR > 0 && dC > 0) 
                {
                    while (rMov < toRow && cMov < toCol)
                    {
                        rMov += 1;
                        cMov += 1;
                        if(chessboard[rMov][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }
                //movement Nord-Est
                else if(dR < 0 && dC > 0)
                {
                    while (rMov > toRow && cMov < toCol)
                    {
                        rMov -= 1;
                        cMov += 1;
                        if(chessboard[rMov][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }
                //movement Nord-West
                else if(dR < 0 && dC < 0)
                {
                    while (rMov > toRow && cMov > toCol)
                    {
                        rMov -= 1;
                        cMov -= 1;
                        if(chessboard[rMov][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }
                //movement Sud-West
                else if(dR > 0 && dC < 0)
                {
                    while (rMov < toRow && cMov > toCol)
                    {
                        rMov += 1;
                        cMov -= 1;
                        if(chessboard[rMov][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }    
            }
        }

        case 'k' : return true;

        case 't' :
        {
            if(vertical)
            {
                if(dR > 0)
                {
                    while (rMov < toRow)
                    {
                        rMov += 1;
                        if(chessboard[rMov][fromCol] not_eq nullptr) return false;
                    }
                    return true;
                }
                else 
                {
                    while (rMov > toRow)
                    {
                        rMov -= 1;
                        if(chessboard[rMov][fromCol] not_eq nullptr) return false;
                    }
                    return true;
                }
            }
            else if(horizontal)
            {
                if(dC > 0)
                {
                    while (cMov < toCol)
                    {
                        cMov += 1;
                        if(chessboard[fromRow][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }
                else
                {
                    while (cMov > toCol)
                    {
                        cMov -= 1;
                        if(chessboard[fromRow][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }
            }
        }

        case 'p' :
        {
            if(dR == 2)
            {
                rMov+=1;
                if(chessboard[rMov][fromCol] not_eq nullptr) return false;
                return true;
            }
            else if (dR == -2) 
            {
                rMov -= 1;
                if(chessboard[rMov][fromCol] not_eq nullptr) return false;
                return true;
            }

            else return true; //this is for either a 1 tile diagonal move or 1 tile vertical move
        }

        default: //impossible but ok
            return false;
    }
    return false;
}