#include "bot.hpp"
//#include "../../board.cpp"


void bot::set_name(std::vector<std::string>names){
    short namesLenght = names.size();
    short nameindex = rand() % namesLenght;
    std::string nameToSet = names.at(nameindex);
    name.assign(nameToSet.begin(), nameToSet.end());
    names.erase(names.begin()+nameindex);
}

coords bot::generateFromCoords(){
    if(maiusc){
        updatedSet = b.getBlackSet();
    }
    else{
        updatedSet = b.getWhiteSet();
    }
    indexFrom = rand() % updatedSet.size();
    start  = updatedSet.at(indexFrom);
    
    return start;
}

coords bot::generateEndCoords(coords start){
    allMoves = b.getAllMoves(start);
    indexTo = rand() % allMoves.size();
    end = allMoves.at(indexTo);

    return end;
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
