#pragma once

#include <sys/epoll.h>
#include <string>
#include <cstring>


using namespace std;

class Epoll
{
    int _fd;
    
public:
    struct epoll_event* events = nullptr;
    Epoll(int max_fd=10);
    ~Epoll();
    void ctl_add(int fd, epoll_event *event);
    void ctl_mod(int fd, epoll_event *event);
    void ctl_del(int fd, epoll_event *event);
    int wait(int max_event, int timeout=-1);
};

struct epoll_event create_event(int fd, uint32_t events, bool is_et); 