#include "Connection.h"
using namespace std;


Connection::Connection(EventLoop* loop, int fd)
: _loop(loop), _fd(fd)
{
    _event = new Event(_fd, EPOLLIN | EPOLLET);
    auto cb = bind(&Connection::handle_connection, this);
    _event->set_callback(cb);
    _loop->add_event(*_event);
}

Connection::~Connection()
{
    // cout << "Connection::~Connection" << endl;
    delete _event;
}

void Connection::handle_connection()
{
    // cout << "Connection::handle_connection" << endl;
    _connection_clallback(*this);
}

void Connection::set_connection_callback(function<void(Connection&)> cb)
{
    _connection_clallback = cb;
}

void Connection::disconnect()
{
    cout << "disconnect" << endl;

    close(this->_fd);
    delete this;
}

