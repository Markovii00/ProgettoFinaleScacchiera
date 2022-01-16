/**
 *@author  Alessandro Viespoli - 2009659 (alessandro.viespoli@studenti.unipd.it)
 *
 **/

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cctype>
#include "include/board.h"

//IL BOT POI PRENDERà A CASO UNA DI QUESTE COORDINATE, FARà IL GETALLMOVES, DOVRà VEDERE SE QUESTE SONO SAFE PER IL SUO RE 
//TODO RIFARE METODI PER APPLICARE LA MOSSA ALLA COPIA (SE PEDINA VIENE MANGIATA NON FARE IL DELETE, MA SOLO METTERE IL PUNTATORE A NULLPTR)
//TODO NEL MOVE PUOI VA ESEGUITA QUESTA MOSSA 
//TODO ALLA FINE DEL METODO MOVE GUARDARE SE IL NEMICO è SOTTO SCACCO MATTO 

bool board::kingInCheck(const coords& king_coordinates, const bool& requestColor) const {
    //king is white, look for black pieces that can eat it
    if(!requestColor) {
        for(unsigned int i = 0; i < blackSet.size(); i++) {
            chessman* piece = chessboard[blackSet[i].first][blackSet[i].second];
            char pedina = piece->getChar();
            if(piece->isLegalMove(blackSet[i], king_coordinates) && clearPath(blackSet[i], king_coordinates, pedina)) {
                std::cout << "errore in king in check1\n";
                return true;
            }    
        }
    }
    else { //king is black, look for white pieces that can eat it
        for(unsigned int i = 0; i < whiteSet.size(); i++) {
            chessman* piece = chessboard[whiteSet[i].first][whiteSet[i].second];
            char pedina = piece->getChar();
            if(piece->isLegalMove(whiteSet[i], king_coordinates) && clearPath(whiteSet[i], king_coordinates, pedina)) {
                std::cout << "errore in king in check2\n";
                return true;
            }
        }
    }
    std::cout << "king in check ok\n";
    return false;
}

bool board::check_on_pawn(const coords& start, const coords& end, const char& fromPieceId, const bool& fromPieceColor) const {
    //the piece in start is a pawn, already checked
    short dir = (fromPieceColor)? 1 : -1;
    if(abs(end.first - start.first) == 1 && abs(end.second - start.second) == 1) { //diagonal move
        if(isEnpassant(start,end).first) 
            return true;
        else if(isPawnEating(start, end, fromPieceColor))
            return true;
    }
    else if(isVertical(start, end)) { //isLegalMove will check if the pawn made the first move
            if(abs(end.first - start.first) == 2) {                                                                        
                if(chessboard[start.first +dir][start.second] == nullptr && chessboard[start.first +dir +dir][start.second] == nullptr)
                    return true;
            }
            else if (abs(end.first - start.first) == 1) {
                if(chessboard[start.first + dir][start.second] == nullptr) 
                    return true;
            }
    }

    return false;
}

