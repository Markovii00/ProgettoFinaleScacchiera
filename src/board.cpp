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

board::board(std::vector<chessman*> copy, coords& start, coords& end) {
    chessman* copyBoard[8][8];  
    for(unsigned int i = 0; i < 64; i++) 
        copyBoard[i/8][i%8] = copy.at(i);
    //manca la copia dei set di pedine        
}

board::board(void)
{
    //possibleEnPassant = false;
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

bool board::kingInCheck(coords& king_coordinates, bool requestColor)
{
    //king is white, look for black pieces that can eat it
    if(!requestColor) {
        for(unsigned int i = 0; i < blackSet.size(); i++) {
            char pedina = chessboard[blackSet[i].first][blackSet[i].second]->getChar();
            if(chessboard[blackSet[i].first][blackSet[i].second]->isLegalMove(blackSet[i].first, blackSet[i].second, king_coordinates.first, king_coordinates.second) && clearPath(blackSet[i], king_coordinates, pedina))
                return false;
        }
    }
    else { //king is black, look for white pieces that can eat it
        for(unsigned int i = 0; i < whiteSet.size(); i++) {
            char pedina = chessboard[whiteSet[i].first][whiteSet[i].second]->getChar();
            if(chessboard[whiteSet[i].first][whiteSet[i].second]->isLegalMove(whiteSet[i].first, whiteSet[i].second, king_coordinates.first, king_coordinates.second) && clearPath(whiteSet[i], king_coordinates, pedina))
                return false;
        }
    }
    return false;
}

bool board::acceptableMove(coords start, coords end, char& fromPieceId) const
{
    if (fromPieceId == 0)
        return false;
    bool fromColor = isBlack(fromPieceId); //true is black

    char toId = getName(end.first, end.second);
    bool toColor = isBlack(toId); //true is black
    if (end.first < 8 && end.second < 8)
    {
        if (toId == 0 || fromColor != toColor)
        {
            return true;
        }
    }
    return false;
}

template<typename Type>
bool is(chessman &data) {
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
coords board::search(bool& requestColor) {
    if(requestColor == true) {
        for(coords i : blackSet) {
            if(is<Type>(*(chessboard[i.first][i.second])))
                return std::make_pair(i.first, i.second);
        }
    }
    else {
        for(coords i : whiteSet) {
            if(is<Type>(*(chessboard[i.first][i.second])))
                return std::make_pair(i.first, i.second);
        }
    }
    return std::make_pair(-1, -1);
}

template<typename Type>
bool board::find(bool& requestColor) const {
    if(requestColor == true) {
        for(coords i : blackSet) {
            if(is<Type>(*(chessboard[i.first][i.second])))
                return true;
        }
    }
    else {
        for(coords i : whiteSet) {
            if(is<Type>(*(chessboard[i.first][i.second])))
                return true;
        }
    }
    return false;
}

std::vector<chessman*> board::copy_board(){
    std::vector<chessman*> ret;
    for(unsigned int i = 0; i < 64; i++) 
        ret.push_back(chessboard[i/8][i%8]);
    return ret;   
}

std::pair<bool,bool> board::move(coords& start, coords& end, bool& pieceToMoveColor) // controllare complessità
{ 
    if(checkTie()) {
        return std::make_pair(false, false); //is over baby
    }

    char fromPieceId = chessboard[start.first][start.second]->getChar();
    bool fromPieceColor = isBlack(fromPieceId);

    // check if the coordinates are within the board limits and whether the destination tile is empty or there is an opponent chessman
    if(!acceptableMove(start, end, fromPieceId)) {
        std::cout << "The selected move is out of bounds, select a new one\n";
        return std::make_pair(false,true);
    }
    if(fromPieceColor != pieceToMoveColor) {
        std::cout << "You can't move an opponent's piece, select a new move\n";
        return std::make_pair(false,true);
    }
    if(!chessboard[start.first][start.second]->isLegalMove(start.first, start.second, end.first, end.second)) {
        std::cout << "The selected move is illegal, select a new one\n";
        return std::make_pair(false,true);
    }
    if(!clearPath(start, end, fromPieceId)) {
        std::cout << "There is a piece in the way of your move, select a new one\n";
        return std::make_pair(false,true);
    }

    //create a copy to verify if after any move the king is under check
    std::vector<chessman*> pointer_copy = copy_board(); 
    board copy(pointer_copy, start, end); 

    if(copy.castling(start, end, pieceToMoveColor)) {
        coords kingCoords = copy.search<king>(fromPieceColor);
        if(kingInCheck(kingCoords, fromPieceColor)) {
            std::cout << "King in check, insert a new move\n";
            return std::make_pair(false,true);
        }
    }
    else if(copy.EnPassant(start, end, fromPieceColor, fromPieceId)) { //RICORDARMI DI FARE LA MOSSA
        coords kingCoords = copy.search<king>(fromPieceColor);
        if(kingInCheck(kingCoords, fromPieceColor)) {
            std::cout << "King in check, insert a new move\n";
            return std::make_pair(false,true);
        }
    }
    else {
        //mossa normale, per il pedone chiama il suo metodo specifico
    }
    


    
    


    //General Move
    executeMove(start, end);
    
    return std::make_pair(true,true);
}

bool board::EnPassant(coords& start, coords& end, bool& kingIsBlack, char& fromPieceId) {
    if (freezeCordinateEnPassant.first != 100 && freezeCordinateEnPassant.second != 100 && is<pawn>(*chessboard[start.first][start.second]))
    {
        // first move && 2 tile row movement
        if (((pawn *)chessboard[start.first][start.second])->PartialEnPassantConditions(start.first, end.first))
        {
            // sure to be next to opponent pawn
            if(fromPieceId == 'p' && ((chessboard[end.first][end.second + 1]->getChar() == 'P') || (chessboard[end.first][end.second - 1]->getChar() == 'P')))
                return true;
            else if (fromPieceId == 'P' && ((chessboard[end.first][end.second + 1]->getChar() == 'p') || (chessboard[end.first][end.second - 1]->getChar() == 'p')))
                return true;
        }
    }
    return false;
}


std::vector<coords> board::KingPossibleMoves(coords kCords, char fromPieceId) const
{
    std::vector<coords> PMoves;
    // i rows j colums
    for (short i = -1; i < 2; i++)
    {
        for (short j = -1; j < 2; j++)
        {
            if (i == 0 && j == 0)
                continue;
            if (acceptableMove(kCords, std::make_pair(kCords.first+i, kCords.second+j), fromPieceId))
            {
                coords amogus;
                amogus.first = kCords.first+i;
                amogus.second = kCords.second+j;
                PMoves.push_back(amogus);
            }
        }
    }
    return PMoves;
}

void board::executeMove(coords start, coords end)
{
    if (getName(end.first, end.second) != 0)
    {
        delete chessboard[end.first][end.second];
    }
    
    chessboard[end.first][end.second] = chessboard[start.first][start.second];
    chessboard[start.first][start.second] = nullptr;
    char toId = getName(end.first, end.second);
    if(is<rook>(*chessboard[end.first][end.second]) && !((rook*)chessboard[start.first][start.second])->hasMoved()) 
        ((rook*)chessboard[end.first][end.second])->setMoved();
    else if(is<king>(*chessboard[end.first][end.second]) && !((king*)chessboard[start.first][start.second])->hasMoved()) 
        ((king*)chessboard[end.first][end.second])->setMoved();
    else if(is<pawn>(*chessboard[end.first][end.second]) && !((pawn*)chessboard[start.first][start.second])->hasMoved()) 
        ((pawn*)chessboard[end.first][end.second])->setMoved();
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
            executeMove(std::make_pair(fromRow, fromCol), std::make_pair(toRow, toCol));
            return true;
        }
        else if (CheckOnEnPassant(fromRow, fromCol, toRow, toCol)) // if not chessman to eat diagonally, might be an en passant
        {
            applyEnPassant(fromRow, fromCol, toRow, toCol);
            return true;
        }
        else // if not none of the above, ILLEGAL MOVE
            return false;
    }
    else //all other legit moves
    {
        if (toRow == 0 || toRow == 7)
        {
            executeMove(std::make_pair(fromRow, fromCol), std::make_pair(toRow, toCol));
            promotion(toRow, toCol);
            return true;
        }  
        else
        {
            if(toPieceId != 0) //check if one/two tile/s ahead there aren't chessmans 
                return false;
            executeMove(std::make_pair(fromRow, fromCol), std::make_pair(toRow, toCol));
            return true;
        }     
    }
    return false;
}

