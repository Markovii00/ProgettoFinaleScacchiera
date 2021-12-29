//
// Created by Tommaso Gabrieli on 21/12/21.
//

#ifndef PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGTOOLKIT_HPP
#define PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGTOOLKIT_HPP

#include <iostream>
#include <chrono>
#include <random>

inline int unix_time() {
    return time(NULL);
}

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

inline std::string get_date() {
    const auto rawtime = (const time_t)unix_time();

    struct tm * dt;
    char timestr[30];
    char buffer [30];

    dt = localtime(&rawtime);
    // use any strftime format spec here
    strftime(timestr, sizeof(timestr), "%d/%m/%Y - %H:%M:%S", dt);
    sprintf(buffer,"%s", timestr);
    std::string stdBuffer(buffer);
    return stdBuffer;
}

inline std::string create_log_name() {
    const auto rawtime = (const time_t)unix_time();

    struct tm * dt;
    char timestr[40];
    char buffer [40];

    dt = localtime(&rawtime);
    // use any strftime format spec here
    strftime(timestr, sizeof(timestr), "LogFile_%d-%m-%Y_%H:%M:%S.txt", dt);
    sprintf(buffer,"%s", timestr);
    std::string stdBuffer(buffer);
    return stdBuffer;
}

inline std::ofstream create_file() {
    std::ofstream test;
    test.open(create_log_name());
    return test;
}

inline std::string generate_log_id() {
    std::string s; //six spaces
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> dist(0, 9);

    for (int i = 0; i < 6; ++i) {
        s += std::to_string(dist(engine));
    }

    return s;
}



#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGTOOLKIT_HPP
