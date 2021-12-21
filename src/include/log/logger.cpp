//
// Created by Tommaso Gabrieli on 21/12/21.
//

#include "logger.hpp"
#include <iostream>
#include <fstream>
#include "logtoolkit.hpp"


logger::logger(int debug_level=NORMAL) : debug_level(debug_level) {};

bool logger::log(std::string who, std::string _msg) {
    std::string string_build{"[" + get_hour_time() + "] - " + who + " - " + _msg + "\n"};
    log_stream << string_build;
};



