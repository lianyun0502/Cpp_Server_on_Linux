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
#include "src/Connection.h"
using namespace std;


string addr = "0.0.0.0";
uint16_t port = 8888;



void et_read_event_handler(Connection& conn){
    int read_fd = conn.get_fd();
    char buff[1024];
    conn.read_buffer->clear();
    while(true){
        memset(buff, 0, sizeof(buff));
        ssize_t read_bytes = recv(read_fd, buff, sizeof(buff), 0);
        // printf("read_bytes: %ld\n", read_bytes);
        if (read_bytes > 0){
            conn.read_buffer->collect(buff);
        } else if (read_bytes == -1 && errno == EINTR){
            // client normal interrupt;
            cout <<"interrupted "<< buff <<endl;
            continue; 
        } else if (read_bytes == -1 &&(errno == EAGAIN || errno == EWOULDBLOCK)){
            // data is all recived 
            auto echo = conn.read_buffer->get_data().c_str();
            cout <<"get message from client : "<< echo <<";"<<endl;
            send(read_fd, echo, conn.read_buffer->get_size(), 0);
            cout <<"message end! "<<endl;
            conn.read_buffer->clear();
            break;
        } else if (read_bytes == 0){
            cout << "End of file" << endl;
            conn.disconnect();
            break;
        }
    }
}
void accept_connection_handler(Socket* sock, EventLoop* loop){
    cout << "new client connected" << endl;
    int clit_socket_fd = sock->accept();
    set_non_blocking(clit_socket_fd);
    Connection* connection = new Connection(loop, clit_socket_fd);
    auto cb = bind(et_read_event_handler, placeholders::_1);
    connection->set_connection_callback(cb);
    cout << "add client" << endl;
}


int main(){
    cout << __cplusplus << endl;
    EventLoop * loop = new EventLoop();
    Acceptor *acceptor = new Acceptor(loop, addr, port);
    auto cb = bind(accept_connection_handler, placeholders::_1, loop);
    acceptor->set_connection_handler(cb);
    loop->run();
    delete acceptor;
    delete loop;
    return 0;
}
