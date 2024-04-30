#include "Acceptor.h"

#include <string>
#include <functional>
#include <memory>

using namespace std;

Acceptor::Acceptor(EventLoop* ev_lp, string& addr, u_int16_t port, Domain domain, Type type): _loop(ev_lp)
{
    cout << "Acceptor created" << endl;
    _sock = new Socket(domain, type);
    auto serv_addr = inet_addr(domain, addr.c_str(), port);
    _sock->bind(*serv_addr.get());
    _sock->listen();

    _event = new Event(_sock->get_fd(), EPOLLIN | EPOLLET);
    _loop->add_event(*_event);
    auto cb = bind(&Acceptor::new_connection, this);
    _event->set_callback(cb);

}
Acceptor::~Acceptor()
{
    delete _sock;
    delete _event;
    ::close(_sock->get_fd());
    cout << "Acceptor destroyed" << endl;
}

void Acceptor::set_connection_handler(function<void (Socket*)> connection_handler)
{
    new_connection_callback = connection_handler;
}

void Acceptor::new_connection()
{
    struct sockaddr_in clit_addr;
    socklen_t clit_sock_len = sizeof(clit_addr);
    memset(&clit_addr, 0, clit_sock_len);
    int clit_socket_fd = _sock->accept(clit_addr);
    set_non_blocking(clit_socket_fd);
    new_connection_callback(_sock);
}