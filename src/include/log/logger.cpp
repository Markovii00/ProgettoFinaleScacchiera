//
// Created by Tommaso Gabrieli on 21/12/21.
//

#include "logger.hpp"

logger::logger() {};

bool logger::log(const std::string& who, const std::string& _msg) {
    std::string string_build{"[" + get_hour_time() + "] - " + who + " - " + _msg + "\n"};
    try {
        log_stream << string_build;
    } catch (std::exception& e) {
        return false;
    }
    return true;
}
