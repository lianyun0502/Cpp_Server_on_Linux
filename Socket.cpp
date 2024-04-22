#include<sys/socket.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string>
#include<cstring>
#include<fcntl.h>
#include<unistd.h>
#include<exception>

#include "utils.h"
#include "Socket.h"

using namespace std;


Socket::Socket(Domain domain, Type type, int protocol)
{
    this->_fd = socket(static_cast<int>(domain), static_cast<int>(type), protocol);
    print_error(this->_fd == -1, "Socket creation failed");
};

Socket::~Socket()
{
    if (this->_fd != -1)
    {
        close(this->_fd);
    }
};

void Socket::bind(const struct sockaddr_in &addr)
{
    cout << "server start at " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << endl;
    cout << "binding..." << endl;
    int status = ::bind(this->_fd, (const sockaddr*)&addr, sizeof(addr));
    print_error(status == -1, "Bind failed");
    cout << "bind success" << endl;
};

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
};

int Socket::accept(const struct sockaddr_in &client_addr){
    socklen_t s_len = sizeof(client_addr);
    int client_fd = ::accept(this->_fd, (sockaddr *)&client_addr, &s_len);
    print_error(client_fd == -1, "Accept failed");
    printf("new client fd %d! IP: %s Port: %d\n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    return client_fd;
};

void Socket::connect(const struct sockaddr_in &addr){
    cout << "connecting..." << endl;
    int status = ::connect(this->_fd, (sockaddr *)&addr, sizeof(addr));
    print_error(status == -1, "Connect failed");
    
};

int Socket::get_fd()
{
    return this->_fd;
};




