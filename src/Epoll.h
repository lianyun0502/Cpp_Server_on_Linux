#pragma once

#include <sys/epoll.h>
#include <string>
#include <cstring>
#include <memory>


#include "utils.h"
#include "Event.h"



using namespace std;

class Epoll
{
    int _fd; // epoll fd
    
public:
    struct epoll_event* _events = nullptr;
    Epoll(int max_fd=10);
    ~Epoll();
    void ctl_add(int fd, epoll_event *event);
    void ctl_mod(int fd, epoll_event *event);
    void ctl_del(int fd, epoll_event *event);
    int wait(int max_event, int timeout=-1);
    vector<Event*> poll(int max_event, int timeout);
    int get_fd();
};

unique_ptr<struct epoll_event> gen_epoll_event(int fd, uint32_t events, ...);



   