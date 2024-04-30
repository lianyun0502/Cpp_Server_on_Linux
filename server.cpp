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
            conn.disconnect();
            break;
        }
    }
}
void accept_connection_handler(Socket* socket, EventLoop* loop){
    cout << "new client connected" << endl;
    Connection* connection = new Connection(loop, clit_socket_fd);
    auto cb = bind(et_read_event_handler, placeholders::_1);
    connection->set_connection_callback(cb);
    cout << "add client" << endl;
}


int main(){
    cout << __cplusplus << endl;
    Epoll *epoll = new Epoll(5);
    EventLoop * loop = new EventLoop(epoll);
    Acceptor *acceptor = new Acceptor(loop, addr, port);

    auto cb = bind(accept_connection_handler, placeholders::_1, loop);
    acceptor->set_connection_handler(cb);
    
    loop->run();

    delete acceptor;
    delete epoll;
    delete loop;
    return 0;
}
