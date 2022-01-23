/**
 *@author  Alessandro Viespoli - 2009659 (alessandro.viespoli@studenti.unipd.it)
 *
 **/

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cctype>
#include "../include/board.h"

//KEY METHODS IN SCACCHIERA.CPP AND BOT MANAGEMENT
std::pair<bool,int> board::move(coords& start, coords& end, bool whoseturn, bool attemptMove = false, bool bypassDraftAsk = false) {
    char fromPieceId = chessboard[start.first][start.second]->getChar();
    bool fromPieceColor = chessboard[start.first][start.second]->getSet();

    if (!attemptMove) {
        std::pair<bool, int> cond = isTie(whoseturn);
        if (cond.first) {
            switch (cond.second) {
                case 3 : return std::make_pair(false, 6);
                case 2 : return std::make_pair(false, 5);
                case 1 : return std::make_pair(false, 4);
                case 0 : {
                    if (!bypassDraftAsk) {
                        return std::make_pair(false, 3); //ask for draw
                    }
                    break;
                }
            }
        }
    }

    if(illegalMove(start, end, fromPieceId, fromPieceColor, whoseturn)) {
        return std::make_pair(false,1);
    }

    //Safe variables before making a move, in case there is the need to undo the move
    int old_moveRule50 = moveRule50;
    std::pair<bool, bool> old_hasMovedBefore = std::make_pair(chessboard[start.first][start.second]->hasMoved(), chessboard[end.first][end.second]->hasMoved());
    std::pair<coords, coords> old_lastMovedCoords = lastMoveCoords;
    char pieceAtEnd = chessboard[end.first][end.second]->getChar();

    std::pair<bool, coords> castlingVar = isCastling(start, end);
    int typeOfMove = 0;

    if(isEnpassant(start, end).first) {
        typeOfMove = 1;
        do_enpassant(start, end, lastMoveCoords.second);
    }
    else if(castlingVar.first) {
        typeOfMove = 2;
        do_castling(start, end, castlingVar.second);
    }
    else executeMove(start, end);

    coords kingPos = search<king>(fromPieceColor);
    if (kingInCheck(kingPos, fromPieceColor)) {
        undoMove(start, end, fromPieceColor, typeOfMove, pieceAtEnd, old_moveRule50, old_lastMovedCoords, old_hasMovedBefore);
        return std::make_pair(false,2);
    }

    if (attemptMove)
        undoMove(start, end, fromPieceColor, typeOfMove, pieceAtEnd, old_moveRule50, old_lastMovedCoords, old_hasMovedBefore);
    
    if(isPromotion(end, fromPieceColor) && !attemptMove) {
        toBePromoted = end;
        needPromotion = true;
        return std::make_pair(true, 2);
    }
        
    if (!attemptMove) {
        if (kingInMate(!whoseturn, search<king>(!whoseturn))) {
            return std::make_pair(true, 3);
        }
    }

    return std::make_pair(true, 1);
}
std::vector<std::pair<coords, coords>> board::getSetPossibleMoves(bool setColor) {
    std::vector<std::pair<coords, coords>> possibleMoves;
    //black
    if (setColor) {
        for (coords piece : blackSet) {
            std::vector<coords> allMovesOfChessman = chessboard[piece.first][piece.second]->possibleMoves();

            for (coords movePosition : allMovesOfChessman) {
                char fromPieceId = chessboard[piece.first][piece.second]->getChar();
                bool fromPieceColor = chessboard[piece.first][piece.second]->getSet();

                std::pair<bool, coords> castlingVar = isCastling(piece, movePosition);
                if(!illegalMove(piece, movePosition, fromPieceId, fromPieceColor, setColor) || isEnpassant(piece, movePosition).first || castlingVar.first) {
                    std::pair<bool, int> res = move(piece, movePosition, setColor, true, false);
                    if (res.first && (res.second == 1 || res.second == 2)) {
                        possibleMoves.emplace_back(piece, movePosition);
                    }
                }
            }
        }
    } else {
        for (coords piece : whiteSet) {
            std::vector<coords> allMovesOfChessman = chessboard[piece.first][piece.second]->possibleMoves();
            for (coords movePosition : allMovesOfChessman) {
                char fromPieceId = chessboard[piece.first][piece.second]->getChar();
                bool fromPieceColor = chessboard[piece.first][piece.second]->getSet();

                std::pair<bool, coords> castlingVar = isCastling(piece, movePosition);
                if(!illegalMove(piece, movePosition, fromPieceId, fromPieceColor, setColor) || isEnpassant(piece, movePosition).first || castlingVar.first) {
                    std::pair<bool, int> res = move(piece, movePosition, setColor, true, false);
                    if (res.first && (res.second == 1 || res.second == 2)) {
                        possibleMoves.emplace_back(piece, movePosition);
                    }

                }
            }
        }
    }
    return possibleMoves;
}


