
#include "utils.h"
#include "Socket.h"

using namespace std;


Socket::Socket(Domain domain, Type type, int protocol)
{
    this->_fd = socket(static_cast<int>(domain), static_cast<int>(type), protocol);
    print_error(this->_fd == -1, "Socket creation failed");
    cout << "Socket created, fd = " << _fd << endl;
}

Socket::Socket(int fd)
: _fd(fd)
{
    cout << "Socket created, fd = " << _fd << endl;
}

Socket::~Socket()
{
    if (this->_fd != -1)
    {
        close(this->_fd);
    }
}

void Socket::bind(const struct sockaddr_in &addr)
{
    cout << "server start at " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << endl;
    cout << "binding..." << endl;
    int status = ::bind(this->_fd, (const sockaddr*)&addr, sizeof(addr));
    print_error(status == -1, "Bind failed");
    cout << "bind success" << endl;
}

void Socket::listen(u_int16_t max_conn_sock)
{
    if (max_conn_sock > SOMAXCONN)
    {
        throw runtime_error("Max connection socket is greater than SOMAXCONN");
    }
    cout << "listening..." << endl;
    int status = ::listen(this->_fd, (int)max_conn_sock);
    if (status == -1)
    {
        throw runtime_error(strerror(errno));
    }
}

int Socket::accept(){
    struct sockaddr_in client_addr;
    socklen_t s_len = sizeof(client_addr);
    memset(&client_addr, 0, s_len);
    int client_fd = ::accept(this->_fd, (sockaddr *)&client_addr, &s_len);
    print_error(client_fd == -1, "Accept failed");
    printf("new client fd %d! IP: %s Port: %d\n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    return client_fd;
}

void Socket::connect(const struct sockaddr_in &addr){
    cout << "connecting..." << endl;
    int status = ::connect(this->_fd, (sockaddr *)&addr, sizeof(addr));
    print_error(status == -1, "Connect failed");
    
}

int Socket::get_fd()
{
    return this->_fd;
}

unique_ptr<sockaddr_in> inet_addr(Domain domain, const char * addr, uint16_t port){
    unique_ptr<sockaddr_in> serv_addr = make_unique<sockaddr_in>();
    memset(serv_addr.get(), 0, sizeof(sockaddr_in));
    serv_addr->sin_family = (sa_family_t)domain;
    serv_addr->sin_addr.s_addr = inet_addr(addr);
    serv_addr->sin_port = htons(port);
    return serv_addr;
}