bool board::acceptableMove(const coords& start, const coords& end, const char& fromPieceId, const bool& fromPieceColor) const {
    if (fromPieceId == 0)
        return false;

    if (withinBoardLimits(end)) {
        if(is<pawn>(*chessboard[start.first][start.second])) 
            return check_on_pawn(start, end, fromPieceId, fromPieceColor);
        else if(is<king>(*chessboard[start.first][start.second]) && abs(end.second - start.second) == 2 && isHorizontal(start, end))
            return isCastling(start, end).first;
        else {
            char toId = getName(end.first, end.second);
            bool toColor = isBlack(toId); //true is black
            if (toId == '0' || fromPieceColor != toColor) {
                return true;  
            }     
        }
    }
    
    return false;
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

        if(!((pawn *)piece)->hasMoved() && acceptableMove(_pos, _pos + std::make_pair(2*dir, 0), namePiece, colorPiece))
            ret_.push_back(_pos + std::make_pair(2*dir, 0)); 
        if(acceptableMove(_pos, _pos + std::make_pair(1*dir, 0), namePiece, colorPiece))
            ret_.push_back(_pos + std::make_pair(1*dir, 0)); 

        if(isEnpassant(_pos, _pos + std::make_pair(1*dir, 1)).first)
            ret_.push_back(_pos + std::make_pair(1*dir, 1));
        if(isEnpassant(_pos, _pos + std::make_pair(1*dir, -1)).first)
            ret_.push_back(_pos + std::make_pair(1*dir, -1));

        if(isPawnEating(_pos, _pos + std::make_pair(1*dir, 1), colorPiece))
            ret_.push_back(_pos + std::make_pair(1*dir, 1));  
        if(isPawnEating(_pos, _pos + std::make_pair(1*dir, -1), colorPiece))
            ret_.push_back(_pos + std::make_pair(1*dir, -1));
    }
    else if(is<knight>(*piece)) {
        if(acceptableMove(_pos, _pos + std::make_pair(2, 1), namePiece, colorPiece))
            ret_.push_back(_pos + std::make_pair(2, 1));
        if(acceptableMove(_pos, _pos + std::make_pair(2, -1), namePiece, colorPiece))
            ret_.push_back(_pos + std::make_pair(2, -1));
        if(acceptableMove(_pos, _pos + std::make_pair(1, 2), namePiece, colorPiece))
            ret_.push_back(_pos + std::make_pair(1, 2));
        if(acceptableMove(_pos, _pos + std::make_pair(1, -2), namePiece, colorPiece))
            ret_.push_back(_pos + std::make_pair(1, -2));
        if(acceptableMove(_pos, _pos + std::make_pair(-2, 1), namePiece, colorPiece))
            ret_.push_back(_pos + std::make_pair(-2, 1));
        if(acceptableMove(_pos, _pos + std::make_pair(-2, -1), namePiece, colorPiece))
            ret_.push_back(_pos + std::make_pair(-2, -1));
        if(acceptableMove(_pos, _pos + std::make_pair(-1, -2), namePiece, colorPiece))
            ret_.push_back(_pos + std::make_pair(-1, -2));
        if(acceptableMove(_pos, _pos + std::make_pair(-1, 2), namePiece, colorPiece))
            ret_.push_back(_pos + std::make_pair(-1, 2));
    }
    else { //Queen Rook Bishop
        while (counter < 8 && (Nord || Sud || East || West || Nord_East || Nord_West || Sud_East || Sud_West)) {

            if(Nord) Nord = _pos.first - counter >= 0;
            if(Sud) Sud = _pos.first + counter < 8;
            if(East) East = _pos.second + counter < 8;
            if(West) West = _pos.second - counter >= 0;
            if(Nord_East) Nord_East = _pos.first - counter >= 0 && _pos.second + counter < 8;
            if(Nord_West) Nord_West = _pos.first - counter >= 0 && _pos.second - counter >= 0;
            if(Sud_East) Sud_East = _pos.first + counter < 8 && _pos.second + counter < 8;
            if(Sud_West) Sud_West = _pos.first + counter < 8 && _pos.second - counter >= 0;

            //Vertical Moves
            if(Nord && piece->isLegalMove(_pos, _pos + std::make_pair(-counter,0))) 
                Nord = attemptMove(ret_, colorPiece, _pos + std::make_pair(-counter,0));
            if(Sud && piece->isLegalMove(_pos, _pos + std::make_pair(counter,0))) 
                Sud = attemptMove(ret_, colorPiece, _pos + std::make_pair(counter,0));

            //Horizontal Moves
            if(East && piece->isLegalMove(_pos, _pos + std::make_pair(0, counter))) 
                East = attemptMove(ret_, colorPiece, _pos + std::make_pair(0, counter));
            if(West && piece->isLegalMove(_pos, _pos + std::make_pair(0, -counter))) 
                West = attemptMove(ret_, colorPiece, _pos + std::make_pair(0, -counter));

            //Diagonal Moves
            if(Nord_East && piece->isLegalMove(_pos, _pos + std::make_pair(-counter, counter)) && withinBoardLimits(_pos + std::make_pair(-counter, counter)))   
                Nord_East = attemptMove(ret_, colorPiece, _pos + std::make_pair(-counter, counter));
            if(Nord_West && piece->isLegalMove(_pos, _pos + std::make_pair(-counter, -counter)) && withinBoardLimits(_pos + std::make_pair(-counter, counter))) 
                Nord_West = attemptMove(ret_, colorPiece, _pos + std::make_pair(-counter, -counter));
            if(Sud_East && piece->isLegalMove(_pos, _pos + std::make_pair(counter, counter)) && withinBoardLimits(_pos + std::make_pair(-counter, counter))) 
                Sud_East = attemptMove(ret_, colorPiece, _pos + std::make_pair(counter, counter));
            if(Sud_West && piece->isLegalMove(_pos, _pos + std::make_pair(counter, -counter)) && withinBoardLimits(_pos + std::make_pair(-counter, counter))) 
                Sud_West = attemptMove(ret_, colorPiece, _pos + std::make_pair(counter, -counter));
            
            counter++;
        }
    }
    return ret_;
}

