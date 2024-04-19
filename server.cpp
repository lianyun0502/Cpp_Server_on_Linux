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
using namespace std;


const char * addr = "0.0.0.0";
uint16_t port = 8888;


int main(){
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(addr);
    serv_addr.sin_port = htons(port);

    
    cout << "server start at " << addr << ":" << port << endl;
    cout << "binding..." << endl;
    int bind_status = bind(socket_fd, (const sockaddr*)&serv_addr, sizeof(serv_addr));
    PrintError(bind_status == -1, "bind failed");

    cout << "bind success" << endl;
    cout << "listening..." << endl;
    int listen_status = listen(socket_fd, SOMAXCONN);
    PrintError(listen_status == -1, "listen failed");

    
    int epfd = epoll_create(5); // create epoll instance

    int max_events = 1024;
    struct epoll_event ev, events[max_events];
    ev.events = EPOLLIN | EPOLLET; // set epoll event to read and edge-triggered
    ev.data.fd = socket_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, socket_fd, &ev); // add socket_fd to epoll instance
    int epoll_status = epoll_ctl(epfd, EPOLL_CTL_ADD, socket_fd, &ev); // add socket_fd to epoll instance

    while(true)
    {
        int event_num = epoll_wait(epfd, events, max_events, -1); // wait for epoll event

        for(int i = 0; i < event_num; i++)
        {
            if(events[i].data.fd == socket_fd) // if the event is from socket_fd, it means there is a new client
            {
                cout << "new client connected" << endl;
                struct sockaddr_in clit_addr;
                socklen_t clit_sock_len = sizeof(clit_addr);
                memset(&clit_addr, 0, clit_sock_len);
                int clit_socket_fd = accept(socket_fd, (sockaddr *)&clit_addr, &clit_sock_len);
                printf("new client fd %d! IP: %s Port: %d\n", clit_socket_fd, inet_ntoa(clit_addr.sin_addr), ntohs(clit_addr.sin_port));

                struct epoll_event ev_c;
                ev_c.events = EPOLLIN | EPOLLET; // set epoll event to read and edge-triggered
                ev_c.data.fd = clit_socket_fd;
                fcntl(clit_socket_fd, F_SETFL, fcntl(clit_socket_fd, F_GETFL, 0) | O_NONBLOCK); // set socket_fd to non-blocking mode
                epoll_ctl(epfd, EPOLL_CTL_ADD, clit_socket_fd, &ev_c); // add socket_fd to epoll instance
                cout << "add client" << endl;

            }else if (events[i].events == EPOLLIN){
                while(true){
                    char buff[1024];
                    memset(buff, 0, sizeof(buff));
                    ssize_t read_bytes = recv(events[i].data.fd, buff, sizeof(buff), 0);
                    if (read_bytes > 0){
                        cout <<"get message from client : "<< buff <<endl;
                        send(events[i].data.fd, buff, sizeof(buff), 0);
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
                        close(events[i].data.fd);
                        break;
                    }
                }
            }
        }
    }
    return 0;
}