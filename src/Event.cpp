#include "Event.h"
#include <cstring>


Event::Event(Epoll* epoll, int fd, uint32_t events):_ep(epoll), _fd(fd), _events(events){
    _callback = [](){cout << "default callback" << endl;};
}
Event::Event(int fd, uint32_t events): _fd(fd), _events(events){
    _callback = [](){cout << "default callback" << endl;};
}

Event::~Event(){cout << "Event destructor" << endl;}

void Event::run_event_handle()
{
    _callback();
}
int Event::get_fd(){
    return _fd;
}

void Event::set_callback(function<void()> callback){
    _callback = callback;
}

unique_ptr<struct epoll_event> Event::gen_epoll_event(){
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = _events; // set epoll event to read and edge-triggered
    ev.data.ptr = this;
    unique_ptr<struct epoll_event> ev_up = make_unique<struct epoll_event>(ev);
    return ev_up;
}