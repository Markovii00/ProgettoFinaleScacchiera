/**
* @author Tommaso Gabrieli - 2007967 (tommaso.gabrieli.1@studenti.unipd.it)
*/

#ifndef PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGTOOLKIT_HPP
#define PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGTOOLKIT_HPP


#include <iostream>
#include <list>
#include <regex>
#include <random>
#include <fstream>

//Gets timestamp from system
inline int unix_time() {
    return time(nullptr);
}

//Converts your timestamp to human-friendly date
inline std::string get_hour_time() {
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

// Method that creates a name for the play log file
inline std::string create_log_name() {
    const auto rawtime = (const time_t)unix_time();

    struct tm * dt;
    char timestr[40];
    char buffer [40];

    dt = localtime(&rawtime);
    strftime(timestr, sizeof(timestr), "LogFile_%d-%m-%Y_%H:%M:%S.log", dt);
    sprintf(buffer,"%s", timestr);
    std::string stdBuffer(buffer);
    return stdBuffer;
}

//Creates a unique name for a log cou on file
inline std::string create_replay_output_name() {

    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> rand_name(1234567, 999999999);

    return "Replay_"+ std::to_string(rand_name(engine))+".txt";
}

// Creates log file

inline std::ofstream create_file() {
    std::ofstream test;
    test.open(create_log_name());
    return test;
}

#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGTOOLKIT_HPP
