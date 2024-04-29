#include "EventLoop.h"
#include <vector>



using namespace std;

EventLoop::EventLoop(Epoll* epoll):_epoll(epoll){}

EventLoop::~EventLoop() {
    // Close the epoll file descriptor
    close(this->_epoll->get_fd());
}

void EventLoop::run() {
    while (true) {
        // Wait for events
        auto events = _epoll->poll(1024, -1);
        for (int i = 0; i < events.size(); i++) 
         {
            events[i]->run_event_handle();
        }
    }
}

void EventLoop::add_event(Event& event) {
    _epoll->ctl_add(event.get_fd(), event.gen_epoll_event().get());
}

void EventLoop::remove_event(Event& event) {
    _epoll->ctl_del(event.get_fd(), event.gen_epoll_event().get());
}

void EventLoop::modify_event(Event& event) {
    _epoll->ctl_mod(event.get_fd(), event.gen_epoll_event().get());
}

// void accept_connection_handler(Socket* socket, Epoll *epoll){
//     cout << "new client connected" << endl;
//     struct sockaddr_in clit_addr;
//     socklen_t clit_sock_len = sizeof(clit_addr);
//     memset(&clit_addr, 0, clit_sock_len);

//     int clit_socket_fd = socket->accept(clit_addr);
//     set_non_blocking(clit_socket_fd);

//     // auto ev_c = gen_epoll_event(clit_socket_fd, EPOLLIN | EPOLLET);
//     Event* e = new Event(epoll, clit_socket_fd, EPOLLIN | EPOLLET); // 有內存洩漏風險
//     auto ev_c = e->gen_epoll_event();
//     auto cb = bind(et_read_event_handler, clit_socket_fd);
//     e->set_callback(cb);
//     epoll->ctl_add(clit_socket_fd, ev_c.get());
//     cout << "add client" << endl;
// }

// void et_read_event_handler(int read_fd){
//     while(true){
//         char buff[1024];
//         memset(buff, 0, sizeof(buff));
//         ssize_t read_bytes = recv(read_fd, buff, sizeof(buff), 0);
//         // printf("read_bytes: %ld\n", read_bytes);
//         if (read_bytes > 0){
//             cout <<"get message from client : "<< buff <<";"<<endl;
//             send(read_fd, buff, sizeof(buff), 0);
//         } else if (read_bytes == -1 && errno == EINTR){
//             // client normal interrupt;
//             cout <<"interrupted "<< buff <<endl;
//             continue; 
//         } else if (read_bytes == -1 &&(errno == EAGAIN || errno == EWOULDBLOCK)){
//             // data is all recived 
//             cout <<"message end! "<< buff <<endl;
//             break;
//         } else if (read_bytes == 0){
//             cout << "End of file" << endl;
//             close(read_fd);
//             break;
//         }
//     }
// }