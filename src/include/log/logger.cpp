/**
* @author Tommaso Gabrieli - 2007967 (tommaso.gabrieli.1@studenti.unipd.it)
*/

#include "logger.h"

logger::logger() {};

bool logger::log(const std::string& who, const std::string& _msg) {
    std::string string_build{"[" + get_hour_time() + "] - " + who + " - " + _msg + "\n"};
    try {
        log_stream << string_build << std::flush;
    } catch (std::exception& e) {
        return false;
    }
    return true;
}
