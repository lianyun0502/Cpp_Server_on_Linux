#pragma once
#include<sys/socket.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string>
#include<cstring>
#include<fcntl.h>
#include<unistd.h>
#include<exception>
#include<memory>






enum class Domain 
{
    IPv4 = AF_INET,
    IPv6 = AF_INET6,
};

enum class Type 
{
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM,
    Raw = SOCK_RAW,
    SeqPacket = SOCK_SEQPACKET,
};

class Socket
{
    int _fd = -1;
public:
    Socket(Domain domain, Type type, int protocol=0);
    Socket(int fd);
    ~Socket();
    void bind(const struct sockaddr_in &addr);
    void listen(u_int16_t max_conn_sock = SOMAXCONN);
    void connect(const struct sockaddr_in &addr);
    int accept();
    int get_fd();
};

#include <memory>
unique_ptr<sockaddr_in> inet_addr(Domain domain, const char * addr, uint16_t port);
