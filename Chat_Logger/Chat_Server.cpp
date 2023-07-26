#include <iostream>
#include<string>
#include <thread>
#include <ctime>
#include <chrono>
//#include <exception>
//#include <sstream>
#include "Chat_Server.h"
#include "Sha1.h"

#pragma warning(disable : 4996)


void Chat_Server::Menu() {
	bool f = true;
	while (f) {
		sock.send_data((char*)"\n------������� ��������:------\n1 - ����\n2 - �����������\n3 - ������ ��������� ������ �������\n0 - �����\n");
		sock.send_data((char*)"end_receive");
		char* choise = sock.receive_data();
		switch (choise[0]) {
		case '1':
			SignIn();			//���� �� ������ � ������
			log.toLog("������������ �����");
			break;
		case '2':				// ����������� ������ ������������
			try {
				NewUser();		//�������� ����� ���������� ������ ������������			
			}
			catch (std::exception& e) {
				std::cout << e.what() << std::endl;
			}
			break;
		case '3': {
			std::thread read(&Chat_Server::log_reader, this);
			read.join();
			break; }
		case '0':
			sock.send_data((char*)"Exit");
			sock.send_data((char*)"end_chat");
			f = sock.listen_socket();
			f = false;
			break;
		default:
			sock.send_data((char*)"�������� ����!\n");
			break;
		}
	}
	std::cout << "Chat session terminated! See you again!" << std::endl;
}

void Chat_Server::SignIn() {					//���� �� ������ � ������
	std::string log, pas;
	sock.send_data((char*)"----�������----\n�����: ");
	sock.send_data((char*)"end_receive");
	log = sock.receive_data();

	sock.send_data((char*)"������: ");
	sock.send_data((char*)"end_receive");
	pas = sock.receive_data();
	
	if (UserSearch(log, pas)) {
		bool f1 = true;
		while (f1) {
			sock.send_data((char*)"\n------������� �������e:------\n1 - ��������� ��������e\n2 - �������� ���������\n0 - �����\n");

			sock.send_data((char*)"end_receive");
			char* b = sock.receive_data();
			switch (b[0]) {
			case '1':
				ShowMessage();			//��������� ���������	
				break;
			case '2':				//�������� ���������
				setAddMessage();
				break;
			case '0':				//�����
				
				f1 = false;
				
				break;
			default:
				sock.send_data((char*)"�������� ����!\n");
				break;
			}
		}
	}
	
	else 
		sock.send_data((char*)"\n�������� ����� ��� ������!!!\n");
}

void Chat_Server::ShowMessage() {			//��������� ���������
	sock.send_data((char*)"\n------������� �������e:------\n1 - ������ �� �����������\n2 - ������ \n3 - �����\n0 - �����\n");
	sock.send_data((char*)"end_receive");
	char* d = sock.receive_data();

	switch (d[0]) {
	case '1':
		setUnreadPrivateShowChat();   //�������� ����� ������ �� ����������� ������ ���������  
		break;
	case '2':
		setPrivateShowChat();	      //�������� ����� ������ ������ ��������� 
		break;
	case '3':
		setAllShowChat();	      //�������� ����� ������ ����� ���������
		break;
	case '0':
		break;
	default:
		sock.send_data((char*)"�������� ����!\n");
		break;
	}
}

bool Chat_Server::FindLogin(const std::string& login) {								//����� �������� ������
	db.mysql_start();
	std::string log("SELECT login FROM user_spisok WHERE login = \"" + login + "\"");
	mysql_query(&db.mysql, log.c_str()); //������ ������ � �������

	if (db.res = mysql_store_result(&db.mysql)) {
		if (mysql_num_rows(db.res) == 0)
			mysql_close(&db.mysql);
			return false;
	}
	mysql_close(&db.mysql);
	return true;
	
}

void Chat_Server::NewUser() {					//����� �������� ������ ������������
	std::string name, login, password;

	sock.send_data((char*)"---- ������� ������ ��� ����������� ----\n���: ");
	bool n = true;
	while (n) {
		sock.send_data((char*)"end_receive");
		name = (char*)sock.receive_data();
		std::cout << name << std::endl;
		if (name == "all")
			sock.send_data((char*)"������ ��� ��� ������ �������� ������!\n������� ���: ");

		else n = false;
	}
	bool l = true;
	while (l) {
		sock.send_data((char*)"�����: ");
		sock.send_data((char*)"end_receive");
		login = sock.receive_data();
		std::cout << login << std::endl;
		if (login == "all" || FindLogin(login))
			sock.send_data((char*)"������ ����� ��� ����� �������� ������!\n������� �����: ");
		else l = false;
	}
	sock.send_data((char*)"������: ");
	sock.send_data((char*)"end_receive");
	password = sock.receive_data();

	db.mysql_start();
	unsigned int* _password = new unsigned int[5];
	std::string _pw_str[5];
	_password = sha1((char*)(password.data()), password.length());
	for (size_t i = 0; i < 5; i++) {
		_pw_str[i] = std::to_string(_password[i]);
	}

	std::string pass(_pw_str[0] + " " + _pw_str[1].c_str() + " " + _pw_str[2] + " " + _pw_str[3] + " " + _pw_str[4]);
	std::string usr_query("INSERT INTO user_spisok(id, name, login, password) VALUES (default, \"" + name + "\", \"" + login + "\", \"" + pass + "\")");
	mysql_query(&db.mysql, usr_query.c_str());
	User user(name, login, sha1((char*)password.data(), password.length()));				//������� ������ ����� User
	UserSpisok.push_back(user);						// ��������� ������������ � ������
	currentUsers = std::make_shared<User>(user);		//������� ��������� �� �������� ������������
	sock.send_data((char*)"������������ ���������������!\n\n");

	mysql_close(&db.mysql);
	log.toLog("������ ����� ������������");
	
}

