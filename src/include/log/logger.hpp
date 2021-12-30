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

    /*
     * Costruttore, debug level normale o debug, utile solo per markovii
     */
    explicit logger(int debug_level = NORMAL);

    /*
     * Inserisce una nuova riga nel file di log
     *
     *      who rappresenta il contenuto tra i trattini --
     *      console nel caso di creazione eventi partita
     *
     *      SOLO ED ESCLUSIVAMENTE PER LE AZIONI DEI GIOCATORI USARE IL NOME DEI GIOCATORI!!!!!!
     *      !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     *      !!!!                     BASARSI SUL FILE LOG DI ESEMPIO                        !!!!
     *      !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     */
    bool log(const std::string& who = "console", const std::string& _msg = "Null log");

    /*
     * per markovii
     */
    bool debug(const std::string& message);
};


#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGGER_HPP