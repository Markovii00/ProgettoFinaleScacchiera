//
// Created by markovii on 31/12/2021.
//

#ifndef PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_GRAPHIC_H
#define PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_GRAPHIC_H

#include <iostream>
#include <regex>
#include <match.hpp>
#include <player.hpp>
#include <fstream>

const std::string MATCH_ID{"\\{match-id\\}"};
const std::string PLAYER_1{"\\{player-1\\}"};
const std::string PLAYER_2{"\\{player-2\\}"};
const std::string ROUND_COUNTER{"\\{round-counter\\}"};
const std::string BOARD{"\\{board\\}"};
const std::string DATE{"\\{datetime\\}"};
const std::string HOUR{"\\{hourtime\\}"};

const std::string REPLAY_GRAPHIC_SOURCE{"replay_game.interface"};


bool contains_dynamic_value(std::string& str) {
    std::regex match("\\{.*?\\}");
    return std::regex_search(str, match);
}

inline int unix_time() {
    return time(NULL);
}

inline std::string hourtime() {
    const auto rawtime = (const time_t)unix_time();

    struct tm * dt;
    char timestr[30];
    char buffer [30];

    dt = localtime(&rawtime);
    // use any strftime format spec here
    strftime(timestr, sizeof(timestr), "%H:%M:%S", dt);
    sprintf(buffer,"%s", timestr);
    std::string stdBuffer(buffer);
    return stdBuffer;
}

inline std::string datetime() {
    const auto rawtime = (const time_t)unix_time();

    struct tm * dt;
    char timestr[30];
    char buffer [30];

    dt = localtime(&rawtime);
    // use any strftime format spec here
    strftime(timestr, sizeof(timestr), "%d/%m/%Y", dt);
    sprintf(buffer,"%s", timestr);
    std::string stdBuffer(buffer);
    return stdBuffer;
}


/*
 * Implementazione del "motore" grafico per la visualizzazine del replay.
 *
 */
void g_cout() {


}

std::string scan_variable(match& m) {

    player& p1 = m.get_player(1);
    player& p2 = m.get_player(2);
    board& b = m.get_board();

    std::string elaborated_string;
    std::fstream gstream(REPLAY_GRAPHIC_SOURCE);


    std::regex match_id(MATCH_ID);
    std::regex player_1(PLAYER_1);
    std::regex player_2(PLAYER_2);
    std::regex round_counter(ROUND_COUNTER);
    std::regex board(BOARD);
    std::regex date(DATE);
    std::regex hour(HOUR);

    std::string line;
    while (std::getline(gstream, line)) {
        if (contains_dynamic_value(line)) {
            line = std::regex_replace(line, round_counter, std::to_string(m.get_round()));

            line = std::regex_replace(line, player_1, p1.get_name());
            line = std::regex_replace(line, player_2, p2.get_name());
            line = std::regex_replace(line, date, datetime());
            line = std::regex_replace(line, hour, hourtime());
            line = std::regex_replace(line, board, b.to_string(true));
        }
        elaborated_string += line + "\n";
    }
    return elaborated_string;
}

#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_GRAPHIC_H
