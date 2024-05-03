#include "Connection.h"
using namespace std;


Connection::Connection(EventLoop* loop, int fd)
: _loop(loop), _fd(fd), read_buffer(new Buffer)
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
    delete read_buffer;
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




void Buffer::collect(const char* data)
{
    int data_len = strlen(data);
    for (int i=0 ; i< data_len ; ++i){
        if (data[i] == '\0'){break;}
            _buffer->push_back(data[i]);
        }
}

void Buffer::collect(string &data)
{
    *_buffer = *_buffer + data;
}

string& Buffer::get_data()
{
    return *_buffer;
}

void Buffer::clear()
{
    _buffer->clear();
}

size_t Buffer::get_size()
{
    return _buffer->size();
}

Buffer::~Buffer()
{
    delete _buffer;
}