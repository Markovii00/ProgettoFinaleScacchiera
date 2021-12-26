//
// Created by Tommaso Gabrieli on 21/12/21.
//

#ifndef PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGGER_HPP
#define PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGGER_HPP

#include <fstream>
#include "logtoolkit.hpp"

const int DEBUG = 0;
const int NORMAL = 1;

class logger {
    std::ofstream log_stream = create_file();
    const int debug_level;

public:
    explicit logger(int debug_level = NORMAL);

    bool log(const std::string& who = "console", const std::string& _msg = "Null log");


    bool debug(const std::string& message);
};


#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGGER_HPP