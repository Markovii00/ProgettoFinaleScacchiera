/**
 * @author Francesco Colla - 2007968 (francesco.colla.2@studenti.unipd.it)
 */

#include "../include/bot.h"

bot::bot(std::string &n, bool set, board &bVar) : b(bVar), maiusc(set), name(n) {}

bot::~bot() {}

std::string bot::get_name() const { return name; }
bool bot::get_set() const { return maiusc; }

std::pair<coords, coords> bot::generateRandomMove(void) {
    std::vector<std::pair<coords, coords>> moves = b.getSetPossibleMoves(maiusc);
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> dist(0, moves.size()-1);
    std::pair<coords, coords> move = moves.at(dist(engine));
    return move;
}

bool bot::handledraw(){
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> dist(1, 100);

    return dist(engine) < 6;
}

bool bot::requestDraw(){
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> dist(1, 100);

    return dist(engine) < 2;
}

std::string bot::handlePromotion() {

    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> dist(0, promotionSet.size()-1);


    return promotionSet.at(dist(engine));
}
