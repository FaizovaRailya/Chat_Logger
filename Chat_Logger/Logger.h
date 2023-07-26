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
	std::shared_mutex shared_mutex; //������� ��� ������ ������ � ���� 
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
//	std::string _path;		/**< ���� �� ����� */
//	std::fstream _log;		/**< ������ ������ ofstream, � ������� ������ ���� ������� */
//	
//	std::shared_mutex shared_mutex;		/**< ������� ��� ������ ������ � ���� */
//	/** �������� ����� � �������� ���������. */
//	void open();
//	/** �������� ����� � �������� ���������. */
//	void close();
//public:
//	/**	* ����������� � �����������. ��� �������� ������� � ��������� � �����.
//	* @param path - ���� �� �����;
//	* @param language - ���� ���������.	*/
//	Logger(const std::string& path);
//	/** ���������� */
//	~Logger();
//
//	/*** ������ ��������� � ������.
//	* @param message - ��������� ��� ������ � ����.	*/
//	void operator << (const std::string& message);
//
//	/*** ������ ������ �� �������.
//	* @param - ������, ��������� �� �����.	*/
//	void operator >> (std::string& line);
//};
