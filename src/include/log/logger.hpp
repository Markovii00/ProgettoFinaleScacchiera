//
// Created by Tommaso Gabrieli on 21/12/21.
//

#ifndef PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGGER_HPP
#define PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGGER_HPP

#include <iostream>
#include <fstream>
#include "logtoolkit.hpp"

const int DEBUG = 0;
const int NORMAL = 1;

class logger {
    const std::fstream& log_stream(create_log_name());
    const int debug_level;

public:
    logger(int debug_level);

    bool log(std::string who = "console", std::string _msg = "Null log");


};


#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGGER_HPP
