/**
*@file board.cpp
*@author  Francesco Colla - 2007968 (francesco.colla.2@studenti.unipd.it)
*@brief
*
**/

#ifndef BOARD_CPP
#define BOARD_CPP

#include <iostream>
#include <pawn.h>
#include <rook.h>
#include <knight.h>
#include <bishop.h>
#include <queen.h>
#include <king.h>

board::board()
{
    for (unsigned short cCol = 0; cCol < 8; ++cCol)
    {
        for (unsigned short cRow = 0; cRow < 8; ++cRow)
        {
            chessboard[cRow][cCol]=0;
        }
    }
     
     //allocating all white pawns and pieces

    for (unsigned short cCol = 0; cCol < 8; ++cCol)
    {
        chessboard[6][cCol] = new pawn('p', cCol, 6);
    }
    chessboard[7][0] = new rook(0, 0, 0, 't', 0, 7);
    chessboard[7][1] = new knight('c', 1, 7);
    chessboard[7][2] = new bishop('a', 2, 7);
    chessboard[7][3] = new queen('d');
    chessboard[7][4] = new king('r');
    chessboard[7][5] = new bishop('a', 5, 7);
    chessboard[7][6] = new knight('c', 6, 7);
    chessboard[7][7] = new rook(0, 0, 0, 't', 7, 7);

    //allocating all black pawns and pieces

    for (unsigned short cCol = 0; cCol < 8; cCol++)
    {
        chessboard[1][cCol] = new pawn('P', cCol, 1);
    }
    chessboard[0][0] = new rook(0, 0, 0, 'T', 0, 1);
    chessboard[0][1] = new knight('C', 1, 1);
    chessboard[0][2] = new bishop('A', 2, 1);
    chessboard[0][3] = new queen('D');
    chessboard[0][4] = new king('R');
    chessboard[0][5] = new bishop('A', 5, 1);
    chessboard[0][6] = new knight('C', 6, 1);
    chessboard[0][7] = new rook(0, 0, 0, 'T', 7, 1);
}

board::~board(){
    for (unsigned short cCol = 0; cCol < 8; ++cCol)
    {
        for (auto & cRow : chessboard)
        {
            delete cRow[cCol];
            cRow[cCol] = nullptr;
        }
    }  
}

bool board::kingInCheck(bool requestColor)
{
    short targetKingCol;
    short targetKingRow;
    for (unsigned short cRow = 0; cRow < 8; cRow++)
    {
        for (unsigned short cCol = 0; cCol < 8; ++cCol)
        {
            char id = (*(chessboard[cRow][cCol])).getChar();
            bool cycleColor = getColor(id);
            if(id == 'r') id = 'R';
            if(id != 0 && cycleColor == requestColor && id == 'R')
            {
                targetKingCol=cCol;
                targetKingRow=cRow;  //found the selected king coordinates to verify if next move is safe
            }
        }
    }
    //scan all opponent's pieces to see if there is a legal move to take the king
    for (auto & cRow : chessboard)
    {
        for (auto & cCol : cRow)
        {
            char id = (*cCol).getChar();
            bool cycleColor = getColor(id);
            if(id !=0 && cycleColor != requestColor && (*cCol).isLegalMove(targetKingCol, targetKingRow))
            {
                return true;
            }
        }
    }
    return false;
}

void board::executeMove(short fromRow, short fromCol, short toRow, short toCol)
{
    chessboard[toRow][toCol] = chessboard[fromRow][fromCol];
    eliminatePiece(fromRow, fromCol);
}

void board::eliminatePiece(short row, short col)
{
    chessboard[row][col] = nullptr;
}

void board::changeTurn()
{
    isWhiteTurn = !isWhiteTurn;
}

bool board::acceptableMove(short fromRow, short fromCol, short toRow, short toCol)
{
    char fromId = (*(chessboard[fromRow][fromCol])).getChar();
    bool fromIsBlack = getColor(fromId);
    char toId = (*(chessboard[toRow][toCol])).getChar();
    bool toIsBlack = getColor(toId);
    if(toRow>0&&toRow<9&&toCol>0&&toCol<9)
    {
        if(chessboard[toRow][toCol] == nullptr && fromIsBlack != toIsBlack)
        {
            return true;
        }
    }
    return false;
}

bool getColor(char request)
{
    if(request >='A' && request <= 'Z') return true;
    else return false;
}
#endif