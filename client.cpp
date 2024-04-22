#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>

#include "utils.h"
#include "Socket.h"

using namespace std;


const char * addr = "0.0.0.0";
uint16_t port = 8888;


int main(){

    Socket * client = new Socket(Domain::IPv4, Type::TCP);

    struct sockaddr_in clit_addr;
    memset(&clit_addr, 0, sizeof(clit_addr));
    clit_addr.sin_addr.s_addr = inet_addr(addr);
    clit_addr.sin_port = htons(port);
    clit_addr.sin_family = AF_INET;

    client->connect(clit_addr);

    while(true){
        char buff[1024];
        bzero(buff, sizeof(buff));
        cout << "write some message : ";
        cin.getline(buff, 1024);
        ssize_t write_bytes = send(client->get_fd(), buff, sizeof(buff), 0);
        print_error(write_bytes == -1, "write error");

        bzero(buff, sizeof(buff));

        ssize_t read_bytes = recv(client->get_fd(), buff, sizeof(buff), 0);

        if (read_bytes == -1){
            print_error(true, "read error");
            close(client->get_fd());
        } else if (read_bytes == 0){
            cout << "EOF" << endl;
            break;
        }else{
            cout << "echo : " << buff <<endl;
        }
    }
    delete client;
    return 0;
}