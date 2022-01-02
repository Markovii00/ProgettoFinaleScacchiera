/**
 *@file board.cpp
 *@author  Francesco Colla - 2007968 (francesco.colla.2@studenti.unipd.it)
 *@brief
 *
 **/

#include <iostream>
#include <vector>
#include "board.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"

board::board()
{
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
    unsigned short iRow = 0;
    for (unsigned short cCol = 0; cCol < 8; ++cCol)
    {
        for (auto &cRow : chessboard)
        {
            //Questo if serve ad evitare il segfault nel caso il chessman sia null.
            if (chessboard[iRow][cCol] not_eq nullptr) {
                delete cRow[cCol];
                cRow[cCol] = nullptr;
                iRow++;
            }
        }
    }
}

bool board::kingInCheck(short row, short col, bool requestColor)
{
    for (auto &cRow : chessboard)
    {
        for (auto &cCol : cRow)
        {
            //MODIFICARE DI SICURO
            short counterRow = 0;
            short counterCol = 0;
            char id = (*cCol).getChar();
            bool cycleColor = isBlack(id);
            if (id != 0 && cycleColor != requestColor && (*cCol).isLegalMove(counterRow, counterCol, row, col) && clearPath(counterRow, counterCol, row, col))
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
    std::string coords = getKing(requestColor);
    targetKingRow = coords[0];
    targetKingCol = coords[1];
    // scan all opponent's pieces to see if there is a legal move to take the king
    for (auto &cRow : chessboard)
    {
        for (auto &cCol : cRow)
        {
            //  PROBLEMA CHE NON SI AGGIORNA MAI COROW E CCOL
            short counterRow = 0;
            short counterCol = 0;

            char id = (*cCol).getChar();
            bool cycleColor = isBlack(id);
            if (id != 0 && cycleColor != requestColor && (*cCol).isLegalMove(counterRow, counterCol, targetKingRow, targetKingCol) && clearPath(counterRow, counterCol, targetKingRow, targetKingCol))
            {
                return true;
            }
            counterRow++;
            counterCol++;
        }
    }
    return false;
}

bool board::move(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol) //controllare complessità
{
    //check if the coordinates are within the board limits and whether the destination tile is empty or there is an opponent chessman
    if(!acceptableMove(fromRow,fromCol,toRow,toCol)) return false;
    if(!chessboard[fromRow][fromCol]->isLegalMove(fromRow,fromCol,toRow,toCol)) return false;
    if(!clearPath(fromRow,fromCol,toRow,toCol)) return false;

    char fromPieceId = chessboard[fromRow][fromCol]->getChar();
    bool fromPieceColor = isBlack(fromPieceId);
    short kingCol;
    short kingRow;
    std::string kingCoords = getKing(fromPieceColor);
    kingRow = kingCoords[0];
    kingCol = kingCoords[1];

    //Control on the king
    if (kingInCheck(fromPieceColor)) 
    {
        std::vector<std::pair<short, short>> PMoves;
        PMoves = KingPossibleMoves(kingRow, kingCol);
        for(std::pair<short, short> x : PMoves) 
        {
            if(!kingInCheck(x.first, x.second, fromPieceColor)) 
            {
                std::cout << "Your king is in check, move it";
                return false;
            }
        }
        //all the possible king moves are covered, GG!
        endGame(); 
    }
    
        

        /*
        if(fromPieceId == 'p'|| fromPieceId =='P') movePawn(fromRow,fromCol,toRow,toCol);
        else
        if(kingCol==fromCol&&kingRow==kingCol && ((king*)chessboard[kingRow][kingCol])->hasMoved())
        executeMove(fromRow,fromCol,toRow,toCol);
        */
    
    return true;
}

std::vector<std::pair<short, short>> board::KingPossibleMoves(short fromRow, short fromCol) const 
{
    std::vector<std::pair<short, short>> PMoves;
    //i rows j colums
    for(short i = -1; i < 2; i++) 
    {
        for(short j = -1; j < 2; j++)
        {
            if(i == 0 && j == 0)
                continue;
            short possRow = fromRow + i;
            short possCol = fromCol + j;
            if(acceptableMove(fromRow, fromCol, possRow, possCol)) 
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
    //DA RIFARE, CON 3 IF, OVVERO SE TORRE, RE O PEDONE SPOSTATI SETTARE CON RELATIVO METODO IL FIRSTMOVE = TRUE
    if (chessboard[toRow][toCol]->getChar() != 0)
    {
        delete chessboard[toRow][toCol];
    }
    else
        chessboard[toRow][toCol] = chessboard[fromRow][fromCol];
    delete chessboard[fromRow][fromCol];
    chessboard[fromRow][fromCol] = nullptr;
}

bool board::movePawn(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol)
{
    if(toRow==0||toRow==7)
    {
        promotion(toRow, toCol);
    }
    else executeMove(fromRow,fromCol,toRow,toCol);
    return true;
}

void board::changeTurn()
{
    isWhiteTurn = !isWhiteTurn;
}

bool board::acceptableMove(short fromRow, short fromCol, short toRow, short toCol) const
{
    char fromId = getName(fromRow,fromCol);
    if(fromId == 0) return false;
    bool fromIsBlack = isBlack(fromId);

    char toId = getName(toRow,toCol);
    bool toIsBlack = isBlack(toId);
    if (toRow >= 0 && toRow < 8 && toCol >= 0 && toCol < 8)
    {
        if ( toId == 0 || fromIsBlack != toIsBlack)
        {
            return true;
        }
    }
    return false;
}

char board::getName(short row, short col) const
{
    if(chessboard[row][col] == nullptr) return 0;
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
    //if() lo ho commentato o non riuscivo a fare i miei test da scienziato pazzo uwu
    return false;
}

void board::promotion(unsigned short pawnRow,unsigned short pawnCol)
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
            chessboard[pawnRow][pawnCol] = new queen('D');
        else if (newPiece == 'a' || newPiece == 'A')
            chessboard[pawnRow][pawnCol] = new bishop('A');
        else if (newPiece == 'c' || newPiece == 'C')
            chessboard[pawnRow][pawnCol] = new knight('C');
        else if (newPiece == 't' || newPiece == 'T')
            chessboard[pawnRow][pawnCol] = new rook('T');
    }
    else if (isPawnBlack == false)
    {
        if (newPiece == 'd' || newPiece == 'D')
            chessboard[pawnRow][pawnCol] = new queen('d');
        else if (newPiece == 'a' || newPiece == 'A')
            chessboard[pawnRow][pawnCol] = new bishop('a');
        else if (newPiece == 'c' || newPiece == 'C')
            chessboard[pawnRow][pawnCol] = new knight('c');
        else if (newPiece == 't' || newPiece == 'T')
            chessboard[pawnRow][pawnCol] = new rook('t');
    }
    //implementazione ok, non ottimale. Fare il check solo se un pedone va nell'ultima riga
    //if(accettabile,legale,pedone,riga interessata){promozione}
}

bool castling(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol)
{
    //se fromCol c'è il re && re.hasMoved()==false && toRow == fromRow && toCol == fromCol - 2 (o + 2) && clearpath() && chessboard[toRow][toCOl]->getchar() == t&&torre.hasmoved==false
    //spostare il re a sinistra o destra di 2 e la torre si muove o di col + 3 o col - 2;
    return false;
}

void board::printBoard()
{
    for (unsigned short iRow = 0; iRow < 8; ++iRow) {
        std::cout << 8-iRow << "    ";
        for (unsigned short iCol = 0; iCol < 8; ++iCol) {
            if (chessboard[iRow][iCol] not_eq nullptr)
                std::cout << chessboard[iRow][iCol]->getChar() << " ";
             else {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
    std::cout<<"\n     A B C D E F G H";
}

std::string board::to_string(bool fixed_allignment = false)
{
    std::string board;
    for (unsigned short iRow = 0; iRow < 8; ++iRow) {
        board += (fixed_allignment ? "\t\t\t       " : "");
        board += std::to_string(8-iRow) + "    ";
        for (unsigned short iCol = 0; iCol < 8; ++iCol) {
            if (chessboard[iRow][iCol] not_eq nullptr) {
                board += (chessboard[iRow][iCol]->getChar());
                board += " ";
            }
            else {
                board += " ";
            }
        }
        board += "\n";
    }
    board += (fixed_allignment ? "\n\t\t\t       " : "\n");
    board +="     A B C D E F G H";

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
        std::string out = "" + kingRow + kingCol;
    return out;
}
