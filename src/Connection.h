# pragma once

#include<iostream>
#include<functional>
#include<map>

#include "Event.h"
#include "EventLoop.h"

using namespace std;

class Connection
{
    static map<int, Connection*> _connections_map;
    Event* _event = nullptr;
    EventLoop* _loop = nullptr;
    int _fd = -1;
    function<void(Connection&)> _connection_clallback = nullptr;

public:
    Connection(EventLoop* Loop, int fd);
    ~Connection();
    void handle_connection();
    void disconnect();
    void set_connection_callback(function<void(Connection&)>);
    int get_fd(){ return _fd; }
};
