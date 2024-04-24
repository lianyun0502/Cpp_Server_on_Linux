#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<string>
#include<unistd.h>

#include "src/utils.h"
#include "src/Socket.h"

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
    char buff[1024];
    string s;
    while(true){
        cout << "write some message : ";
        getline(cin, s);
        ssize_t write_bytes = send(client->get_fd(), s.c_str(), s.length(), 0);
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