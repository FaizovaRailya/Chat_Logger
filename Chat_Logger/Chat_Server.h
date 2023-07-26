#pragma once
#include "User.h"
#include "Message.h"
#include "Socket.h"
#include "Mysql_Chat.h"
#include "Logger.h"
#include <vector>
#include <exception>
#include <memory>

class Chat_Server {
	std::vector<User> UserSpisok;				 //������ ������������������ �������������
	std::vector<Message> messageList;			 //������� ������, ������� ������ ��������� � ����
	std::shared_ptr<User> currentUsers = nullptr; //��������� ��������� �� �������� ������������

	Socket_server sock;
	Mysql_Chat db;

public:
	Chat_Server() = default;
	
	~Chat_Server() = default;

	char* currentUser = nullptr;
	char* id_currentUser = nullptr;

	void Menu();
	void SignIn();								  //���� �� ������ � ������
	void ShowMessage();							  //���� �� ������ � ������
	bool FindLogin(const std::string& login);				  //�������� ������
	void NewUser();			    					  //�������� ������ ������������
	bool UserSearch(const std::string& login, const std::string& password);   //����� ������������ �� ������ � ������
	void PrintNamesUsers();							  //��������� ������ ������������������ �������������
	bool FindUserinUserSpisok(std::string& name);			  //�������� ��������� �� ������� ���
	void setUnreadPrivateShowChat();					  //������ �� ����������� ������ ���������
	void setPrivateShowChat();						  //������ ������ ���������
	void setAllShowChat();							  //������ ����� ���������
	void setAddMessage();							  //���������� ���������
	void log_writer(Message& _msg);
	void log_reader();
	Logger _log;
};




