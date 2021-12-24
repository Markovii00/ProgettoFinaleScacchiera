/**
*@file board.cpp
*@author  Francesco Colla - 2007968 (francesco.colla.2@studenti.unipd.it)
*@brief
*
**/

#ifndef BOARD_CPP
#define BOARD_CPP

#include <iostream>
#include <src/include/board.h>

board::board()
{
        for (unsigned short cCol = 0; cCol < 8; ++cCol)
    {
        for (unsigned short cRow = 0; cRow < 8; ++cRow)
        {
            chessboard[cRow][cCol]=0;
        }
    }
     
     //allocating all black pawns and pieces

     for (unsigned short cCol = 0; cCol < 8; ++cCol)
     {
         chessboard[6][cCol] = new pawn('P', cCol, 6);
     }
     chessboard[7][0] = new rook('T', 0, 7);
     chessboard[7][1] = new knight('C', 1, 7);
     chessboard[7][2] = new bishop('A', 2, 7);
     chessboard[7][3] = new queen('D');
     chessboard[7][4] = new king('R');
     chessboard[7][5] = new bishop('A', 5, 7);
     chessboard[7][6] = new knight('C', 6, 7);
     chessboard[7][7] = new rook('T', 7, 7);

     //allocating all white pawns and pieces

     for (unsigned short cCol = 0; cCol < 8; cCol++)
     {
         chessboard[1][cCol] = new pawn('p', cCol, 1);
     }
     chessboard[1][0] = new rook('t', 0, 1);
     chessboard[1][1] = new knight('c', 1, 1);
     chessboard[1][2] = new bishop('a', 2, 1);
     chessboard[1][3] = new queen('d');
     chessboard[1][4] = new king('r');
     chessboard[1][5] = new bishop('a', 5, 1);
     chessboard[1][6] = new knight('c', 6, 1);
     chessboard[1][7] = new rook('t', 7, 1);
}

board::~board(){
    for (unsigned short cCol = 0; cCol < 8; ++cCol)
    {
        for (unsigned short cRow = 0; cRow < 8; ++cRow)
        {
            delete chessboard[cRow][cCol];
            chessboard[cRow][cCol] = 0;
        }
    }  
}

bool board::kingNotSafe(char pieceIdentity)
{
    short targetKingCol;
    short targetKingRow;
    bool isBlack = true;   //variable used to identify chesspiece color
    for (unsigned short cRow = 0; cRow < 8; cRow++)
    {
        for (unsigned short cCol = 0; cCol < 8; ++cCol)
        {
            if(chessboard[cRow][cCol]!=0&&chessboard[cRow][cCol]->getColor()==isBlack&&chessboard[cRow][cCol]->getChar()=='K')
            {
                targetKingCol=cCol;
                targetKingRow=cRow;  //found the selected king coordinates to verify if next move is safe
            }
        }
    }
    //scan all opponent's pieces to see if there is a legal move to take the king
    for (unsigned short cRow = 0; cRow < 8; cRow++)
    {
        for (unsigned short cCol = 0; cCol < 8; ++cCol)
        {
            if(chessboard[cRow][cCol]!=0&&chessboard[cRow][cCol]->getColor()!=isBlack&&chessboard[cRow][cCol]->isLegalMove())
            {
                return true;
            }
        }
    }
    return false;
}

void board::executeMove(short fromCol, short fromRow, short toCol, short toRow)
{
    chessboard[toRow][toCol] = chessboard[fromRow][fromCol];
    eliminatePiece(fromRow, fromCol);
}

void board::eliminatePiece(short row, short col)
{
    chessboard[row][col] = 0;
}

void board::changeTurn()
{
    isWhiteTurn = !isWhiteTurn;
}

bool board::isTargetValid(short row, short col)
{
    if(row>0&&row<9&&col>0&&col<9)
    {
        return true;
    }
    return false;
}

char getColor(char request)
{
    if(request >='A' && request <= 'Z') return 'b';
    else return 'w';
}
#endif