bool board::attemptMove(std::vector<coords>& _vet, const bool& colorPiece, const coords& _tempCoords) const {
    bool statement = true;

    if(chessboard[_tempCoords.first][_tempCoords.second] == nullptr)
        _vet.push_back(_tempCoords);
    else if(chessboard[_tempCoords.first][_tempCoords.second] not_eq nullptr && colorPiece != isBlack(chessboard[_tempCoords.first][_tempCoords.second]->getChar())) {
        _vet.push_back(_tempCoords);
        statement = false; //the piece cant go further
    }
    else statement = false;

    return statement;
}

int board::number_possible_moves(const bool& fromPieceColor) const {
    int num_possible_moves = 0;
    if(fromPieceColor) { 
        for(coords i : blackSet) {
            std::vector<coords> temp = getAllMoves(i);  
            num_possible_moves += temp.size();
        }
    }
    else {
        for(coords i : whiteSet) {
            std::vector<coords> temp = getAllMoves(i); 
            num_possible_moves += temp.size();
        }
    }
    return num_possible_moves;
}

void board::fakeExecuteMove(const coords& start, const coords& end, const int& typeMove, const coords& pawn_to_be_eaten, const coords& rook_to_move) {
    switch (typeMove) {
    case 2: { 
        bool set = isBlack(chessboard[start.first][start.second]->getChar());
        //Tower Movement
        short col_rook_move = (rook_to_move.second == 0) ? 3 : 5;
        chessboard[rook_to_move.first][col_rook_move] = chessboard[rook_to_move.first][rook_to_move.second];
        chessboard[rook_to_move.first][rook_to_move.second] = nullptr;  
        updateCoordsInSet(rook_to_move, std::make_pair(rook_to_move.first, col_rook_move), set);

        //King movement
        chessboard[end.first][end.second] = chessboard[start.first][start.second];
        chessboard[start.first][start.second] = nullptr;
        updateCoordsInSet(start, end, set); //king put here cause call for update change the lastMoveCoords variable
    }
    case 1: { 
        bool pieceEaten = isBlack(chessboard[pawn_to_be_eaten.first][pawn_to_be_eaten.second]->getChar());

        chessboard[end.first][end.second] = chessboard[start.first][start.second];
        chessboard[start.first][start.second] = nullptr;
        updateCoordsInSet(start, end, !pieceEaten);
        
        chessboard[pawn_to_be_eaten.first][pawn_to_be_eaten.second] = nullptr;
        removeFromSet(pawn_to_be_eaten, pieceEaten);
    }
    default: {
        if (getName(end.first, end.second) != 0) 
            removeFromSet(end, isBlack(chessboard[end.first][end.second]->getChar()));
    
        chessboard[end.first][end.second] = chessboard[start.first][start.second];
        chessboard[start.first][start.second] = nullptr;
        updateCoordsInSet(start, end, isBlack(chessboard[end.first][end.second]->getChar()));
    }
    }
}

bool board::isSafeMove(const coords& start, const coords& end, bool& pieceToMoveColor) const {
    std::vector<chessman*> pieces = copy_board();
    board copy(pieces, lastMoveCoords);

    if(copy.isEnpassant(start, end).first)
        copy.fakeExecuteMove(start, end, 1, lastMoveCoords.second, std::make_pair(-1, -1)); 
    else if(copy.isCastling(start, end).first)
        copy.fakeExecuteMove(start, end, 2, std::make_pair(-1, -1), copy.isCastling(start, end).second); 
    else 
        copy.fakeExecuteMove(start, end, 0, std::make_pair(-1, -1), std::make_pair(-1, -1)); 
    
    coords kingCoords = copy.search<king>(pieceToMoveColor);
    if(!copy.kingInCheck(kingCoords, pieceToMoveColor))
        return true;

    return false;  
}

