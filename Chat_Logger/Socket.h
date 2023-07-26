#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <stdlib.h>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#define MESSAGE_LENGTH 1024     // ������������ ������ ������ ��� ������
#define PORT 7777               // ����� ������������ ���� ����� �����

class Socket_server {
private:
    struct sockaddr_in tcpserveraddress, tcpclient;
    char message[MESSAGE_LENGTH];

    SOCKET socket_id = INVALID_SOCKET;
    SOCKET accept_socket = INVALID_SOCKET;

public:
    Socket_server();
    ~Socket_server();

    bool listen_socket();
    char* receive_data();
    int send_data(char* msg);
};

class Socket_client {
private:
    struct sockaddr_in tcpserveraddress, tcpclient;
    char message[MESSAGE_LENGTH]{};
    SOCKET socket_id = INVALID_SOCKET;

public:
    Socket_client();
    ~Socket_client();

    char* receive_data();
    int send_data(char* msg);
};

