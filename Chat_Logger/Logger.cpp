#include "Logger.h"
#include <string>
#include <ctime>
#include <chrono>
#pragma warning(disable : 4996)

Logger::Logger() {
    if (!logfile) {
        logfile = std::fstream("log.txt", std::ios::in | std::ios::out | std::ios::trunc);
        std::cout << "logfile created" << std::endl;
    }
    if (logfile) {
        std::cout << "logfile already created" << std::endl;
        logfile.seekg(0, std::ios_base::end);
    }
    else {
        std::cout << "Could not create or open file log.txt!" << std::endl;
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

//#include "Logger.h"
//#include <chrono>
////#include <ctime>   // localtime
//#include <iomanip> // put_times
//#include <iostream>
//
//Logger::Logger(const std::string& path) : _path(path) {
//	open();
//}
//
//void Logger::open() {
//	// Открываем файл для чтения и записи сообщений.
//	// Новые сообщения будут дописаны в конец файла.
//	// Попытка 1. Предполагаем, что файл существует.
//	_log.open("log.txt", std::ios::out | std::ios::in | std::ios::ate);
//	if (_log) {
//		std::string logMessage;
//		
//		logMessage = "Открыт файл: log.txt";
//		*this << logMessage;
//	}
//	else {
//		// Файл не найден. Открываем новый файл.
//		_log.open("log.txt", std::ios::out);
//		if (_log) {
//			std::string logMessage;
//			logMessage = "Открыт файл: log.txt";
//			*this << logMessage;
//		}
//		else {
//#ifdef _DEBUG		
//			std::cout << "Ошибка открытия файла: log.txt";
//#endif
//		}
//	}
//}
//void Logger::close() {
//	std::string logMessage;
//	logMessage = "Закрыт файл: log.txt" + _path;
//	*this << logMessage;
//	_log.close();
//}
//Logger::~Logger() { close(); }
//void Logger::operator << (const std::string& message) {
//	shared_mutex.lock();
//	/** Проверяем, доступен ли файл */
//	if (_log.is_open()) {
//		_log.clear();;
//		/** Получаем текущее время в формате "год.месяц.дата часы:минуты:секунды". */
//		auto timePoint = std::chrono::system_clock::now();
//		auto timeT = std::chrono::system_clock::to_time_t(timePoint);
//		auto timeObject = std::put_time(std::localtime(&timeT), "%Y-%m-%d %X");
//		/** Печатаем сообщение с датой и временем в журнал. */
//		_log << "[" << timeObject << "] - " << message << std::endl;
//#ifdef _DEBUG
//		std::cout << "[" << timeObject << "] - " << message << std::endl;
//#endif
//		_log.flush();
//	}
//	shared_mutex.unlock();
//}
//void Logger::operator >> (std::string& line) {
//	// Читаем сообщение из файла, после чего запоминаем положение каретки.
//	static std::streamoff position = 0;
//	line.clear();
//	shared_mutex.lock();
//	if (_log.is_open() && position != -1) {
//		_log.seekg(position);
//		getline(_log, line);
//		position = _log.tellg();
//		shared_mutex.unlock();
//		return;
//	}
//	// Тест. Считаем, что файл считывается от начала до конца.
//	// Сбрасываем положение каретки, чтобы в файл можно было записывать инофрмацию.
//	if (_log.is_open() && position == -1) {
//		_log.seekg(0);
//	}
//	shared_mutex.unlock();
//}
