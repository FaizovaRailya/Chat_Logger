#pragma once
#include "message.h"
#include <string>
#include <ctime>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <shared_mutex>

class Logger {
	std::shared_mutex shared_mutex;
	std::fstream logfile;
public:
	Logger();
	~Logger();
	void write_log(Message& _msg);
	std::string read_log();
	void toLog(const std::string& text);
	std::string fromLog();
};

