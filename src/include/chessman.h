/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#ifndef CHESSMAN_H
#define CHESSMAN_H

typedef std::pair<short, short> coords;

class chessman
{
    protected:
    char piece;

    public:
    chessman(char p);
    virtual ~chessman();

    //check if the move can be done by a specific chessman
    virtual bool isLegalMove(const coords& start, const coords& end) = 0;

    //idea : fare un metodo che sapendo la mia posizione, restituisce una lista con tutte le cordinate raggiungibili da quel pezzo in x,y cordinate

    char getChar() const;

    chessman(const chessman &other) = delete;
    chessman &operator=(const chessman &other) = delete;
};

#endif