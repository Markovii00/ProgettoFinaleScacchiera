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

//[V]
board::board(std::vector<chessman*> copy, std::pair<coords, coords> lastMC) {
    lastMoveCoords.first = lastMC.first;
    lastMoveCoords.second = lastMC.second;
    for(unsigned int i = 0; i < 64; i++) {
        chessboard[i/8][i%8] = copy.at(i);
        if(isBlack(chessboard[i/8][i%8]->getChar()))
            blackSet.push_back(std::make_pair(i/8, i%8));
        else 
            whiteSet.push_back(std::make_pair(i/8, i%8));
    }       
}
//[V]
board::board(void)
{
    lastMoveCoords.first = std::make_pair(0,0);
    lastMoveCoords.second = std::make_pair(0,0);
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
//[V]
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
//[V]
bool board::kingInCheck(const coords& king_coordinates, const bool& requestColor) const {
    //king is white, look for black pieces that can eat it
    if(!requestColor) {
        for(unsigned int i = 0; i < blackSet.size(); i++) {
            char pedina = chessboard[blackSet[i].first][blackSet[i].second]->getChar();
            if(chessboard[blackSet[i].first][blackSet[i].second]->isLegalMove(blackSet[i].first, blackSet[i].second, king_coordinates.first, king_coordinates.second) && clearPath(blackSet[i], king_coordinates, pedina))
                return true;
        }
    }
    else { //king is black, look for white pieces that can eat it
        for(unsigned int i = 0; i < whiteSet.size(); i++) {
            char pedina = chessboard[whiteSet[i].first][whiteSet[i].second]->getChar();
            if(chessboard[whiteSet[i].first][whiteSet[i].second]->isLegalMove(whiteSet[i].first, whiteSet[i].second, king_coordinates.first, king_coordinates.second) && clearPath(whiteSet[i], king_coordinates, pedina))
                return true;
        }
    }
    return false;
}
//[V]
bool board::acceptableMove(coords start, coords end, char& fromPieceId, bool& fromPieceColor) const
{
    if (fromPieceId == 0)
        return false;
    
    char toId = getName(end.first, end.second);
    bool toColor = isBlack(toId); //true is black
    if (end.first < 8 && end.second < 8)
    {
        if (toId == 0 || fromPieceColor != toColor)
        {
            return true;
        }
    }
    return false;
}
//[V]
template<typename Type>
coords board::search(bool& requestColor) const{
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
//[V]
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
//[V]
std::vector<chessman*> board::copy_board(){
    std::vector<chessman*> ret;
    for(unsigned int i = 0; i < 64; i++) 
        ret.push_back(chessboard[i/8][i%8]);
    return ret;   
}

std::vector<coords> board::getAllMoves(const coords& _pos) const {
    
    chessman* piece = chessboard[_pos.first][_pos.second];
    char namePiece = piece->getChar();
    bool colorPiece = isBlack(namePiece);

    bool Nord = true;
    bool Sud = true;
    bool East = true;
    bool West = true;
    bool Nord_East = true;
    bool Nord_West = true;
    bool Sud_West = true;
    bool Sud_East = true;

    short counter = 1;

    std::vector<coords> ret_;

    if(is<king>(*piece)) {
        //see if the special move castling is avaiable.
        if(isCastling(_pos, std::make_pair(_pos.first, _pos.second + 2)).first) 
            ret_.push_back(std::make_pair(_pos.first, _pos.second + 2));
        if(isCastling(_pos, std::make_pair(_pos.first, _pos.second - 2)).first) 
            ret_.push_back(std::make_pair(_pos.first, _pos.second - 2));
        
        for (short i = -1; i < 2; i++) {
            for (short j = -1; j < 2; j++) {
                if (i == 0 && j == 0)
                    continue;
                coords temp_king_pos = std::make_pair(_pos.first + i, _pos.second + j);
                if (acceptableMove(_pos, temp_king_pos, namePiece, colorPiece) && !kingInCheck(temp_king_pos, colorPiece)) 
                    ret_.push_back(temp_king_pos);   
            }
        }
    }
    else if(is<pawn>(*piece)) {
        short dir = (colorPiece) ? 1 : -1;

        //!((king *)chessboard[start.first][start.second])->hasMoved()

        if((pawn *)piece->hasMoved())

    }
    else if(is<knight>(*piece)) {

    }
    else {

    }
    return ret_;
}

std::pair<bool,bool> board::move(coords& start, coords& end, bool& pieceToMoveColor)  { 
    char fromPieceId = chessboard[start.first][start.second]->getChar();
    bool fromPieceColor = isBlack(fromPieceId);

    coords kingCoords = search<king>(fromPieceColor);
    //std::vector<coords> king_poss_moves = KingPossibleMoves(kingCoords, fromPieceColor);
    
    if(draw_for_pieces() || (!kingInCheck(kingCoords, fromPieceColor) && /*nessuna pedina può muoversi*/)) {
        std::cout << "Is a tie, GG\n";
        return std::make_pair(false, false); //is over baby
    }

    std::pair<bool, coords> castling = isCastling(start, end); 
    
    // check if the coordinates are within the board limits and whether the destination tile is empty or there is an opponent chessman
    if(!acceptableMove(start, end, fromPieceId, fromPieceColor)) {
        std::cout << "The selected move is out of bounds, select a new one\n";
        return std::make_pair(false,true);
    }
    if(fromPieceColor != pieceToMoveColor) {
        std::cout << "You can't move an opponent's piece, select a new move\n";
        return std::make_pair(false,true);
    }
    if(!chessboard[start.first][start.second]->isLegalMove(start.first, start.second, end.first, end.second) || castling.first) {
        std::cout << "The selected move is illegal, select a new one\n";
        return std::make_pair(false,true);  
    }
    if(!clearPath(start, end, fromPieceId)) {
        std::cout << "There is a piece in the way of your move, select a new one\n";
        return std::make_pair(false,true);
    }

    //create a copy to verify if after any move the king is under check
    std::vector<chessman*> pointer_copy = copy_board(); 
    board copy(pointer_copy, lastMoveCoords); 

    //General Move
    executeMove(start, end);
    
    return std::make_pair(true,true);
}
//[V]
std::pair<bool, coords> board::isEnpassant(coords& start, coords& end) {
    if(is<pawn>(*(chessboard[lastMoveCoords.second.first][lastMoveCoords.second.second])) && abs(lastMoveCoords.first.first - lastMoveCoords.second.first) == 2) {
        if(is<pawn>(*(chessboard[start.first][start.second])) && start.first == lastMoveCoords.second.first) { //same row and selected a pawn
            if(isDiagonal(start, end) && end.second == lastMoveCoords.second.second) { //diagonal movement in the col of last pawn moved
                return std::make_pair(true, lastMoveCoords.second);
            }
        }
    }
    return std::make_pair(false, std::make_pair(0, 0));
}
//[v]
void board::do_enpassant(coords& start, coords& end, coords& pawn_to_be_eaten) {
    chessboard[end.first][end.second] = chessboard[start.first][start.second];
    chessboard[start.first][start.second] = nullptr;

    delete chessboard[pawn_to_be_eaten.first][pawn_to_be_eaten.second];
    chessboard[pawn_to_be_eaten.first][pawn_to_be_eaten.second] = nullptr;
}
//[V]
std::vector<coords> board::KingPossibleMoves(coords& kCords, bool& fromPieceColor) const {
    char king = fromPieceColor ? 'k' : 'K' ;
    std::vector<coords> PMoves;
    for (short i = -1; i < 2; i++) {
        for (short j = -1; j < 2; j++) {
            if (i == 0 && j == 0)
                continue;
            coords temp_king_pos = std::make_pair(kCords.first + i, kCords.second + j);
            if (acceptableMove(kCords, temp_king_pos, king, fromPieceColor) && !kingInCheck(temp_king_pos, fromPieceColor)) {
                PMoves.push_back(temp_king_pos);
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

bool board::movePawn(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol, bool fromPieceColor) {
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
//[V]
char board::getName(unsigned short row,unsigned short col) const
{
    if (chessboard[row][col] == nullptr)
        return 0;
    return chessboard[row][col]->getChar();
}
//[V]
bool board::isBlack(const char& request) const
{
    if (request >= 'A' && request <= 'Z')
        return true;
    else
        return false;
}
//[V]
bool board::draw_for_pieces() const {
    bool isBlack = true;
    bool isWhite = false;
    coords b_bishop = search<bishop>(isWhite);
    coords B_bishop = search<bishop>(isBlack);

    bool ret = false;

    ret |= moveCounter > 150;
    ret |= whiteSet.size() == 1 && blackSet.size() == 1;
    ret |= whiteSet.size() == 2 && blackSet.size() == 1 && find<king>(isWhite) && find<bishop>(isWhite);
    ret |= whiteSet.size() == 1 && blackSet.size() == 2 && find<bishop>(isBlack) && find<king>(isBlack);
    ret |= whiteSet.size() == 2 && blackSet.size() == 1 && find<king>(isWhite) && find<knight>(isWhite);
    ret |= whiteSet.size() == 1 && blackSet.size() == 2 && find<king>(isBlack) && find<knight>(isBlack);
    ret |= whiteSet.size() == 2 && blackSet.size() == 2 && find<bishop>(isBlack) && find<bishop>(isWhite) && ((b_bishop.first + b_bishop.second) % 2 == (B_bishop.first + B_bishop.second) % 2);
                                                          
    return ret;
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
//[V]
bool board::isVertical(coords& start, coords& end) const { return start.second == end.second && start.first != start.second; }
//[V]
bool board::isHorizontal(coords& start, coords& end) const { return start.first == end.first &&  start.second != end.second; }
//[V]
bool board::isDiagonal(coords& start, coords& end) const { return abs(end.first - start.first) == abs(end.second - start.first); }
//[V]
std::pair<bool, coords> board::isCastling(const coords& start, const coords& end) const {
    if(!kingInCheck(start, isBlack(chessboard[start.first][start.second]->getChar()))) {
        if(is<king>(*chessboard[start.first][start.second]) && !((king *)chessboard[start.first][start.second])->hasMoved() && end.second - start.second == 2) {
            //Arrocco corto
            if (is<rook>(*chessboard[start.first][7]) && !((rook *)chessboard[start.first][7])->hasMoved()) {
                short counter = start.second + 1;
                while (counter < 7) {
                    if (getName(start.first, counter) != 0)
                        return std::make_pair(false, std::make_pair(0, 0));
                    counter++;
                }
                return std::make_pair(true, std::make_pair(start.first, 7));
            } 
        }
        else if(is<king>(*chessboard[start.first][start.second]) && !((king *)chessboard[start.first][start.second])->hasMoved() && end.second - start.second == -2) {
            //Arrocco lungo
            if (is<rook>(*chessboard[start.first][0]) && !((rook *)chessboard[start.first][0])->hasMoved()) {
                short counter = start.second - 1;
                while (counter > 0) {
                    if (getName(start.first, counter) != 0)
                        return std::make_pair(false, std::make_pair(0, 0));
                    counter--;
                }
                return std::make_pair(true, std::make_pair(start.first, 0));
            }
        }
    }
}
//[V]
void board::do_castling(coords& start, coords& end, coords& rook_to_move) { 
    //King movement
    chessboard[end.first][end.second] = chessboard[start.first][start.second];
    chessboard[start.first][start.second] = nullptr;

    //Tower Movement
    short col_rook_move = (rook_to_move.second == 0) ? 3 : 5;
    chessboard[rook_to_move.first][col_rook_move] = chessboard[rook_to_move.first][rook_to_move.second];
    chessboard[rook_to_move.first][rook_to_move.second] = nullptr;  
}
//[V]
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
//[V]
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
    bool diagonal = isDiagonal(start, end);

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
                rMov += 1;
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