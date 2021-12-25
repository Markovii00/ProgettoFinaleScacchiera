//
// Created by Tommaso Gabrieli on 21/12/21.
//

#include "logger.hpp"

logger::logger(int debug_level) : debug_level(debug_level) {
};

bool logger::log(const std::string& who, const std::string& _msg) {
    std::string string_build{"[" + get_hour_time() + "] - " + who + " - " + _msg + "\n"};
    try {
        log_stream << string_build;
    } catch (std::exception& e) {
        return false;
    }
    return true;
}

bool logger::debug(const std::string& _msg) {
    if(debug_level != DEBUG) return false;

    std::string string_build{"[" + get_hour_time() + "] - DEBUG - " + _msg + "\n"};
    try {
        log_stream << string_build;
    } catch (std::exception& e) {
        return false;
    }
    return true;
}

