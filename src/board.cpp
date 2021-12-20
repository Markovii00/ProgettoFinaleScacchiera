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
        for (unsigned short cRow = 0; cRow < 8; ++cRow)
    {
        for (unsigned short cCol = 0; cCol < 8; ++cCol)
        {
            chessboard[cCol][cRow]=0;
        }
    }
     
     //allocating all black pawns and pieces

     for (unsigned short cCol = 0; cCol < 8; ++cCol)
     {
         chessboard[cCol][6] = new pawn('P', cCol, 6);
     }
     chessboard[0][7] = new rook('T', 0, 7);
     chessboard[1][7] = new knight('C', 1, 7);
     chessboard[2][7] = new bishop('A', 2, 7);
     chessboard[3][7] = new queen('D');
     chessboard[4][7] = new king('R');
     chessboard[5][7] = new bishop('A', 5, 7);
     chessboard[6][7] = new knight('C', 6, 7);
     chessboard[7][7] = new rook('T', 7, 7);

     //allocating all white pawns and pieces

     for (unsigned short cCol = 0; cCol < 8; cCol++)
     {
         chessboard[cCol][1] = new pawn('p', cCol, 1);
     }
     chessboard[0][1] = new rook('t', 0, 1);
     chessboard[1][1] = new knight('c', 1, 1);
     chessboard[2][1] = new bishop('a', 2, 1);
     chessboard[3][1] = new queen('d');
     chessboard[4][1] = new king('r');
     chessboard[5][1] = new bishop('a', 5, 1);
     chessboard[6][1] = new knight('c', 6, 1);
     chessboard[7][1] = new rook('t', 7, 1);
}

board::~board(){
    for (unsigned short cRow = 0; cRow < 8; ++cRow)
    {
        for (unsigned short cCol = 0; cCol < 8; ++cCol)
        {
            delete chessboard[cCol][cRow];
            chessboard[cCol][cRow] = 0;
        }
    }  
}

bool board::safeKing(char pieceColor)
{
    short cKingCol;
    short cKingRow;
    for (unsigned short cCol = 0; cCol < 8; cCol++)
    {
        for (unsigned short cRow = 0; cRow < cRow; cRow++)
        {
            if(chessboard[cCol][cRow]!=0)
            {
                //if(getColor(chessboard[cCol][cRow]))
            }
        }
        
    }
    

}

char getColor(char request)
{
    if(request >='A' && request <= 'Z') return 'b';
    else return 'w';
}
#endif