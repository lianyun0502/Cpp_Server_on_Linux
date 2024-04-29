#include "Acceptor.h"

#include <string>
#include <functional>
#include <memory>

using namespace std;


void et_read_event_handler(int read_fd){
    while(true){
        char buff[1024];
        memset(buff, 0, sizeof(buff));
        ssize_t read_bytes = recv(read_fd, buff, sizeof(buff), 0);
        // printf("read_bytes: %ld\n", read_bytes);
        if (read_bytes > 0){
            cout <<"get message from client : "<< buff <<";"<<endl;
            send(read_fd, buff, sizeof(buff), 0);
        } else if (read_bytes == -1 && errno == EINTR){
            // client normal interrupt;
            cout <<"interrupted "<< buff <<endl;
            continue; 
        } else if (read_bytes == -1 &&(errno == EAGAIN || errno == EWOULDBLOCK)){
            // data is all recived 
            cout <<"message end! "<< buff <<endl;
            break;
        } else if (read_bytes == 0){
            cout << "End of file" << endl;
            close(read_fd);
            break;
        }
    }
}
void accept_connection_handler(Socket* socket, Epoll *epoll){
    cout << "new client connected" << endl;
    struct sockaddr_in clit_addr;
    socklen_t clit_sock_len = sizeof(clit_addr);
    memset(&clit_addr, 0, clit_sock_len);

    int clit_socket_fd = socket->accept(clit_addr);
    set_non_blocking(clit_socket_fd);

    // auto ev_c = gen_epoll_event(clit_socket_fd, EPOLLIN | EPOLLET);
    Event* e = new Event(epoll, clit_socket_fd, EPOLLIN | EPOLLET); // 有內存洩漏風險
    auto ev_c = e->gen_epoll_event();
    auto cb = bind(et_read_event_handler, clit_socket_fd);
    e->set_callback(cb);
    epoll->ctl_add(clit_socket_fd, ev_c.get());
    cout << "add client" << endl;
}

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
    new_connection_callback(_sock);
}