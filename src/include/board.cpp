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
#include "board.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"
/*
if (chessboard[cRow][cCol] not_eq nullptr)
{
}
*/

board::board()
{
    possibleEnPassant = false;
    freezeCordinateEnPassant.first = 0;
    freezeCordinateEnPassant.second = 0;

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

bool board::kingInCheck(short row, short col, bool requestColor)
{
    for (unsigned short cCol = 0; cCol < 8; ++cCol)
    {
        for (unsigned short cRow = 0; cRow < 8; ++cRow)
        {
            if (chessboard[cRow][cCol] not_eq nullptr)
            {
                char id = chessboard[cRow][cCol]->getChar();
                bool cycleColor = isBlack(id);
                if (id != 0 && cycleColor != requestColor && chessboard[cRow][cCol]->isLegalMove(cRow, cCol, row, col) && clearPath(cRow, cCol, row, col))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool board::move(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol) // controllare complessità
{
    // check if the coordinates are within the board limits and whether the destination tile is empty or there is an opponent chessman
    if (!acceptableMove(fromRow, fromCol, toRow, toCol))
        return false;
    if (!chessboard[fromRow][fromCol]->isLegalMove(fromRow, fromCol, toRow, toCol))
        return false;
    if (!clearPath(fromRow, fromCol, toRow, toCol))
        return false;

    char fromPieceId = chessboard[fromRow][fromCol]->getChar();
    bool fromPieceColor = isBlack(fromPieceId);
    short kingCol;
    short kingRow;
    std::string kingCoords = getKing(fromPieceColor);
    kingRow = kingCoords[0];
    kingCol = kingCoords[1];

    // Control on the king
    if (kingInCheck(kingRow, kingCol, fromPieceColor))
    {
        std::vector<std::pair<short, short>> PMoves;
        PMoves = KingPossibleMoves(kingRow, kingCol);
        for (std::pair<short, short> x : PMoves)
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

std::vector<std::pair<short, short>> board::KingPossibleMoves(short fromRow, short fromCol) const
{
    std::vector<std::pair<short, short>> PMoves;
    // i rows j colums
    for (short i = -1; i < 2; i++)
    {
        for (short j = -1; j < 2; j++)
        {
            if (i == 0 && j == 0)
                continue;
            short possRow = fromRow + i;
            short possCol = fromCol + j;
            if (acceptableMove(fromRow, fromCol, possRow, possCol))
            {
                std::pair<short, short> amogus;
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
    if (chessboard[toRow][toCol]->getChar() != 0)
    {
        delete chessboard[toRow][toCol];
    }
    else
        chessboard[toRow][toCol] = chessboard[fromRow][fromCol];
    delete chessboard[fromRow][fromCol];
    chessboard[fromRow][fromCol] = nullptr;
    char toId = getName(toRow, toCol);
    if(isRook(toId) && !((rook*)chessboard[toRow][toCol])->hasMoved()) 
        ((rook*)chessboard[toRow][toCol])->setMoved();
    else if(isKing(toId && !((king*)chessboard[toRow][toCol])->hasMoved())) 
        ((king*)chessboard[toRow][toCol])->setMoved();
    else if(isPawn(toId) && !((pawn*)chessboard[toRow][toCol])->hasMoved()) 
        ((pawn*)chessboard[toRow][toCol])->setMoved();
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

void board::changeTurn()
{
    isWhiteTurn = !isWhiteTurn;
}

bool board::acceptableMove(short fromRow, short fromCol, short toRow, short toCol) const
{
    char fromId = getName(fromRow, fromCol);
    if (fromId == 0)
        return false;
    bool fromIsBlack = isBlack(fromId);

    char toId = getName(toRow, toCol);
    bool toIsBlack = isBlack(toId);
    if (toRow >= 0 && toRow < 8 && toCol >= 0 && toCol < 8)
    {
        if (toId == 0 || fromIsBlack != toIsBlack)
        {
            return true;
        }
    }
    return false;
}

char board::getName(short row, short col) const
{
    if (chessboard[row][col] == nullptr)
        return 0;
    return chessboard[row][col]->getChar();
}

bool board::isBlack(char request) const
{
    if (request >= 'A' && request <= 'Z')
        return true;
    else
        return false;
}

bool board::isEnded()
{
    return true;
}

bool board::clearPath(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol)
{
    // if() lo ho commentato o non riuscivo a fare i miei test da scienziato pazzo uwu
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

void board::printBoard()
{
    for (unsigned short iRow = 0; iRow < 8; ++iRow)
    {
        std::cout << 8 - iRow << "    ";
        for (unsigned short iCol = 0; iCol < 8; ++iCol)
        {
            if (chessboard[iRow][iCol] not_eq nullptr)
                std::cout << chessboard[iRow][iCol]->getChar() << " ";
            else
            {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n     A B C D E F G H";
}

std::string board::to_string(bool fixed_allignment = false)
{
    std::string board;
    for (unsigned short iRow = 0; iRow < 8; ++iRow)
    {
        board += (fixed_allignment ? "\t\t\t       " : "");
        board += std::to_string(8 - iRow) + "    ";
        for (unsigned short iCol = 0; iCol < 8; ++iCol)
        {
            if (chessboard[iRow][iCol] not_eq nullptr)
            {
                board += (chessboard[iRow][iCol]->getChar());
                board += " ";
            }
            else
            {
                board += " ";
            }
        }
        board += "\n";
    }
    board += (fixed_allignment ? "\n\t\t\t       " : "\n");
    board += "     A B C D E F G H";

    return board;
}

std::string board::getKing(bool requestColor)
{
    char kingCol;
    char kingRow;
    for (unsigned short cRow = 0; cRow < 8; cRow++)
    {
        for (unsigned short cCol = 0; cCol < 8; ++cCol)
        {
            if (chessboard[cRow][cCol] not_eq nullptr)
            {
                char id = chessboard[cRow][cCol]->getChar();
                bool cycleColor = isBlack(id);
                if (id == 'r')
                    id = 'R';
                if (id != 0 && cycleColor == requestColor && id == 'R')
                {
                    kingCol = cCol;
                    kingRow = cRow; // found the selected king coordinates to verify if next move is safe
                }
            }
        }
    }
    std::string out = "" + kingRow + kingCol;
    return out;
}

bool board::isPawn(char pieceId) const { return pieceId == 'p' || pieceId == 'P'; }

bool board::isKing(char pieceId) const { return pieceId == 'k' || pieceId == 'K'; }

bool board::isQueen(char pieceId) const { return pieceId == 'd' || pieceId == 'D'; }

bool board::isBishop(char pieceId) const { return pieceId == 'a' || pieceId == 'A'; }

bool board::isKnight(char pieceId) const { return pieceId == 'c' || pieceId == 'C'; }

bool board::isRook(char pieceId) const { return pieceId == 't' || pieceId == 'T'; }