bool Chat_Server::UserSearch(const std::string& login, const std::string& password) {	//����� ������ ������������ �� ������ � ������	
	db.mysql_start();
	std::string log("SELECT * FROM user_spisok WHERE login = \"" + login + "\"");
	mysql_query(&db.mysql, log.c_str()); //������ ������ � �������

	unsigned int* _password = new unsigned int[5];
	std::string _pw_str[5];
	_password = sha1((char*)(password.data()), password.length());
	for (size_t i = 0; i < 5; i++) {
		_pw_str[i] = std::to_string(_password[i]);
	}
	std::string pass(_pw_str[0] + " " + _pw_str[1].c_str() + " " + _pw_str[2] + " " + _pw_str[3] + " " + _pw_str[4]);

	if (db.res = mysql_store_result(&db.mysql)) {
		while (db.row = mysql_fetch_row(db.res)) {
			for (size_t i = 0; i < mysql_num_fields(db.res); i++) {
				if (db.row[3] == pass) {
					currentUser = db.row[1];    //��������� �� �������� ������������
					id_currentUser = db.row[0]; // id �������� ������������
					sock.send_data((char*)"\n������������: ");
					sock.send_data((char*)currentUser);
					mysql_close(&db.mysql);	
					return true;
				}
			}
		}
	}
	mysql_close(&db.mysql);
	return false;
}

void Chat_Server::PrintNamesUsers() {				    //����� ��������� ������ ������������������ �������������
	db.mysql_start();
	mysql_query(&db.mysql, "SELECT name FROM user_spisok WHERE name != 'all'"); //������ ������ � �������
	if (db.res = mysql_store_result(&db.mysql)) {
		while (db.row = mysql_fetch_row(db.res)) {
			for (size_t i = 0; i < mysql_num_fields(db.res); i++) {
				sock.send_data(db.row[i]);
			}
			sock.send_data((char*)"\n");
		}
	}
	mysql_close(&db.mysql);
}

bool Chat_Server::FindUserinUserSpisok(std::string& name) {	//����� ��������� ��������� �� ������� ���	
	db.mysql_start();
	std::string n("SELECT name FROM user_spisok WHERE name = \"" + name + "\"");
	mysql_query(&db.mysql, n.c_str()); //������ ������ � �������
	if (db.res = mysql_store_result(&db.mysql)) {
		if (mysql_num_rows(db.res) == 0) {
			mysql_close(&db.mysql);
			return false;
		}
		else {
			mysql_close(&db.mysql);
			return true;
		}		
	}
}

void Chat_Server::setUnreadPrivateShowChat() {			//����� ������ ������������� ������ ���������
	db.mysql_start();
	std::string id_to = id_currentUser;
	sock.send_data((char*)"--------------���--------------\n");
	std::string mess("SELECT name, text FROM message JOIN user_spisok ON message.id_from_name = user_spisok.id WHERE status = '0' AND id_to_name = \"" + id_to + "\"");
	mysql_query(&db.mysql, mess.c_str()); //������ ������ � �������

	if (db.res = mysql_store_result(&db.mysql)) {
		if (mysql_num_rows(db.res) == 0) {
			sock.send_data((char*)"� ��� ��� �� ����������� ���������!\n");
		}
		else {
			while (db.row = mysql_fetch_row(db.res)) {
				for (size_t i = 1; i < mysql_num_fields(db.res); ++i) {
					sock.send_data((char*)"�� ");
					sock.send_data(db.row[0]);
					sock.send_data((char*)": ");
					sock.send_data(db.row[1]);
					sock.send_data((char*)"\n");
				}
			}
		}
	}
	std::string mes("UPDATE message JOIN user_spisok ON message.id_from_name = user_spisok.id SET status = '1' WHERE status = '0' AND id_to_name = \"" + id_to + "\"");
	mysql_query(&db.mysql, mes.c_str()); //������ ������ � �������

	sock.send_data((char*)"-------------------------------");
	mysql_close(&db.mysql);
}

