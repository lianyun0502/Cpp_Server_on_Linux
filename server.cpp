#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<assert.h>
#include<errno.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<memory>

#include "src/utils.h"
#include "src/Socket.h"
#include "src/Epoll.h"
#include "src/EventLoop.h"
#include "src/Event.h"
using namespace std;


const char * addr = "0.0.0.0";
uint16_t port = 8888;

int main(){
    cout << __cplusplus << endl;
    Socket *server = new Socket(Domain::IPv4, Type::TCP);
    auto serv_addr = inet_addr(Domain::IPv4, addr, port);
    server->bind(*serv_addr.get());
    server->listen();
    
    Epoll *epoll = new Epoll(5);
    // auto ev = gen_epoll_event(server->get_fd(), EPOLLIN | EPOLLET);
    Event *event = new Event(epoll, server->get_fd(), EPOLLIN | EPOLLET);
    auto ev = event->gen_epoll_event();
    auto cb = bind(accept_connection_handler, server, epoll);
    event->set_callback(cb);
    epoll->ctl_add(server->get_fd(), ev.get());

    EventLoop * loop = new EventLoop(epoll, server);
    loop->run();
   
    delete event;
    delete server;
    delete epoll;
    delete loop;
    return 0;
}