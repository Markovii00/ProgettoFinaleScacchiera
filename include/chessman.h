/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#ifndef CHESSMAN_H
#define CHESSMAN_H

#include <utility>
#include <vector>
#include <cmath>

typedef std::pair<short, short> coords;

class chessman {
    protected:
    char piece;
    coords position;
    bool first_move;
    bool set;

    public:

     /**
     * @param p character of the chessman
     * @param pos position of the chessman
     * @param set true for black set, false for white set
     */
    chessman(char p, coords pos, bool set);
    ~chessman();

     /**
     * @brief Check whether the inserted move is specific for a piece's set of moves
     * @param start selected position in board
     * @param end final destination
     */
    virtual bool isLegalMove(const coords& start, const coords& end) const = 0;

    /**
     * @return all the reachable coordinates based on the actual position of the chessman
     */
    virtual std::vector<coords> possibleMoves(void) const = 0;

    char getChar(void) const;
    coords getPosition(void) const;
    bool hasMoved(void) const;
    bool getSet() const;

    void setPosition(const coords& newPos);
    void setMoved(void);
    void unsetMoved(void);

    chessman(const chessman &other) = delete;
    chessman &operator=(const chessman &other) = delete;
};

//Down here there are all the chessman subclasses

class knight : public chessman {
    public:
    knight(char p, coords pos, bool set);

    bool isLegalMove(const coords& start, const coords& end) const override;
    std::vector<coords> possibleMoves(void) const override;
};

class king : public chessman {
    public:
    king(char p, coords pos, bool set);

    bool isLegalMove(const coords& start, const coords& end) const override;
    std::vector<coords> possibleMoves(void) const override;
};

class bishop : public chessman {
    public :
    bishop(char p, coords pos, bool set);

    bool isLegalMove(const coords& start, const coords& end) const override;
    std::vector<coords> possibleMoves(void) const override;
};

class queen : public chessman {
    public :
    queen(char p, coords pos, bool set);

    bool isLegalMove(const coords& start, const coords& end) const override;
    std::vector<coords> possibleMoves(void) const override;
};

class pawn : public chessman {
    public : 
    pawn(char p, coords pos, bool set);

    bool isLegalMove(const coords& start, const coords& end) const override;
    std::vector<coords> possibleMoves(void) const override;

    //Check part of the enPassant Conditions
    bool PartialEnPassantConditions(unsigned short& _startRow, unsigned short& _destiRow) const;
};

class rook : public chessman{
    public :
    rook(char p, coords pos, bool set);

    bool isLegalMove(const coords& start, const coords& end) const override;
    std::vector<coords> possibleMoves(void) const override;
};

class emptyTile : public chessman{
    public :
    emptyTile(char p, coords pos, bool set);

    bool isLegalMove(const coords& start, const coords& end) const override;
    std::vector<coords> possibleMoves(void) const override;
};

#endif