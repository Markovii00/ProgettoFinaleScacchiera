/**
 *@file board.cpp
 *@author  Francesco Colla - 2007968 (francesco.colla.2@studenti.unipd.it)
 *@brief
 *
 **/


#include <iostream>
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"
#include "board.h"

board::board()
{
    for (unsigned short cCol = 0; cCol < 8; ++cCol)
    {
        for (unsigned short cRow = 0; cRow < 8; ++cRow)
        {
            chessboard[cRow][cCol] = 0;
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
    unsigned short iRow = 0;
    for (unsigned short cCol = 0; cCol < 8; ++cCol)
    {
        for (auto &cRow : chessboard)
        {   
            chessboard[iRow][cCol]->~chessman();
            delete cRow[cCol];
            cRow[cCol] = nullptr;
            iRow++;
        }
    }
}

bool board::kingInCheck(short col, short row, bool requestColor)
{
    for (auto &cRow : chessboard)
    {
        for (auto &cCol : cRow)
        {
            short counterRow = 0;
            short counterCol = 0;
            char id = (*cCol).getChar();
            bool cycleColor = isBlack(id);
            if (id != 0 && cycleColor != requestColor && (*cCol).isLegalMove(counterCol, counterRow, col, row) && clearPath(counterCol, counterRow, col, row))
            {
                return true;
            }
            counterRow++;
            counterCol++;
        }
    }
    return false;
}

bool board::kingInCheck(bool requestColor)
{
    short targetKingCol;
    short targetKingRow;
    for (unsigned short cRow = 0; cRow < 8; cRow++)
    {
        for (unsigned short cCol = 0; cCol < 8; ++cCol)
        {
            char id = chessboard[cRow][cCol]->getChar();
            bool cycleColor = isBlack(id);
            if (id == 'r')
                id = 'R';
            if (id != 0 && cycleColor == requestColor && id == 'R')
            {
                targetKingCol = cCol;
                targetKingRow = cRow; // found the selected king coordinates to verify if next move is safe
            }
        }
    }
    // scan all opponent's pieces to see if there is a legal move to take the king
    for (auto &cRow : chessboard)
    {
        for (auto &cCol : cRow)
        {
            short counterRow = 0;
            short counterCol = 0;
            char id = (*cCol).getChar();
            bool cycleColor = isBlack(id);
            if (id != 0 && cycleColor != requestColor && (*cCol).isLegalMove(counterCol, counterRow, targetKingCol, targetKingRow) && clearPath(counterCol, counterRow, targetKingCol, targetKingRow))
            {
                return true;
            }
            counterRow++;
            counterCol++;
        }
    }
    return false;
}

bool board::move(unsigned short fromCol, unsigned short fromRow, unsigned short toCol, unsigned short toRow) //controllare complessità
{
    char fromPieceId = chessboard[fromRow][fromCol]->getChar();
    bool fromPieceColor = isBlack(fromPieceId);
    if (kingInCheck(fromPieceColor))
    {
        short kingCol;
        short kingRow;
        for (unsigned short cRow = 0; cRow < 8; cRow++)
        {
            for (unsigned short cCol = 0; cCol < 8; ++cCol)
            {
                char id = chessboard[cRow][cCol]->getChar();
                bool cycleColor = isBlack(id);
                if (id == 'r')
                    id = 'R';
                if (id != 0 && cycleColor == fromPieceColor && id == 'R')
                {
                    kingCol = cCol;
                    kingRow = cRow; // found the selected king coordinates to verify if next move is safe
                }
            }
        }
        if (kingInCheck(kingCol - 1, kingRow + 1, fromPieceColor) && kingInCheck(kingCol, kingRow + 1, fromPieceColor) && kingInCheck(kingCol + 1, kingRow + 1, fromPieceColor))
        {
            if (kingInCheck(kingCol - 1, kingRow, fromPieceColor) && kingInCheck(kingCol + 1, kingRow, fromPieceColor))
            {
                if (kingInCheck(kingCol - 1, kingRow - 1, fromPieceColor) && kingInCheck(kingCol, kingRow - 1, fromPieceColor) && kingInCheck(kingCol + 1, kingRow - 1, fromPieceColor))
                {
                    isEnded();
                }
            }
        }
        else
            while (fromCol != kingCol && fromRow != kingRow)
            {
                std::cout << "Il re è sotto scacco! Muovilo per non perdere la partita! inserendo nuovamente riga e colonna";
                std::cin >> fromRow;
                std::cin >> fromCol;
            }
<<<<<<< Updated upstream
=======
<<<<<<< HEAD
<<<<<<< HEAD
        }
        if(clearPath(fromCol,fromRow,toCol,toRow))
        {
            if(fromPieceId == 'p'|| fromPieceId =='P') movePawn(fromCol,fromRow,toCol,toRow);
            else
            executeMove(fromRow,fromCol,toRow,toCol);
        }
        return true;
=======
=======
>>>>>>> 82c7141c0dbc7819739f402375358befb6bf8502
>>>>>>> Stashed changes
    }
    if (clearPath(fromCol, fromRow, toCol, toRow))
    {
        executeMove(fromRow, fromCol, toRow, toCol);
        if ((toRow == 0 || toRow == 7) && (chessboard[toRow][toCol]->getChar() == 'p' || chessboard[toRow][toCol]->getChar() == 'P'))
            promotion(toRow, toCol);
    }
<<<<<<< Updated upstream
=======
<<<<<<< HEAD
>>>>>>> 82c7141c0dbc7819739f402375358befb6bf8502
=======
>>>>>>> 82c7141c0dbc7819739f402375358befb6bf8502
>>>>>>> Stashed changes
}
void board::executeMove(short fromRow, short fromCol, short toRow, short toCol)
{
    if (chessboard[toRow][toCol]->getChar() != 0)
    {
        chessboard[toRow][toCol]->~chessman();
        delete chessboard[toRow][toCol];
    }
    else
        chessboard[toRow][toCol] = chessboard[fromRow][fromCol];
    delete chessboard[fromRow][fromCol];
    chessboard[fromRow][fromCol] = nullptr;
}

<<<<<<< Updated upstream
=======
<<<<<<< HEAD
<<<<<<< HEAD
bool board::movePawn(unsigned short fromCol, unsigned short fromRow, unsigned short toCol, unsigned short toRow)
{
    if(toRow==0||toRow==7)
    {
        promotion(toCol, toRow);
    }
    return true;
}

=======
>>>>>>> 82c7141c0dbc7819739f402375358befb6bf8502
=======
>>>>>>> 82c7141c0dbc7819739f402375358befb6bf8502
>>>>>>> Stashed changes
void board::changeTurn()
{
    isWhiteTurn = !isWhiteTurn;
}

bool board::acceptableMove(short fromRow, short fromCol, short toRow, short toCol)
{
    char fromId = chessboard[fromRow][fromCol]->getChar();
    bool fromIsBlack = isBlack(fromId);
    char toId = chessboard[toRow][toCol]->getChar();
    bool toIsBlack = isBlack(toId);
    if (toRow > 0 && toRow < 9 && toCol > 0 && toCol < 9)
    {
        if (chessboard[toRow][toCol] == nullptr && fromIsBlack != toIsBlack)
        {
            return true;
        }
    }
    return false;
}

bool board::isBlack(char request)
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

bool board::clearPath(unsigned short fromCol, unsigned short fromRow, unsigned short toCol, unsigned short toRow)
{
<<<<<<< Updated upstream
    if (chessboard[fromRow][fromCol]->isLegalMove(fromCol, fromRow, toCol, fromRow) || acceptableMove(fromRow, fromCol, toRow, toCol))
=======
<<<<<<< HEAD
<<<<<<< HEAD
    while(fromCol!=toCol&&fromCol!=toCol)
=======
    if (chessboard[fromRow][fromCol]->isLegalMove(fromCol, fromRow, toCol, fromRow) || acceptableMove(fromRow, fromCol, toRow, toCol))
>>>>>>> 82c7141c0dbc7819739f402375358befb6bf8502
=======
    if (chessboard[fromRow][fromCol]->isLegalMove(fromCol, fromRow, toCol, fromRow) || acceptableMove(fromRow, fromCol, toRow, toCol))
>>>>>>> 82c7141c0dbc7819739f402375358befb6bf8502
>>>>>>> Stashed changes
    {
    }
    return true;
}

void board::promotion(unsigned short pawnCol,unsigned short pawnRow)
{
    char newPiece;
    char id = chessboard[pawnRow][pawnCol]->getChar();
    bool isPawnBlack = isBlack(id);
    std::cout << "La pedina in posizione " << pawnRow + 1 << pawnCol + 1 << " può essere promossa, inserire il tipo di pedina desiderato";
    std::cin >> newPiece;
    while (newPiece == 'p' || newPiece == 'P' || newPiece == 'r' || newPiece == 'R')
    {
        std::cout << "Pedina richiesta non valida";
        std::cin >> newPiece;
    }
    delete chessboard[pawnRow][pawnCol];
    if (isPawnBlack == true)
    {
        if (newPiece == 'd' || newPiece == 'D')
            chessboard[pawnCol][pawnRow] = new queen('D');
        else if (newPiece == 'a' || newPiece == 'A')
            chessboard[pawnCol][pawnRow] = new bishop('A');
        else if (newPiece == 'c' || newPiece == 'C')
            chessboard[pawnCol][pawnRow] = new knight('C');
        else if (newPiece == 't' || newPiece == 'T')
            chessboard[pawnCol][pawnRow] = new rook('T');
    }
    else if (isPawnBlack == false)
    {
        if (newPiece == 'd' || newPiece == 'D')
            chessboard[pawnCol][pawnRow] = new queen('d');
        else if (newPiece == 'a' || newPiece == 'A')
            chessboard[pawnCol][pawnRow] = new bishop('a');
        else if (newPiece == 'c' || newPiece == 'C')
            chessboard[pawnCol][pawnRow] = new knight('c');
        else if (newPiece == 't' || newPiece == 'T')
            chessboard[pawnCol][pawnRow] = new rook('t');
    }
    //implementazione ok, non ottimale. Fare il check solo se un pedone va nell'ultima riga
    //if(accettabile,legale,pedone,riga interessata){promozione}
}
<<<<<<< Updated upstream
=======
<<<<<<< HEAD
<<<<<<< HEAD

void board::printBoard()
{
    unsigned short row = 0;
    unsigned short col = 0;
    char colLetter = 'A';
    for (auto &cRow : chessboard)
    {   std::cout << 8-row << "  "; 
        for (auto &cCol : cRow)
        {
            std::cout << chessboard[col][row]->getChar();
        }
    }
    std::cout<<'\n'<<'\n'<<"  ABCDEFGH";
}
#endif
=======
>>>>>>> 82c7141c0dbc7819739f402375358befb6bf8502
=======
>>>>>>> 82c7141c0dbc7819739f402375358befb6bf8502
>>>>>>> Stashed changes