//COSTRUCTOR AND DESTRUCTOR
board::board(void) {
    lastMoveCoords.first = std::make_pair(0,0);
    lastMoveCoords.second = std::make_pair(0,0);

    // allocating all white pawns and pieces
    for (unsigned short cCol = 0; cCol < 8; ++cCol) {
        chessboard[6][cCol] = new pawn('p', std::make_pair(6, cCol), false);
        whiteSet.push_back(std::make_pair(6,cCol)); //adding all pawn
        whiteSet.push_back(std::make_pair(7,cCol)); //adding all the other pieces
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
        chessboard[1][cCol] = new pawn('P', std::make_pair(1,cCol), true);
        blackSet.push_back(std::make_pair(1,cCol)); //adding all pawn
        blackSet.push_back(std::make_pair(0,cCol)); //adding all the other pieces
    }

    chessboard[0][0] = new rook('T', std::make_pair(0,0), true);
    chessboard[0][1] = new knight('C', std::make_pair(0,1), true);
    chessboard[0][2] = new bishop('A', std::make_pair(0,2), true);
    chessboard[0][3] = new queen('D', std::make_pair(0,3), true);
    chessboard[0][4] = new king('R', std::make_pair(0,4), true);
    chessboard[0][5] = new bishop('A', std::make_pair(0,5), true);
    chessboard[0][6] = new knight('C', std::make_pair(0,6), true);
    chessboard[0][7] = new rook('T', std::make_pair(0,7), true);

    for (int iRow = 2;  iRow < 6; iRow++) {
        for (int iCol = 0;  iCol < 8; iCol++) {
            chessboard[iRow][iCol] = new emptyTile(' ', std::make_pair(iRow,iCol), true); 
        }
    }

    //Create string
    tables.insert(std::pair<std::string, short>("TCADRACTPPPPPPPP                                pppppppptcadract", 1));
}
board::~board() {
    for (unsigned short cCol = 0; cCol < 8; ++cCol) {
        for (unsigned short cRow = 0; cRow < 8; ++cRow) {
            delete (chessboard[cRow][cCol]);
        }
    }
}