bool board::CheckOnEnPassant(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol) { return possibleEnPassant && freezeCordinateEnPassant.first == fromRow && ((freezeCordinateEnPassant.second == fromCol + 1) || (freezeCordinateEnPassant.second == fromCol - 1)) && abs(freezeCordinateEnPassant.first - toRow) == 1 && freezeCordinateEnPassant.second == toCol; }

void board::applyEnPassant(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol)
{
    chessboard[toRow][toCol] = chessboard[fromRow][fromCol];
    chessboard[fromRow][fromCol] = nullptr;
    delete chessboard[fromRow][toCol];
    chessboard[fromRow][toCol] = nullptr;
}

char board::getName(unsigned short row,unsigned short col) const
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
    bool isWhite = false;
    if(moveCounter > 150){                                               
        std::cout << "75 turns have been done, the game is a tie!";
        return true;
    }
    else if(whiteSetSize == 1 && blackSetSize == 1)                     
    {
        if(find<king>(isBlack) && find<king>(isWhite))
        {
            std::cout << "No more moves are possible, the game is tie!";
            return true;
        }
    }
    else if(whiteSetSize == 2 && blackSetSize == 1)                      
    {
        if((find<king>(isWhite) && find<bishop>(isWhite)) && find<king>(isBlack))
        {
            std::cout << "No more moves are possible, the game is tie!";
            return true;
        }
    }
    else if(whiteSetSize == 1 && blackSetSize == 2)
    {
        if((find<king>(isBlack) && find<bishop>(isBlack)) && find<king>(isWhite))
        {
            std::cout << "No more moves are possible, the game is tie!";
            return true;
        }
    }
    else if(whiteSetSize == 2 && blackSetSize == 1)                       
    {
        if((find<king>(isWhite) && find<knight>(isWhite)) && find<king>(isBlack))
        {
            std::cout << "No more moves are possible, the game is tie!";
            return true;
        }
    }
    else if(whiteSetSize == 1 && blackSetSize == 2)
    {
        if((find<king>(isBlack) && find<knight>(isBlack)) && find<king>(isWhite))
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
    bool isValid = false;

    std::cout << "La pedina in posizione " << pawnRow + 1 << pawnCol + 1 << " può essere promossa, inserire il tipo di pedina desiderato";
    while (!isValid)
    {
        std::cin >> newPiece;
        if(newPiece == "q")
        std::cout << "Pedina richiesta non valida";
    }
    
    delete chessboard[pawnRow][pawnCol];
    if (isPawnBlack == true)
    {
        if ()
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




bool board::isVertical(unsigned short& fromRow, unsigned short& fromCol, unsigned short& toRow, unsigned short& toCol) const { return fromCol == toCol && fromRow != toRow;}

bool board::isHorizontal(unsigned short& fromRow, unsigned short& fromCol, unsigned short& toRow, unsigned short& toCol) const {return fromRow==toRow&&fromCol!=toCol;}

bool board::isDiagonal(unsigned short& fromRow, unsigned short& fromCol, unsigned short& toRow, unsigned short& toCol) const { return abs(toRow - fromRow) == abs(toCol - fromCol); }

bool board::castling(coords& start, coords& end, bool& kingIsBlack) {
    if(is<king>(*chessboard[start.first][start.second]) && !((king *)chessboard[start.first][start.second])->hasMoved()) {
        if (kingIsBlack) { //arrocco corto nero
            if (end.first == 0 && end.second == start.second + 2 && is<rook>(*chessboard[0][7]) && !((rook *)chessboard[0][7])->hasMoved()) {
                short counter = start.second + 1;
                while (counter < 7) {
                    if (getName(start.first, counter) != 0)
                        return false;
                    counter++;
                }
                coords rookStart = std::make_pair(0, 7);
                coords rookEnd = std::make_pair(0, 5);
                executeMove(start, end);
                executeMove(rookStart, rookEnd);
                return true;
            } //arrocco lungo nero
            else if (end.first == 0 && end.second == start.second - 2 && is<rook>(*chessboard[0][0]) && !((rook *)chessboard[0][0])->hasMoved()) {
                short counter = start.second - 1;
                while (counter > 0) {
                    if (getName(start.first, counter) != 0)
                        return false;
                    counter--;
                }
                coords rookStart = std::make_pair(0, 0);
                coords rookEnd = std::make_pair(0, 3);
                executeMove(start, end);
                executeMove(rookStart, rookEnd);
                return true;
            } 
        }  
        else if(!kingIsBlack) { //arrocco corto bianco
            if (end.first == 7 && end.second == start.second + 2 && is<rook>(*chessboard[7][7]) && !((rook *)chessboard[7][7])->hasMoved()) {
                short counter = start.second + 1;
                while (counter < 7) {
                    if (getName(start.first, counter) != 0)
                        return false;
                    counter++;
                }
                coords rookStart = std::make_pair(7, 7);
                coords rookEnd = std::make_pair(7, 5);
                executeMove(start, end);
                executeMove(rookStart, rookEnd);
                return true;
            } //arrocco lungo bianco
            else if (end.first == 7 && end.second == start.second - 2 && is<rook>(*chessboard[7][0]) && !((rook *)chessboard[7][0])->hasMoved()) {
                short counter = start.second - 1;
                while (counter > 0) {
                    if (getName(start.first, counter) != 0)
                        return false;
                    counter--;
                }
                coords rookStart = std::make_pair(7, 0);
                coords rookEnd = std::make_pair(7, 3);
                executeMove(start, end);
                executeMove(rookStart, rookEnd);
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


bool board::clearPath(coords& start, coords& end, char& fromPieceId) const
{
    fromPieceId = tolower(fromPieceId);
    short dR = end.first - start.first;
    short dC = end.second - start.second;

    short rMov = start.first;
    short cMov = start.second;

    bool vertical = isVertical( start.first, start.second, end.first, end.second);
    bool horizontal = isHorizontal( start.first, start.second, end.first, end.second);
    bool diagonal = isDiagonal( start.first, start.second, end.first, end.second);

    switch (fromPieceId)
    {
        case 'c' : return true;
        case 'a' : 
        {
            //movement Sud-Est
            if(dR > 0 && dC > 0) 
            {
                while (rMov < end.first && cMov < end.second)
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
                while (rMov > end.first && cMov < end.second)
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
                while (rMov > end.first && cMov > end.second)
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
                while (rMov < end.first && cMov > end.second)
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
                    while (rMov < end.first)
                    {
                        rMov += 1;
                        if(chessboard[rMov][start.second] not_eq nullptr) return false;
                    }
                    return true;
                }
                else
                {
                    while (rMov > end.first)
                    {
                        rMov -= 1;
                        if(chessboard[rMov][start.second] not_eq nullptr) return false;
                    }
                    return true;
                }
            }
            else if(horizontal) 
            {
                if(dC > 0) 
                {
                    while (cMov < end.second)
                    {
                        cMov += 1;
                        if(chessboard[start.first][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }
                else
                {
                    while (cMov > end.second)
                    {
                        cMov -= 1;
                        if(chessboard[start.first][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }

            }
            else //diagonal movement FIN QUA DIOBOIA
            {
                //movement Sud-Est
                if(dR > 0 && dC > 0) 
                {
                    while (rMov < end.first && cMov < end.second)
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
                    while (rMov > end.first && cMov < end.second)
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
                    while (rMov > end.first && cMov > end.second)
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
                    while (rMov < end.first && cMov > end.second)
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
                    while (rMov < end.first)
                    {
                        rMov += 1;
                        if(chessboard[rMov][start.second] not_eq nullptr) return false;
                    }
                    return true;
                }
                else 
                {
                    while (rMov > end.first)
                    {
                        rMov -= 1;
                        if(chessboard[rMov][start.second] not_eq nullptr) return false;
                    }
                    return true;
                }
            }
            else if(horizontal)
            {
                if(dC > 0)
                {
                    while (cMov < end.second)
                    {
                        cMov += 1;
                        if(chessboard[start.first][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }
                else
                {
                    while (cMov > end.second)
                    {
                        cMov -= 1;
                        if(chessboard[start.first][cMov] not_eq nullptr) return false;
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
                if(chessboard[rMov][start.second] not_eq nullptr) return false;
                return true;
            }
            else if (dR == -2) 
            {
                rMov -= 1;
                if(chessboard[rMov][start.second] not_eq nullptr) return false;
                return true;
            }

            else return true; //this is for either a 1 tile diagonal move or 1 tile vertical move
        }

        default: //impossible but ok
            return false;
    }
    return false;
}