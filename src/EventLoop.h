#pragma once
#include<iostream>
#include"Epoll.h"
#include"Socket.h"
#include"utils.h"
#include"Event.h"

using namespace std;


class EventLoop
{
    Epoll* _epoll;
    Socket* _socket;

public:
    EventLoop(Epoll *epoll, Socket *_socket);
    ~EventLoop();
    void run();
};

void et_read_event_handler(int read_fd);
void accept_connection_handler(Socket* socket, Epoll* epoll);