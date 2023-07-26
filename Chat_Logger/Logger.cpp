#include "Logger.h"
#include <string>
#include <ctime>
#include <chrono>
#pragma warning(disable : 4996)

Logger::Logger() {
    //предполагаем что файл существует
    logfile.open("log.txt", std::ios::in | std::ios::out);
    if (!logfile) {
        //создаём файл
        logfile = std::fstream("log.txt", std::ios::in | std::ios::out | std::ios::trunc);
        std::cout << "Создан файл журнала" << std::endl;
    }
    if (logfile) {
        std::cout << "Файл журнала уже создан" << std::endl;
        logfile.seekg(0, std::ios_base::end);
    }
    else {
        std::cout << "Не удалось создать или открыть файл log.txt !" << std::endl;
    }
}

Logger::~Logger() {
    logfile.close();
}

void Logger::write_log(Message& _msg) {
    shared_mutex.lock();
    char* time_str;
    time_t x = _msg.getTime();
    time_str = ctime(&x);
    int time_str_cut = strlen(time_str);
    time_str[--time_str_cut] = '\0';
    //std::cout << "write log!" << std::endl;
    logfile.seekp(0, std::ios_base::end);
    //os << std::endl;
    logfile << "(";
    logfile << time_str;
    logfile << ") ";
    logfile << " от: ";
    logfile << _msg.getFromMessage();
    logfile << " кому: ";
    logfile << _msg.getToMessage();
    logfile << " текст: ";
    logfile << _msg.getText();
    logfile << std::endl;
    shared_mutex.unlock();
}

void Logger::toLog(const std::string& text) {
    shared_mutex.lock();
    std::fstream out("log.txt", std::ios::app);
    if (out.is_open())
        out << text << std::endl;
    shared_mutex.unlock();
}

std::string Logger::read_log() {
    shared_mutex.lock_shared();
    int cursor;
    cursor = logfile.tellg();
    if (!cursor) {
        shared_mutex.unlock_shared();
        return "File empty!";
    }
    else {
        char ch;
        int flag = 0;
        while (true) {
            logfile.seekg(-1, std::ios::cur);
            cursor = logfile.tellg();
            if (!cursor) break;
            ch = logfile.peek();
            if (ch == '\n' && flag != 0) {
                logfile.seekg(2, std::ios::cur);
                break;
            }
            flag = 1;
        }
        std::string last_str;
        getline(logfile, last_str);
        
        shared_mutex.unlock_shared();
        return last_str;
    }
}
