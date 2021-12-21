//
// Created by Tomma on 12/21/2021.
//

#ifndef PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_NOMATCHFOUND_HPP
#define PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_NOMATCHFOUND_HPP

#include <iostream>
#include <exception>

struct NoMatchFound : public std::exception {

public:
    explicit NoMatchFound(const char* msg) : std::exception(){};

};

#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_NOMATCHFOUND_HPP