bool board::illegalMove(const coords& start, const coords& end, const char& fromPieceId, const bool& fromPieceColor, const bool& whoseturn) const {
    return !acceptableMove(start, end, fromPieceId, fromPieceColor) || fromPieceColor != whoseturn || !chessboard[start.first][start.second]->isLegalMove(start, end) || !clearPath(start, end, fromPieceId); 
}

std::pair<bool,bool> board::move(coords& start, coords& end, bool& whoseturn)  { 
    char fromPieceId = getName(start.first, start.second);
    bool fromPieceColor = isBlack(fromPieceId);

    int cond = isTie(whoseturn);
    switch (cond) {
    case 1 : return std::make_pair(false, false);
    case 0 : return std::make_pair(true, false); //ask for draw
    }

    if(illegalMove(start, end, fromPieceId, fromPieceColor, whoseturn)) {
        std::cout << "ILLEGAL MOVE \n";
        return std::make_pair(false,true);
    }
    std::cout << "controlli mosse safe ok";
    
    if(!isSafeMove(start, end, fromPieceColor)) {
        std::cout << "King in danger, insert new move \n";
        return std::make_pair(false,true);
    }
    std::cout << "controlli mossa safe per il re ok \n"; 

    std::pair<bool, coords> castlingVar = isCastling(start, end);
        
    if(isEnpassant(start, end).first) 
        do_enpassant(start, end, lastMoveCoords.second);
    else if(castlingVar.first)
        do_castling(start, end, castlingVar.second);
    else executeMove(start, end);
    
    std::cout << "controlli mossa eseguita";
    if(isPromotion(end, fromPieceColor)) 
        promotion(end, fromPieceColor);

    //the enemy is under scacco matto?

    return std::make_pair(true,true);
}

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

bool board::isPawnEating(const coords& start, const coords& end, const bool& fromPieceColor) const {
    //called when i'm sure the piece at start is a pawn
    if(withinBoardLimits(end)) {
        char toPieceId = getName(end.first, end.second); 
        bool toPieceColor = isBlack(toPieceId);
    
        if (isDiagonal(start, end)) 
            if (toPieceColor != fromPieceColor && toPieceId != 0) //can eat diagonally?
                return true;
    }  
    return false;
}

bool board::isPromotion(const coords& end, const bool& fromPieceColor) const {
    short rightRow = (fromPieceColor) ? 7 : 0;
   
    return is<pawn>(*chessboard[end.first][end.second]) && end.first == rightRow;  
}

void board::promotion(const coords& pawnPos, const bool& pawnColor) {
    /*
    char _p;
    bool isValid = false;

    std::cout << "Your pawn can be promoted, insert new piece : ";
    while (!isValid) {
        std::cin >> _p;
        isValid |= _p == ;
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
    */
}

