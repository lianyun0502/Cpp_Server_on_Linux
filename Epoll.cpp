#include "Epoll.h"
#include "utils.h"



Epoll::Epoll(int max_fd)
{
    this->_fd = epoll_create(max_fd);
    print_error(this->_fd == -1, "epoll_create failed");
    
}
Epoll::~Epoll()
{   
    if (this->events != nullptr)
    {
        delete[] this->events;
    }
    ::close(this->_fd);
}

void Epoll::ctl_add(int fd, epoll_event *ev_ptr)
{
    int ret = epoll_ctl(this->_fd, EPOLL_CTL_ADD, fd, ev_ptr);
    print_error(ret == -1, "epoll_ctl add failed");
}

void Epoll::ctl_mod(int fd, epoll_event *ev_ptr)
{
    int ret = epoll_ctl(this->_fd, EPOLL_CTL_MOD, fd, ev_ptr);
    print_error(ret == -1, "epoll_ctl mod failed");
}

void Epoll::ctl_del(int fd, epoll_event *ev_ptr)
{
    int ret = epoll_ctl(this->_fd, EPOLL_CTL_DEL, fd, ev_ptr);
    print_error(ret == -1, "epoll_ctl del failed");
}

int Epoll::wait(int max_event, int timeout)
{
    if (this->events != nullptr)
    {
        delete[] this->events;
    }
    this->events = new epoll_event[max_event];
    int event_num = epoll_wait(this->_fd, events, max_event, timeout);
    print_error(event_num == -1, "epoll_wait failed");
    return event_num;
}

struct epoll_event creat_event(int fd, uint32_t events, bool is_et)
{
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.data.fd = fd;
    event.events = events;
    if (is_et){ event.events |= EPOLLET; }
    return event; // 怕返回的是局部變數，所以要用拷貝
}