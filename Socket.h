#pragma once
#include <sys/socket.h>


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
    ~Socket();
    void bind(const struct sockaddr_in &addr);
    void listen(u_int16_t max_conn_sock = SOMAXCONN);
    void connect(const struct sockaddr_in &addr);
    int accept(const struct sockaddr_in &client_addr);
    int get_fd();
};
