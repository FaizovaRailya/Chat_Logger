#pragma once
#include <fstream>
#include <shared_mutex>
#include <string>
#pragma once
#include "message.h"
#include <string>
#include <ctime>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <shared_mutex>

class Logger {
private:
	std::shared_mutex shared_mutex; //мьютекс для защиты записи в файл 
	//void open();
	//void close();
public:
	std::fstream logfile = std::fstream("log.txt", std::ios::in | std::ios::out);
	Logger();
	~Logger();

	//friend std::fstream& operator >>(std::fstream& is, Message& _msg);
	//friend std::ostream& operator <<(std::ostream& os, const Message& _msg);
	void write_log(Message& _msg);
	std::string read_log();
};

//class Logger {
//private:
//	std::string _path;		/**< путь до файла */
//	std::fstream _log;		/**< объект класса ofstream, с которым связан файл журнала */
//	
//	std::shared_mutex shared_mutex;		/**< мьютекс для защиты записи в файл */
//	/** Открытие файла с журналом сообщений. */
//	void open();
//	/** Закрытие файла с журналом сообщений. */
//	void close();
//public:
//	/**	* Конструктор с параметрами. Для создания объекта с привязкой к файлу.
//	* @param path - путь до файла;
//	* @param language - язык сообщений.	*/
//	Logger(const std::string& path);
//	/** Деструктор */
//	~Logger();
//
//	/*** Запись сообщений в журнал.
//	* @param message - сообщение для записи в файл.	*/
//	void operator << (const std::string& message);
//
//	/*** Чтение строки из журнала.
//	* @param - строка, считанная из файла.	*/
//	void operator >> (std::string& line);
//};
