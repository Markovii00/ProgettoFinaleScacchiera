#include "bot.hpp"
//#include "../../board.cpp"


void bot::set_name(std::vector<std::string>names){
    short namesLenght = names.size();
    short nameindex = rand() % namesLenght;
    std::string nameToSet = names.at(nameindex);
    name.assign(nameToSet.begin(), nameToSet.end());
    names.erase(names.begin()+nameindex);
}

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

char bot::handlePromotion(){
    return 's';
}
