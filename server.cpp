#include<iostream>
#include<memory>
#include<string>
#include<functional>

#include "src/utils.h"
#include "src/Socket.h"
#include "src/Epoll.h"
#include "src/EventLoop.h"
#include "src/Event.h"
#include "src/Acceptor.h"
using namespace std;


string addr = "0.0.0.0";
uint16_t port = 8888;

void test(){
    auto i = inet_addr(Domain::IPv4, addr.c_str(), port);
}

int main(){
    cout << __cplusplus << endl;
    Epoll *epoll = new Epoll(5);
    EventLoop * loop = new EventLoop(epoll);
    Acceptor *acceptor = new Acceptor(loop, addr, port);

    auto cb = bind(accept_connection_handler, placeholders::_1, epoll);
    acceptor->set_connection_handler(cb);
    
    loop->run();

    delete acceptor;
    delete epoll;
    delete loop;
    return 0;
}