//METHODS TO KEEP VARIABLES UPDATED
void board::insertBoardInMap(void) {
    std::string boardInserted = boardToString();

    std::map<std::string, short>::iterator it;
    it = tables.find(boardInserted);

    if(it == tables.end()) { //configuration never seen
        tables.insert(tables.begin(),std::pair<std::string, short>(boardInserted, 1));
    }
    else { //configuration already in the map
        (it->second)++;
    }
}
void board::removeBoardFromMap(void) {
    std::string boardInserted = boardToString();

    std::map<std::string, short>::iterator it;
    it = tables.find(boardInserted);

    if(it->second == 1) { //configuration almost added
        tables.erase(boardInserted);
    }
    else { //configuration already in the map before
        (it->second)--;
    }
}
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
void board::addCoordsInSet(const coords& position, const bool& colorSet) {
    if(colorSet)
        blackSet.push_back(position);
    else
        whiteSet.push_back(position);
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

//DRAW
std::pair<bool, int> board::isTie(bool& pieceToMoveColor) {
    coords kingCoords = search<king>(pieceToMoveColor);

    auto it = tables.find(boardToString());
    if (it != tables.end()) {
        if(tables.at(boardToString()) >= 3) 
            return std::make_pair(true, 0);
        else if(draw_for_pieces()) {
            return std::make_pair(true, 1);
        } else if (!kingInCheck(kingCoords, pieceToMoveColor) && getSetPossibleMoves(pieceToMoveColor).empty())  {
            return std::make_pair(true, 2);
        } else if (moveRule50 >= 50) {
            return std::make_pair(true, 3);
        }
    }

    return std::make_pair(false, -1);
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
    ret |= whiteSet.size() == 2 && blackSet.size() == 1 && contains<bishop>(isWhite);
    ret |= whiteSet.size() == 1 && blackSet.size() == 2 && contains<bishop>(isBlack);
    ret |= whiteSet.size() == 2 && blackSet.size() == 1 && contains<knight>(isWhite);
    ret |= whiteSet.size() == 1 && blackSet.size() == 2 && contains<knight>(isBlack);
    ret |= whiteSet.size() == 2 && blackSet.size() == 2 && contains<bishop>(isBlack) && contains<bishop>(isWhite) && ((b_bishop.first + b_bishop.second) % 2 == (B_bishop.first + B_bishop.second) % 2);
    ret |= whiteSet.size() == 1 && blackSet.size() == 3 && blackKnights == 2;        
    ret |= whiteSet.size() == 3 && blackSet.size() == 1 && whiteKnights == 2;

    return ret;
}

//PROMOTION 
bool board::isPromotion(const coords& end, const bool& fromPieceColor) const {
    short rightRow = (fromPieceColor) ? 7 : 0;

    return is<pawn>(*chessboard[end.first][end.second]) && end.first == rightRow;
}
std::pair<bool, bool> board::promotion(short promotionChess, bool pawnColor) {
    if (!needPromotion)
        return std::make_pair(false, false);

    promotionChess = tolower(promotionChess);

    bool hasDonePromotion = false;
    switch (promotionChess) {
        case 'd': {
            chessboard[toBePromoted.first][toBePromoted.second] = new queen((pawnColor) ? 'D' : 'd', toBePromoted, pawnColor);
            needPromotion = false;
            hasDonePromotion = true;
            break;
        }
        case 't': {
            chessboard[toBePromoted.first][toBePromoted.second] = new rook((pawnColor) ? 'T' : 't', toBePromoted, pawnColor);
            needPromotion = false;
            hasDonePromotion = true;
            break;
        }
        case 'a': {
            chessboard[toBePromoted.first][toBePromoted.second] = new bishop((pawnColor) ? 'A' : 'a', toBePromoted, pawnColor);
            needPromotion = false;
            hasDonePromotion = true;
            break;
        }
        case 'c': {
            chessboard[toBePromoted.first][toBePromoted.second] = new knight((pawnColor) ? 'C' : 'c', toBePromoted, pawnColor);
            needPromotion = false;
            hasDonePromotion = true;
            break;
        }
        default: {
            hasDonePromotion = false;
            break;
        }
    }

    if (getSetPossibleMoves(!pawnColor).empty()) {
        return std::make_pair(hasDonePromotion, false);
    } else
        return std::make_pair(hasDonePromotion, true);
}


//APPLYING MOVES
void board::do_enpassant(const coords& start, const coords& end, const coords& pawn_to_be_eaten) {

    bool pieceEaten = chessboard[pawn_to_be_eaten.first][pawn_to_be_eaten.second]->getSet();

    chessboard[end.first][end.second] = chessboard[start.first][start.second];
    chessboard[start.first][start.second] = new emptyTile(' ', start, true);
    updateCoordsInSet(start, end, !pieceEaten);

    chessboard[end.first][end.second]->setPosition(end);
    
    chessboard[pawn_to_be_eaten.first][pawn_to_be_eaten.second] = new emptyTile(' ', pawn_to_be_eaten, true);
    removeFromSet(pawn_to_be_eaten, pieceEaten);

    moveRule50 = 0;
    lastMoveCoords.first = start;
    lastMoveCoords.second = end;

    insertBoardInMap();
}
void board::do_castling(const coords& start, const coords& end, const coords& rook_to_move) {
    bool set = chessboard[start.first][start.second]->getSet();
    short col_rook_move = (rook_to_move.second == 0) ? 3 : 5;

    //Tower Movement
    chessboard[rook_to_move.first][col_rook_move] = chessboard[rook_to_move.first][rook_to_move.second];
    chessboard[rook_to_move.first][rook_to_move.second] = new emptyTile(' ', rook_to_move, true);
    updateCoordsInSet(rook_to_move, std::make_pair(rook_to_move.first, col_rook_move), set);

    chessboard[rook_to_move.first][col_rook_move]->setMoved();
    chessboard[rook_to_move.first][col_rook_move]->setPosition(std::make_pair(rook_to_move.first, col_rook_move));

    //King movement
    chessboard[end.first][end.second] = chessboard[start.first][start.second];
    chessboard[start.first][start.second] = new emptyTile(' ', start, true);
    updateCoordsInSet(start, end, set); //king put here cause call for update change the lastMoveCoords variable

    chessboard[end.first][end.second]->setMoved();
    chessboard[end.first][end.second]->setPosition(end);
    
    moveRule50++;
    lastMoveCoords.first = start;
    lastMoveCoords.second = end;
    insertBoardInMap();
}
void board::executeMove(const coords& start, const coords& end) {
    if (chessboard[end.first][end.second]->getChar() != ' ') {
        removeFromSet(end, chessboard[end.first][end.second]->getSet());
        moveRule50 = 0;
    }
    else if(is<pawn>(*chessboard[start.first][start.second]))
        moveRule50 = 0;
    else  moveRule50++;

    chessboard[end.first][end.second] = chessboard[start.first][start.second];
    chessboard[start.first][start.second] = new emptyTile(' ', std::make_pair(start.first, start.second), true);
    updateCoordsInSet(start, end, chessboard[end.first][end.second]->getSet());
    chessboard[end.first][end.second]->setPosition(end);

    if (!chessboard[end.first][end.second]->hasMoved())
        chessboard[end.first][end.second]->setMoved();

    lastMoveCoords.first = start;
    lastMoveCoords.second = end;
    insertBoardInMap();
}
void board::undoMove(const coords& start, const coords& end, const bool& fromPieceColor, const int& typeOfMove, char &pieceAtEnd, const int& old_moveRule50, const std::pair<coords, coords>& old_lastMovedCoords, const std::pair<bool,bool>& oldMovedVal) {
    moveRule50 = old_moveRule50;
    lastMoveCoords = old_lastMovedCoords;
    removeBoardFromMap();

    switch (typeOfMove) {
        case 1: {
            chessboard[start.first][start.second] = chessboard[end.first][end.second];
            chessboard[end.first][end.second] = new emptyTile(' ', end, true);
            updateCoordsInSet(end,start, fromPieceColor);
            chessboard[start.first][start.second]->setPosition(start);

            char pawnEaten = (fromPieceColor) ? 'p' : 'P';

            chessboard[start.first][end.second] = new pawn(pawnEaten, std::make_pair(start.first, end.second), !fromPieceColor);
            chessboard[start.first][end.second]->setMoved();
            addCoordsInSet(std::make_pair(start.first, end.second), !fromPieceColor);
            return;
        }

        case 2: {
            short originalRookColumn = (end.second == 6) ? 7 : 0; //look king final column to guess original rook position
            short actualRookColumn = (originalRookColumn == 7) ? 5 : 3;

            //Undoing king movement
            chessboard[start.first][start.second] = chessboard[end.first][end.second];
            chessboard[end.first][end.second] = new emptyTile(' ', end, true);
            updateCoordsInSet(end, start, fromPieceColor);
            chessboard[start.first][start.second]->setPosition(start);
            chessboard[start.first][start.second]->unsetMoved();

            //Undoing rook movement
            chessboard[start.first][originalRookColumn] = chessboard[start.first][actualRookColumn];
            chessboard[start.first][actualRookColumn] = new emptyTile(' ', std::make_pair(start.first, actualRookColumn), true);
            updateCoordsInSet(std::make_pair(start.first, actualRookColumn), std::make_pair(start.first, originalRookColumn), fromPieceColor);
            chessboard[start.first][originalRookColumn]->setPosition(std::make_pair(start.first, originalRookColumn));
            chessboard[start.first][originalRookColumn]->unsetMoved();
            
            return;
        }

        default: {
            if (pieceAtEnd != ' ') {
                short pieceAtEndIf = tolower(pieceAtEnd);
                chessboard[start.first][start.second] = chessboard[end.first][end.second];
                if (pieceAtEndIf == 'p')
                    chessboard[end.first][end.second] = new pawn(pieceAtEnd, end, !fromPieceColor);
                else if (pieceAtEndIf == 'd')
                    chessboard[end.first][end.second] = new queen(pieceAtEnd, end, !fromPieceColor);
                else if (pieceAtEndIf == 'c')
                    chessboard[end.first][end.second] = new knight(pieceAtEnd, end, !fromPieceColor);
                else if (pieceAtEndIf == 'a')
                    chessboard[end.first][end.second] = new bishop(pieceAtEnd, end, !fromPieceColor);
                else if (pieceAtEndIf == 't')
                    chessboard[end.first][end.second] = new rook(pieceAtEnd, end, !fromPieceColor);

                addCoordsInSet(end, !fromPieceColor);
                updateCoordsInSet(end, start, fromPieceColor);

                chessboard[start.first][start.second]->setPosition(start);

            } else {
                chessboard[start.first][start.second] = chessboard[end.first][end.second];
                chessboard[end.first][end.second] = new emptyTile(' ', end, true);

                updateCoordsInSet(end, start, fromPieceColor);

                chessboard[start.first][start.second]->setPosition(start);
                
            }

            if (!oldMovedVal.first)
                chessboard[start.first][start.second]->unsetMoved();

            if (oldMovedVal.second)
                chessboard[end.first][end.second]->setMoved();
            return;
        }
    }
}


//INPUT AND GAME CHECKS
bool board::illegalMove(const coords& start, const coords& end, const char& fromPieceId, const bool& fromPieceColor, const bool& whoseturn) const {
    return !acceptableMove(start, end, fromPieceId, fromPieceColor) || fromPieceColor != whoseturn || !chessboard[start.first][start.second]->isLegalMove(start, end) || !clearPath(start, end, fromPieceId) || is<king>(*chessboard[end.first][end.second]);
}
bool board::acceptableMove(const coords& start, const coords& end, const char& fromPieceId, const bool& fromPieceColor) const {
    if (fromPieceId == ' ')
        return false;

    if (withinBoardLimits(end)) {
        if(is<pawn>(*chessboard[start.first][start.second]))
            return check_on_pawn(start, end, fromPieceId, fromPieceColor);
        else if(is<king>(*chessboard[start.first][start.second]) && abs(end.second - start.second) == 2 && isHorizontal(start, end))
            return isCastling(start, end).first;
        else {
            char toId = chessboard[end.first][end.second]->getChar();
            bool toColor = chessboard[end.first][end.second]->getSet(); //true is black
            if (toId == ' ' || fromPieceColor != toColor) {
                return true;
            }
        }
    }
    return false;
}
bool board::kingInCheck(const coords& king_coordinates, const bool& requestColor) const {
    //king is white, look for black pieces that can eat it
    if(!requestColor) {
        for(unsigned int i = 0; i < blackSet.size(); i++) {
            chessman* piece = chessboard[blackSet[i].first][blackSet[i].second];
            char pedina = piece->getChar();
            if(piece->isLegalMove(blackSet[i], king_coordinates) && clearPath(blackSet[i], king_coordinates, pedina))
                return true;
        }
    }
    else { //king is black, look for white pieces that can eat it
        for(unsigned int i = 0; i < whiteSet.size(); i++) {
            chessman* piece = chessboard[whiteSet[i].first][whiteSet[i].second];
            char pedina = piece->getChar();

            if(piece->isLegalMove(whiteSet[i], king_coordinates) && clearPath(whiteSet[i], king_coordinates, pedina))
                return true;
        }
    }
    return false;
}
bool board::kingInMate(bool setToCheck, const coords& kingPos) {
    if (kingInCheck(kingPos, setToCheck)) 
        if(getSetPossibleMoves(setToCheck).empty())
            return true;
    
    return false;
}
bool board::withinBoardLimits(const coords& pos) const { return pos.first >= 0 && pos.first < 8 && pos.second >= 0 && pos.second < 8; }


//CHECKS ON MOVE ACTIONS
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
                if(is<emptyTile>(*chessboard[start.first +dir][start.second]) && is<emptyTile>(*chessboard[start.first +dir +dir][start.second]))
                    return true;
            }
            else if (abs(end.first - start.first) == 1) {
                if(is<emptyTile>(*chessboard[start.first + dir][start.second]))
                    return true;
            }
    }

    return false;
}
bool board::isPawnEating(const coords& start, const coords& end, const bool& fromPieceColor) const {
    //called when i'm sure the piece at start is a pawn
    if(withinBoardLimits(end)) {
        char toPieceId = chessboard[end.first][end.second]->getChar();
        bool toPieceColor = chessboard[end.first][end.second]->getSet();

        if (isDiagonal(start, end))
            if (toPieceColor != fromPieceColor && toPieceId != ' ') //can eat diagonally?
                return true;
    }
    return false;
}
std::pair<bool, coords> board::isEnpassant(const coords& start, const coords& end) const {
    if(is<pawn>(*(chessboard[lastMoveCoords.second.first][lastMoveCoords.second.second])) && abs(lastMoveCoords.first.first - lastMoveCoords.second.first) == 2) {
        if(is<pawn>(*(chessboard[start.first][start.second])) && start.first == lastMoveCoords.second.first) { //same row and selected a pawn
            if(isDiagonal(start, end) && end.second == lastMoveCoords.second.second) { //diagonal movement in the col of last pawn moved
                return std::make_pair(true, lastMoveCoords.second);
            }
        }
    }
    return std::make_pair(false, std::make_pair(-1, -1));
}
std::pair<bool, coords> board::isCastling(const coords& start, const coords& end) const {
    bool castlingSide = chessboard[start.first][start.second]->getSet();
    if(!kingInCheck(start, castlingSide)) {
        if(is<king>(*chessboard[start.first][start.second]) && !(chessboard[start.first][start.second])->hasMoved() && end.second - start.second == 2) {
            //Arrocco corto
            if (is<rook>(*chessboard[start.first][7]) && !(chessboard[start.first][7])->hasMoved()) {
                short counter = start.second + 1;
                while (counter < 7) { //check if the way between king and rook is clear, and none of the middle tie is threatened by enemy pieces
                    if (chessboard[start.first][counter]->getChar() != ' ' || kingInCheck(std::make_pair(start.first, counter), castlingSide))
                        return std::make_pair(false, std::make_pair(-1, -1));
                    counter++;
                }
                return std::make_pair(true, std::make_pair(start.first, 7));
            } 
        }
        else if(is<king>(*chessboard[start.first][start.second]) && !(chessboard[start.first][start.second])->hasMoved() && end.second - start.second == -2) {
            //Arrocco lungo

            if (is<rook>(*chessboard[start.first][0]) && !(chessboard[start.first][0])->hasMoved()) {
                short counter = start.second - 1;
                while (counter > 1) { //check if the way between king and rook is clear, and none of the middle tie is threatened by enemy pieces
                    if (chessboard[start.first][counter]->getChar() != ' '  || kingInCheck(std::make_pair(start.first, counter), castlingSide)) {
                        return std::make_pair(false, std::make_pair(-1, -1));
                    }   
                    counter--;
                }
                if (chessboard[start.first][counter]->getChar() != ' ') { //In the last column between rook and king there is no need to check if that tile is threatened by enemy pieces
                    return std::make_pair(false, std::make_pair(-1, -1));
                }   
                return std::make_pair(true, std::make_pair(start.first, 0));
            }
        }
    }
    return std::make_pair(false, std::make_pair(-1, -1));
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
                rMov += 1;
                cMov += 1;
                while ( rMov < end.first && cMov < end.second) {
                    if(chessboard[rMov][cMov]->getChar() != ' ') return false;
                    rMov += 1;
                    cMov += 1;
                }
                return true;
            }
            else if(dR < 0 && dC > 0) { //movement Nord-Est
                rMov -= 1;
                cMov += 1;

                while (rMov > end.first && cMov < end.second) {
                    if(chessboard[rMov][cMov]->getChar() != ' ') return false;
                    rMov -= 1;
                    cMov += 1;
                }
                return true;
            }            
            else if(dR < 0 && dC < 0) { //movement Nord-West
                rMov -= 1;
                cMov -= 1;
                while (rMov > end.first && cMov > end.second) {
                    if(chessboard[rMov][cMov]->getChar() != ' ') return false;

                    rMov -= 1;
                    cMov -= 1;
                }
                return true;
            }            
            else if(dR > 0 && dC < 0) { //movement Sud-West
                rMov += 1;
                cMov -= 1;
                while (rMov < end.first && cMov > end.second) {
                    if(chessboard[rMov][cMov]->getChar() != ' ') return false;
                    rMov += 1;
                    cMov -= 1;
                }
                return true;
            }
        }

        case 'd' : {
            if(vertical) {
                if(dR > 0) { //Sud
                    rMov += 1;
                    while (rMov < end.first) {
                        if(chessboard[rMov][start.second]->getChar() != ' ') return false;
                        rMov += 1;
                    }
                    return true;
                }
                else {
                    rMov -= 1;
                    while (rMov > end.first) { //Nord
                        if(chessboard[rMov][start.second]->getChar() != ' ') return false;
                        rMov -= 1;
                    }
                    return true;
                }
            }
            else if(horizontal) {
                if(dC > 0) { //East
                    cMov += 1;
                    while (cMov < end.second) {
                        if(chessboard[start.first][cMov]->getChar() != ' ') return false;
                        cMov += 1;
                    }
                    return true;
                }
                else { //West
                    cMov -= 1;
                    while (cMov > end.second) {
                        if(chessboard[start.first][cMov]->getChar() != ' ') return false;
                        cMov -= 1;
                    }
                    return true;
                }
            }
            else { //Diagonal
                if(dR > 0 && dC > 0) { //movement Sud-Est
                    rMov += 1;
                    cMov += 1;
                    while ( rMov < end.first && cMov < end.second) {
                        if(chessboard[rMov][cMov]->getChar() != ' ') return false;
                        rMov += 1;
                        cMov += 1;
                    }
                    return true;
                }
                else if(dR < 0 && dC > 0) { //movement Nord-Est
                    rMov -= 1;
                    cMov += 1;

                    while (rMov > end.first && cMov < end.second) {
                        if(chessboard[rMov][cMov]->getChar() != ' ') return false;
                        rMov -= 1;
                        cMov += 1;
                    }
                    return true;
                }
                else if(dR < 0 && dC < 0) { //movement Nord-West
                    rMov -= 1;
                    cMov -= 1;
                    while (rMov > end.first && cMov > end.second) {
                        if(chessboard[rMov][cMov]->getChar() != ' ') return false;

                        rMov -= 1;
                        cMov -= 1;
                    }
                    return true;
                }
                else if(dR > 0 && dC < 0) { //movement Sud-West
                    rMov += 1;
                    cMov -= 1;
                    while (rMov < end.first && cMov > end.second) {
                        if(chessboard[rMov][cMov]->getChar() != ' ') return false;
                        rMov += 1;
                        cMov -= 1;
                    }
                    return true;
                }
            }
        }

        case 'r' : return true;

        case 't' : {
            if(vertical) {
                if(dR > 0) { //Sud
                    rMov += 1;
                    while (rMov < end.first) {
                        if(chessboard[rMov][start.second]->getChar() != ' ') return false;
                        rMov += 1;
                    }
                    return true;
                }
                else {
                    rMov -= 1;
                    while (rMov > end.first) { //Nord
                        if(chessboard[rMov][start.second]->getChar() != ' ') return false;
                        rMov -= 1;
                    }
                    return true;
                }
            }
            else if(horizontal) {
                if(dC > 0) { //East
                    cMov += 1;
                    while (cMov < end.second) {
                        if(chessboard[start.first][cMov]->getChar() != ' ') return false;
                        cMov += 1;
                    }
                    return true;
                }
                else { //West
                    cMov -= 1;
                    while (cMov > end.second) {
                        if(chessboard[start.first][cMov]->getChar() != ' ') return false;
                        cMov -= 1;
                    }
                    return true;
                }
            }
        }

        case 'p' : return true;
    }
    return false;
}
bool board::isVertical(const coords& start, const coords& end) const { return start.second == end.second && start.first != end.first; }
bool board::isHorizontal(const coords& start, const coords& end) const { return start.first == end.first &&  start.second != end.second; }
bool board::isDiagonal(const coords& start, const coords& end) const { return abs(end.first - start.first) == abs(end.second - start.second); }