bool board::clearPath(const coords& start, const coords& end, const char& fromPieceId) const {
    //The control for tile end is left in acceptableMove
    char piece = tolower(fromPieceId);
    short dR = end.first - start.first;
    short dC = end.second - start.second;

    short rMov = start.first;
    short cMov = start.second;

    bool vertical = isVertical(start, end);
    bool horizontal = isHorizontal(start, end);
    bool diagonal = isDiagonal(start, end);

    switch (piece) {
        case 'c' : return true;
        case 'a' : {
            if(dR > 0 && dC > 0) { //movement Sud-Est
                while (rMov < end.first && cMov < end.second) {
                    rMov += 1;
                    cMov += 1;
                    if(chessboard[rMov][cMov] not_eq nullptr) return false;
                }
                return true;
            }
            else if(dR < 0 && dC > 0) { //movement Nord-Est
                while (rMov > end.first && cMov < end.second) {
                    rMov -= 1;
                    cMov += 1;
                    if(chessboard[rMov][cMov] not_eq nullptr) return false;
                }
                return true;
            }            
            else if(dR < 0 && dC < 0) { //movement Nord-West
                while (rMov > end.first && cMov > end.second) {
                    rMov -= 1;
                    cMov -= 1;
                    if(chessboard[rMov][cMov] not_eq nullptr) return false;
                }
                return true;
            }            
            else if(dR > 0 && dC < 0) { //movement Sud-West
                while (rMov < end.first && cMov > end.second) {
                    rMov += 1;
                    cMov -= 1;
                    if(chessboard[rMov][cMov] not_eq nullptr) return false;
                }
                return true;
            }
        }

        case 'd' : {
            if(vertical) {
                if(dR > 0) { //Sud
                    while (rMov < end.first) {
                        rMov += 1;
                        if(chessboard[rMov][start.second] not_eq nullptr) return false;
                    }
                    return true;
                }
                else {
                    while (rMov > end.first) { //Nord
                        rMov -= 1;
                        if(chessboard[rMov][start.second] not_eq nullptr) return false;
                    }
                    return true;
                }
            }
            else if(horizontal) {
                if(dC > 0) { //East
                    while (cMov < end.second) {
                        cMov += 1;
                        if(chessboard[start.first][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }
                else { //West
                    while (cMov > end.second) {
                        cMov -= 1;
                        if(chessboard[start.first][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }
            }
            else { //Horizontal
                if(dR > 0 && dC > 0) { //movement Sud-Est
                    while (rMov < end.first && cMov < end.second) {
                        rMov += 1;
                        cMov += 1;
                        if(chessboard[rMov][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }
                else if(dR < 0 && dC > 0) { //movement Nord-Est
                    while (rMov > end.first && cMov < end.second) {
                        rMov -= 1;
                        cMov += 1;
                        if(chessboard[rMov][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }            
                else if(dR < 0 && dC < 0) { //movement Nord-West
                    while (rMov > end.first && cMov > end.second) {
                        rMov -= 1;
                        cMov -= 1;
                        if(chessboard[rMov][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }            
                else if(dR > 0 && dC < 0) { //movement Sud-West
                    while (rMov < end.first && cMov > end.second) {
                        rMov += 1;
                        cMov -= 1;
                        if(chessboard[rMov][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }
            }
        }

        case 'k' : return true; 

        case 't' : {
            if(vertical) {
                if(dR > 0) { //Sud
                    while (rMov < end.first) {
                        rMov += 1;
                        if(chessboard[rMov][start.second] not_eq nullptr) return false;
                    }
                    return true;
                }
                else {
                    while (rMov > end.first) { //Nord
                        rMov -= 1;
                        if(chessboard[rMov][start.second] not_eq nullptr) return false;
                    }
                    return true;
                }
            }
            else if(horizontal) {
                if(dC > 0) { //East
                    while (cMov < end.second) {
                        cMov += 1;
                        if(chessboard[start.first][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }
                else { //West
                    while (cMov > end.second) {
                        cMov -= 1;
                        if(chessboard[start.first][cMov] not_eq nullptr) return false;
                    }
                    return true;
                }
            }
        }

        case 'p' : {
            bool pieceColor = isBlack(fromPieceId);
            return check_on_pawn(start, end, fromPieceId, pieceColor);
        }
    }
    return false;
}

//COSTRUCTORS AND DESTRUCTORS
board::board(void) {
    lastMoveCoords.first = std::make_pair(-1,-1);
    lastMoveCoords.second = std::make_pair(-1,-1);
    moveRule50 = 0;

    // allocating all white pawns and pieces
    for (unsigned short cCol = 0; cCol < 8; ++cCol) {
        chessboard[6][cCol] = new pawn('p', std::make_pair(6, cCol), false);
        whiteSet.emplace_back(6,cCol); //adding all pawn
        whiteSet.emplace_back(7,cCol); //adding all the other pieces
    }

    chessboard[7][0] = new rook('t', std::make_pair(7,0), false);
    chessboard[7][1] = new knight('c', std::make_pair(7,1), false);
    chessboard[7][2] = new bishop('a', std::make_pair(7,2), false);
    chessboard[7][3] = new queen('d', std::make_pair(7,3), false);
    chessboard[7][4] = new king('r', std::make_pair(7,4), false);
    chessboard[7][5] = new bishop('a', std::make_pair(7,5), false);
    chessboard[7][6] = new knight('c', std::make_pair(7,6), false);
    chessboard[7][7] = new rook('t', std::make_pair(7,7), false);

    // allocating all black pawns and pieces
    for (unsigned short cCol = 0; cCol < 8; cCol++) {
        chessboard[1][cCol] = new pawn('P', std::make_pair(1,0), true);
        blackSet.emplace_back(1,cCol); //adding all pawn
        blackSet.emplace_back(0,cCol); //adding all the other pieces
    }

    chessboard[0][0] = new rook('T', std::make_pair(0,0), true);
    chessboard[0][1] = new knight('C', std::make_pair(0,1), true);
    chessboard[0][2] = new bishop('A', std::make_pair(0,2), true);
    chessboard[0][3] = new queen('D', std::make_pair(0,3), true);
    chessboard[0][4] = new king('R', std::make_pair(0,4), true);
    chessboard[0][5] = new bishop('A', std::make_pair(0,5), true);
    chessboard[0][6] = new knight('C', std::make_pair(0,6), true);
    chessboard[0][7] = new rook('T', std::make_pair(0,7), true);

    for (int iRow = 2;  iRow < 5; iRow++) {
        for (int iCol = 0;  iCol < 7; iCol++) {
            chessboard[iRow][iCol] = new emptyTile(' ', std::make_pair(iRow,iCol), true); // @TODO In caso creare classe set

        }
    }

    //Create string
    tables.insert(std::pair<std::string, short>("TCADRACTPPPPPPPP00000000000000000000000000000000pppppppptcadract", 1));
}
board::~board() {
    for (unsigned short cCol = 0; cCol < 8; ++cCol) {
        for (unsigned short cRow = 0; cRow < 8; ++cRow) {
            delete (chessboard[cRow][cCol]);
        }
    }
}

/*board::board(std::vector<chessman*> copy, std::pair<coords, coords> lastMC) {
    lastMoveCoords.first = lastMC.first;
    lastMoveCoords.second = lastMC.second;
    for(unsigned int i = 0; i < 64; i++) {
        chessboard[i/8][i%8] = copy.at(i);
        if(chessboard[i/8][i%8] not_eq nullptr) {
            if(isBlack(chessboard[i/8][i%8]->getChar()))
                blackSet.push_back(std::make_pair(i/8, i%8));
            else 
                whiteSet.push_back(std::make_pair(i/8, i%8));
        } 
    }       
}*/

//COMPLETE METHODS
/*char board::getName(short row, short col) const {
    if (chessboard[row][col] == nullptr)
        return '0';
    return chessboard[row][col]->getChar();
}
std::vector<chessman*> board::copy_board(void) const{
    std::vector<chessman*> ret;
    for(unsigned int i = 0; i < 64; i++) 
        ret.push_back(chessboard[i/8][i%8]); //nullptrs are added too
    return ret;   
}

//DRAW METHODS
void board::insertBoardInMap(void) {
    std::string boardInserted = boardToString();

    std::map<std::string, short>::iterator it;
    it = tables.find(boardInserted);

    if(it == tables.end()) { //configuration never seen
        tables.insert(std::pair<std::string, short>(boardInserted, 1));
    }
    else { //configuration already in the map
        (it->second)++;
    }
}
int board::isTie(const bool& pieceToMoveColor) const {
    coords kingCoords = search<king>(pieceToMoveColor);
    
    if(tables.at(boardToString()) >= 3) {
        std::cout << "Draw? : ";
        return 0;
    }
    else if(draw_for_pieces() || (!kingInCheck(kingCoords, pieceToMoveColor) && number_possible_moves(pieceToMoveColor) == 0) || moveRule50 >= 50) {
        std::cout << "Is a tie, GG\n";
        return 1;
    }
    return -1;
}
bool board::draw_for_pieces() const {
    bool isBlack = true;
    bool isWhite = false;
    coords b_bishop = search<bishop>(isWhite);
    coords B_bishop = search<bishop>(isBlack);
    int blackKnights = howManyAlive<knight>(isBlack);
    int whiteKnights = howManyAlive<knight>(isWhite);

    bool ret = false;

    ret |= whiteSet.size() == 1 && blackSet.size() == 1;
    ret |= whiteSet.size() == 2 && blackSet.size() == 1 && find<king>(isWhite) && find<bishop>(isWhite);
    ret |= whiteSet.size() == 1 && blackSet.size() == 2 && find<bishop>(isBlack) && find<king>(isBlack);
    ret |= whiteSet.size() == 2 && blackSet.size() == 1 && find<king>(isWhite) && find<knight>(isWhite);
    ret |= whiteSet.size() == 1 && blackSet.size() == 2 && find<king>(isBlack) && find<knight>(isBlack);
    ret |= whiteSet.size() == 2 && blackSet.size() == 2 && find<bishop>(isBlack) && find<bishop>(isWhite) && ((b_bishop.first + b_bishop.second) % 2 == (B_bishop.first + B_bishop.second) % 2);
    ret |= whiteSet.size() == 1 && blackSet.size() == 3 && blackKnights == 2;        
    ret |= whiteSet.size() == 3 && blackSet.size() == 1 && whiteKnights == 2;                                       
    return ret;
}

//TEMPLATE METHODS
template<typename Type>
coords board::search(const bool& requestColor) const{
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
bool board::find(const bool& requestColor) const {
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
template<typename Type>
int board::howManyAlive(const bool& requestColor) const {
    int ret = 0;
    if(requestColor == true) {
        for(coords i : blackSet) {
            if(is<Type>(*(chessboard[i.first][i.second])))
                ret++;
        }
    }
    else {
        for(coords i : whiteSet) {
            if(is<Type>(*(chessboard[i.first][i.second])))
                ret++;
        }
    }
    return ret;
}

//MOVES
std::pair<bool, coords> board::isEnpassant(const coords& start, const coords& end) const {
    if(is<pawn>(*(chessboard[lastMoveCoords.second.first][lastMoveCoords.second.second])) && abs(lastMoveCoords.first.first - lastMoveCoords.second.first) == 2) {
        if(is<pawn>(*(chessboard[start.first][start.second])) && start.first == lastMoveCoords.second.first) { //same row and selected a pawn
            if(isDiagonal(start, end) && end.second == lastMoveCoords.second.second) { //diagonal movement in the col of last pawn moved
                return std::make_pair(true, lastMoveCoords.second);
            }
        }
    }
    return std::make_pair(false, std::make_pair(0, 0));
}
void board::do_enpassant(const coords& start, const coords& end, const coords& pawn_to_be_eaten) {

    bool pieceEaten = isBlack(chessboard[pawn_to_be_eaten.first][pawn_to_be_eaten.second]->getChar());

    chessboard[end.first][end.second] = chessboard[start.first][start.second];
    chessboard[start.first][start.second] = nullptr;
    updateCoordsInSet(start, end, !pieceEaten);
    
    delete chessboard[pawn_to_be_eaten.first][pawn_to_be_eaten.second];
    chessboard[pawn_to_be_eaten.first][pawn_to_be_eaten.second] = nullptr;
    removeFromSet(pawn_to_be_eaten, pieceEaten);

    insertBoardInMap();
}
std::pair<bool, coords> board::isCastling(const coords& start, const coords& end) const {
    if(!kingInCheck(start, isBlack(chessboard[start.first][start.second]->getChar()))) {
        if(is<king>(*chessboard[start.first][start.second]) && !((king *)chessboard[start.first][start.second])->hasMoved() && end.second - start.second == 2) {
            //Arrocco corto
            if (is<rook>(*chessboard[start.first][7]) && !((rook *)chessboard[start.first][7])->hasMoved()) {
                short counter = start.second + 1;
                while (counter < 7) { //check if the way between king and rook is clear, and none of the middle tie is threatened by enemy pieces
                    if (getName(start.first, counter) != 0 || kingInCheck(std::make_pair(start.first, counter), isBlack(chessboard[start.first][counter]->getChar())))
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
                while (counter > 1) { //check if the way between king and rook is clear, and none of the middle tie is threatened by enemy pieces
                    if (getName(start.first, counter) != 0 || kingInCheck(std::make_pair(start.first, counter), isBlack(chessboard[start.first][counter]->getChar()))) {
                        return std::make_pair(false, std::make_pair(0, 0));
                    }   
                    counter--;
                }
                if (getName(start.first, counter) != 0) { //In the last column between rook and king there is no need to check if that tile is threatened by enemy pieces
                    return std::make_pair(false, std::make_pair(0, 0));
                }   
                return std::make_pair(true, std::make_pair(start.first, 0));
            }
        }
    }
    return std::make_pair(false, std::make_pair(0, 0));
}
void board::do_castling(const coords& start, const coords& end, const coords& rook_to_move) { 
    bool set = isBlack(chessboard[start.first][start.second]->getChar());
    //Tower Movement
    short col_rook_move = (rook_to_move.second == 0) ? 3 : 5;
    chessboard[rook_to_move.first][col_rook_move] = chessboard[rook_to_move.first][rook_to_move.second];
    chessboard[rook_to_move.first][rook_to_move.second] = nullptr;  
    updateCoordsInSet(rook_to_move, std::make_pair(rook_to_move.first, col_rook_move), set);
    ((rook*)chessboard[rook_to_move.first][col_rook_move])->setMoved();

    //King movement
    chessboard[end.first][end.second] = chessboard[start.first][start.second];
    chessboard[start.first][start.second] = nullptr;
    updateCoordsInSet(start, end, set); //king put here cause call for update change the lastMoveCoords variable
    ((king*)chessboard[end.first][end.second])->setMoved(); 

    insertBoardInMap();
}*/
void board::executeMove(const coords& start, const coords& end) {
    if (chessboard[end.first][end.second]->getChar() != ' ') {
        removeFromSet(end, chessboard[end.first][end.second]->getSet());
        delete chessboard[end.first][end.second];
        moveRule50 = 0; 
    }
    else if(is<pawn>(*chessboard[start.first][start.second]))
        moveRule50 = 0;
    else  moveRule50++;
    
    chessboard[end.first][end.second] = chessboard[start.first][start.second];
    chessboard[start.first][start.second] = new emptyTile(' ', std::make_pair(start.first, start.second), true);
    updateCoordsInSet(start, end, chessboard[end.first][end.second]->getSet());

    if (!chessboard[start.first][start.second]->hasMoved())
        chessboard[start.first][start.second]->setMoved();

    lastMoveCoords.first = start;
    lastMoveCoords.second = end;
    insertBoardInMap();
}


//PRINT
void board::printBoard(void) const {
    for (unsigned short iRow = 0; iRow < 8; ++iRow)
    {
        std::cout << 8 - iRow << "    ";
        for (unsigned short iCol = 0; iCol < 8; ++iCol)
        {
                std::cout << " " << chessboard[iRow][iCol]->getChar() << (iCol == 7 ? "": " |");
        }
        std::cout << "\n" << (iRow == 7 ? "\n" : "     -------------------------------\n");
    }
    std::cout << "      A   B   C   D   E   F   G   H";
}
/*std::string board::to_string(bool fixed_allignment = false) const{
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
}*/

std::string board::boardToString(void) const {
    std::string ret = "";
    for(unsigned short i = 0; i < 64; i++) {
        ret += chessboard[i/8][i%8]->getChar();
    }
    return ret;
} 

//VARIABLE UPDATE
void board::updateCoordsInSet(const coords& start, const coords& end, const bool& pieceMoved) {
    if(pieceMoved) { //black
        for (int i = 0; i < blackSet.size(); ++i) {
            if (blackSet.at(i) == start) {
                blackSet.at(i) = end;
            }
        }
    }
    else {
        for (int i = 0; i < whiteSet.size(); ++i) {
            if (whiteSet.at(i) == start) {
                whiteSet.at(i) = end;
            }
        }
    }
}

void board::removeFromSet(const coords &coordsPieceEaten, const bool& setPieceEaten) {
    bool startRemoving = false;
    if(setPieceEaten) { //black
        for(unsigned short i = 0; i < blackSet.size() - 1; i++) {
            if(blackSet.at(i) == coordsPieceEaten) startRemoving = true;
            if(startRemoving) blackSet.at(i) = blackSet.at(i+1);
        }   
        blackSet.pop_back();
    }
    else {
        for(unsigned short i = 0; i < whiteSet.size() - 1; i++) {
            if(whiteSet.at(i) == coordsPieceEaten) startRemoving = true;
            if(startRemoving) whiteSet.at(i) = whiteSet.at(i+1);
        }   
        whiteSet.pop_back();
    }
}
/*
//BOOL METHODS
bool board::isBlack(const char& request) const {
    if (request >= 'A' && request <= 'Z')
        return true;
    return false;
}
bool board::isVertical(const coords& start, const coords& end) const { return start.second == end.second && start.first != start.second; }
bool board::isHorizontal(const coords& start, const coords& end) const { return start.first == end.first &&  start.second != end.second; }
bool board::isDiagonal(const coords& start, const coords& end) const { return abs(end.first - start.first) == abs(end.second - start.second); }*/
bool board::withinBoardLimits(const coords& end) const {
    return end.first >= 0 && end.first < 8 && end.second >= 0 && end.second < 8;
}

//GET METHODS
std::vector<coords> board::getWhiteSet(void) const { return whiteSet; }
std::vector<coords> board::getBlackSet(void) const { return blackSet; }