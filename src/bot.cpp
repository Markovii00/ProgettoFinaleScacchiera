#include "bot.h"

bot::bot(std::string &n, bool set, board &bVar) : b(bVar), maiusc(set), name(n) {}

bot::~bot() {}

std::string bot::get_name() const { return name; }
bool bot::get_set() const { return maiusc; }

std::pair<coords, coords> bot::generateRandomMove(){
    std::vector<std::pair<coords, coords>> moves = b.getSetPossibleMoves(maiusc);
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> dist(0, moves.size()-1);
    std::pair<coords, coords> move = moves.at(dist(engine));
    return move;
}

bool bot::handledraw(){
    short randchoice = rand() % 100 + 1;
    if(randchoice < 25)
        return true;
    else
        return false;
}

bool bot::requestDraw(){
    short randchoice = rand() % 100 + 1;
    if(randchoice < 5)
        return true;
    else
        return false;
}

std::string bot::handlePromotion() {

    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> dist(0, promotionSet.size()-1);


    return promotionSet.at(dist(engine));
}