void Chat_Server::setPrivateShowChat() {				//����� ������ ������ ���������
	db.mysql_start();
	std::string id_to = id_currentUser;
	sock.send_data((char*)"--------------���--------------\n");
	std::string mesp("SELECT name, text FROM message RIGHT JOIN user_spisok on message.id_from_name = user_spisok.id WHERE id_to_name = \"" + id_to + "\" OR id_from_name = \"" + id_to + "\" ORDER BY data");
	mysql_query(&db.mysql, mesp.c_str()); //������ ������ � �������
	if (db.res = mysql_store_result(&db.mysql)) {
		if (mysql_num_rows(db.res) == 0) {
			sock.send_data((char*)"� ��� ��� ���������!\n");
		}
		else {
			while (db.row = mysql_fetch_row(db.res)) {
				for (size_t i = 1; i < mysql_num_fields(db.res); ++i) {
					sock.send_data((char*)"�� ");
					sock.send_data(db.row[0]);
					sock.send_data((char*)": ");
					sock.send_data(db.row[1]);
					sock.send_data((char*)"\n");
				}
			}
		}
	}
	std::string mes("UPDATE message JOIN user_spisok ON message.id_from_name = user_spisok.id SET status = '1' WHERE id_to_name = \"" + id_to + "\"");
	mysql_query(&db.mysql, mes.c_str()); //������ ������ � �������

	sock.send_data((char*)"-------------------------------");
	mysql_close(&db.mysql);
}

void Chat_Server::setAllShowChat() {							// ����� ������ ����� ���������
	db.mysql_start();
	std::string id_to = id_currentUser;
	sock.send_data((char*)"------------����� ���------------\n");
	mysql_query(&db.mysql, "SELECT name, text FROM message join user_spisok on message.id_from_name = user_spisok.id WHERE status = '0' AND id_to_name = '3'"); //������ ������ � �������
	if (db.res = mysql_store_result(&db.mysql)) {
		while (db.row = mysql_fetch_row(db.res)) {
			for (size_t i = 1; i < mysql_num_fields(db.res); ++i) {
				sock.send_data((char*)"�� ");

				sock.send_data(db.row[0]);
				sock.send_data((char*)": ");
				sock.send_data(db.row[1]);
				sock.send_data((char*)"\n");
			}
		}
	}
	sock.send_data((char*)"-------------------------------");
	mysql_close(&db.mysql);
}

void Chat_Server::setAddMessage() {						    	//����� ���������� ��������� � ��
	std::string id_from, id_to;
	std::chrono::system_clock::time_point value_t = std::chrono::system_clock::now();
	time_t timestamp = std::chrono::system_clock::to_time_t(value_t);
	const tm* ltm = std::localtime(&timestamp);

	char mbstr[15];
	std::strftime(mbstr, 15, "%Y-%m-%d", ltm);
	std::cout << mbstr << std::endl;

	sock.send_data((char*)"\n������� ��� ���� ��������� ���������:\n");
	PrintNamesUsers();									  //������� ������ �������������
	sock.send_data((char*)"all - ��������� ����\n");
	sock.send_data((char*)"end_receive");
	std::string inputName = sock.receive_data();

	id_from = id_currentUser;
	
	if (!FindUserinUserSpisok(inputName)) {
		sock.send_data((char*)"������������ � ������ ������ �� ������\n");
	}
	else {
		sock.send_data((char*)"\n������� ����� ���������: \n");
		sock.send_data((char*)"end_receive");
		std::string text = sock.receive_data();

		db.mysql_start();
		if (inputName == "all") {							  //�������� ���� �������������
			std::string mesAll("INSERT INTO message(id, id_from_name, id_to_name, data, status, text) VALUES (default, \"" + id_from + "\", '3', \"" + mbstr + "\", '0', \"" + text + "\")");
			mysql_query(&db.mysql, mesAll.c_str()); //������ ������ � �������
			mysql_close(&db.mysql);
			messageList.push_back(Message(currentUser, "all", text, timestamp));
			for (std::vector<Message>::iterator it = messageList.begin(); it != messageList.end(); ++it) {
				std::cout << it->getFromMessage() << std::endl;
			}
			
			sock.send_data((char*)"��������� ��������� ���� �������������!\n");
		}
		else {													//�������� ������ ���������
		std:string id_t("SELECT id FROM user_spisok WHERE name = \"" + inputName + "\" ");
			mysql_query(&db.mysql, id_t.c_str()); //������ ������ � �������
			if (db.res = mysql_store_result(&db.mysql)) {
				while (db.row = mysql_fetch_row(db.res)) {
					for (size_t i = 0; i < mysql_num_fields(db.res); i++) {
						id_to = db.row[i];
					}
				}
			}
			std::string mes("INSERT INTO message(id, id_from_name, id_to_name, data, status, text) VALUES (default, \"" + id_from + "\", \"" + id_to + "\", \"" + mbstr + "\", '0', \"" + text + "\")");
			mysql_query(&db.mysql, mes.c_str()); //������ ������ � �������	
			Message msg(currentUser, inputName, text, timestamp);
			/*std::thread f(log_writer(), msg);
			f.join();*/
			log.write_log(msg);
			messageList.push_back(msg);
			
		}
		mysql_close(&db.mysql);
	}
}
void Chat_Server::log_writer(Message& _msg){
	log.write_log(_msg);
}
void Chat_Server::log_reader(){
	sock.send_data((char*)log.read_log().data());
}

