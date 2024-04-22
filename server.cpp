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

#include "utils.h"
#include "Socket.h"
#include "Epoll.h"
using namespace std;


const char * addr = "0.0.0.0";
uint16_t port = 8888;

void et_read_event_handler(int read_fd){
    while(true){
        char buff[1024];
        memset(buff, 0, sizeof(buff));
        ssize_t read_bytes = recv(read_fd, buff, sizeof(buff), 0);
        if (read_bytes > 0){
            cout <<"get message from client : "<< buff <<endl;
            send(read_fd, buff, sizeof(buff), 0);
        } else if (read_bytes == -1 && errno == EINTR){
            // client normal interrupt;
            cout <<"interrupted "<< buff <<endl;
            continue; 
        } else if (read_bytes == -1 &&(errno == EAGAIN || errno == EWOULDBLOCK)){
            // data is all recived 
            cout <<"blocked "<< buff <<endl;
            break;
        } else if (read_bytes == 0){
            cout << "End of file" << endl;
            close(read_fd);
            break;
        }
    }
}

int main(){
    Socket *server = new Socket(Domain::IPv4, Type::TCP);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(addr);
    serv_addr.sin_port = htons(port);
    server->bind(serv_addr);
    server->listen();
    
    Epoll epoll = Epoll(5);

    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET; // set epoll event to read and edge-triggered
    ev.data.fd = server->get_fd();


    epoll.ctl_add(server->get_fd(), &ev);


    while(true)
    {

        int event_num = epoll.wait(1024);
        for(int i = 0; i < event_num; i++)
        {
            if(epoll.events[i].data.fd == server->get_fd()) // if the event is from socket_fd, it means there is a new client
            {
                cout << "new client connected" << endl;
                struct sockaddr_in clit_addr;
                socklen_t clit_sock_len = sizeof(clit_addr);
                memset(&clit_addr, 0, clit_sock_len);

                int clit_socket_fd = server->accept(clit_addr);
                set_non_blocking(clit_socket_fd);

                struct epoll_event ev_c;
                ev_c.events = EPOLLIN | EPOLLET; // set epoll event to read and edge-triggered
                ev_c.data.fd = clit_socket_fd;

                epoll.ctl_add(clit_socket_fd, &ev_c);
                cout << "add client" << endl;

            }else if (epoll.events[i].events == EPOLLIN){
                et_read_event_handler(epoll.events[i].data.fd);
            }
        }
        
    }
    delete server;
    return 0;
}