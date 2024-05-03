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

public:
    EventLoop(Epoll *epoll=nullptr);
    ~EventLoop();
    void run();
    void add_event(Event& event);
    void remove_event(Event& event);
    void modify_event(Event& event);
};

// void et_read_event_handler(int read_fd);
// void accept_connection_handler(Socket* socket, Epoll* epoll);