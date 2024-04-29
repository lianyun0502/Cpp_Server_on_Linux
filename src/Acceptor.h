#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <memory>


#include "utils.h"
#include "Socket.h" // 不放在 utils.h 後面會有編譯錯誤error: ‘unique_ptr’ does not name a type，原因不明
#include "Event.h"
#include "EventLoop.h"


using namespace std;

class Acceptor 
{
    Socket* _sock;
    Event* _event; // connection accpet event
    EventLoop* _loop; 
    function<void (Socket*)> new_connection_callback;

public:
    Acceptor(EventLoop* ev_lp, string& addr, u_int16_t port, Domain domain=Domain::IPv4, Type type = Type::TCP);
    ~Acceptor();
    void set_connection_handler(function<void (Socket*)> connection_handler);
    void new_connection();
};


void et_read_event_handler(int read_fd);
void accept_connection_handler(Socket* socket, Epoll* epoll);

