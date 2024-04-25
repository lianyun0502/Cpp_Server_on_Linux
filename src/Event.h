#pragma once

#include<iostream>
#include<functional>
#include<memory>
#include<sys/epoll.h>

class Epoll;

using namespace std;

class Event
{
    Epoll* _ep = nullptr; //記錄這是屬於哪個epoll
    int _fd = -1;
    uint32_t _events = 0;
    function<void()> _callback;
public:
    Event(Epoll *epoll, int fd, uint32_t events);
    ~Event();
    void run_event_handle();
    int get_fd();
    void set_callback(function<void()> callback);
    unique_ptr<struct epoll_event> gen_epoll_event();
};

