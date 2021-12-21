//
// Created by Tommaso Gabrieli on 21/12/21.
//

#ifndef PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGTOOLKIT_HPP
#define PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGTOOLKIT_HPP

#include <iostream>
#include <ctime>
#include <chrono>

int unix_time() {
    return time(NULL);
}

std::string get_hour_time() {
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

std::string get_date() {
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

std::string create_log_name() {
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



#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_LOGTOOLKIT_HPP