//PRINT
void board::printBoard(void) const {
    std::cout << "┌───┐ ┌───┬───┬───┬───┬───┬───┬───┬───┐\n";
    for (unsigned short iRow = 0; iRow < 8; ++iRow)
    {
        std::cout << "│ " << 8 - iRow << " ├─┤";
        for (unsigned short iCol = 0; iCol < 8; ++iCol)
        {
            std::cout << " " << chessboard[iRow][iCol]->getChar() << " │";
        }
        std::cout << "\n" << (iRow == 7 ? "" : "├───┤ ├───┼───┼───┼───┼───┼───┼───┼───┤\n");
    }
    std::cout << "└───┘ └─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┘\n" <<
              "      ┌─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┐\n" <<
              "      │ A │ B │ C │ D │ E │ F │ G │ H │\n" <<
              "      └───┴───┴───┴───┴───┴───┴───┴───┘\n";
}
std::string board::to_string() const{
    std::string bb;

    for (unsigned short iRow = 0; iRow < 8; ++iRow)
    {;
        bb += "  ";
        bb += std::to_string((8 - iRow));
        bb += "    ";
        for (unsigned short iCol = 0; iCol < 8; ++iCol)
        {
            bb += " ";
            bb += chessboard[iRow][iCol]->getChar();
            bb += (iCol == 7) ? "" : " |";
        }
        bb += "\n";
        bb += (iRow == 7 ? "" : "      --------------------------------\n");
    }
    bb += "                                       \n";
    bb += "        A   B   C   D   E   F   G   H  \n";
    return bb;
}
std::string board::boardToString(void) const {
    std::string ret = "";
    for(unsigned short i = 0; i < 64; i++) {
        if (chessboard[i/8][i%8] == nullptr) {
            ret += ' ';
        } else
            ret += chessboard[i/8][i%8]->getChar();
    }
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
bool board::contains(const bool& requestColor) const {
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


//GET METHODS
std::vector<coords> board::getWhiteSet(void) const { return whiteSet; }
std::vector<coords> board::getBlackSet(void) const { return blackSet; }

coords operator+(const coords& start, std::pair<short, short> offset) {
    return std::make_pair(start.first + offset.first, start.second + offset.second);
}

