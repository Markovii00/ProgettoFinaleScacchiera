/**
* @author Tommaso Gabrieli - 2007967 (tommaso.gabrieli.1@studenti.unipd.it)
*/


#ifndef PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGGER_HPP
#define PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGGER_HPP

#include <fstream>
#include "logtoolkit.h"

const int DEBUG = 0;
const int NORMAL = 1;

class logger {
    std::ofstream log_stream = create_file();

public:
    /**
    * @brief just an empty constructor, nothing happens here
    */
    explicit logger();

    /**
     * @brief Inserts a new line on the log file
     * @param who indicates who has done an action , console for system debugging
     * @param _msg message to put in your log file
     * @return true -> Log file updated with success!
     *         false -> An error has occurred, log file not updated!
     */
    bool log(const std::string& who = "console", const std::string& _msg = "Null log");

};


#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGGER_HPP
