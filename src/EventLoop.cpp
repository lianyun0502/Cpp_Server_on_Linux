#include "EventLoop.h"



using namespace std;

EventLoop::EventLoop(Epoll* epoll, Socket* socket):_epoll(epoll), _socket(socket)
{

}

EventLoop::~EventLoop() {
    // Close the epoll file descriptor
    close(this->_epoll->get_fd());
}

void EventLoop::run() {
    while (true) {
        // Wait for events
        int num_events = _epoll->wait(1024, -1);
        if (num_events == -1) {
            throw runtime_error("Error waiting for events");
        }

        // Handle events
        for (int i = 0; i < num_events; i++) 
         {
            if(_epoll->events[i].data.fd == _socket->get_fd()) // if the event is from socket_fd, it means there is a new client
            {
                cout << "new client connected" << endl;
                struct sockaddr_in clit_addr;
                socklen_t clit_sock_len = sizeof(clit_addr);
                memset(&clit_addr, 0, clit_sock_len);

                int clit_socket_fd = _socket->accept(clit_addr);
                set_non_blocking(clit_socket_fd);

                auto ev_c = gen_epoll_event(clit_socket_fd, EPOLLIN | EPOLLET);

                _epoll->ctl_add(clit_socket_fd, ev_c.get());
                cout << "add client" << endl;

            }else if (_epoll->events[i].events == EPOLLIN){
                et_read_event_handler(_epoll->events[i].data.fd);
            }
        }
    }
}

void et_read_event_handler(int read_fd){
    while(true){
        char buff[1024];
        memset(buff, 0, sizeof(buff));
        ssize_t read_bytes = recv(read_fd, buff, sizeof(buff), 0);
        printf("read_bytes: %ld\n", read_bytes);
        if (read_bytes > 0){
            cout <<"get message from client : "<< buff <<";"<<endl;
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