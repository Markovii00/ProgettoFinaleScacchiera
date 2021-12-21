//
// Created by Tomma on 12/21/2021.
//

#ifndef PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_NOMATCHFOUND_HPP
#define PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_NOMATCHFOUND_HPP

#include <iostream>
#include <exception>
#include <utility>

struct NoMatchFound : public std::exception {
    std::string _msg;

public:
    explicit NoMatchFound(std::string  msg) : _msg(std::move(msg)){};

    const char* what() const noexcept override
    {
        return _msg.c_str();
    }

};

#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_NOMATCHFOUND_HPP
