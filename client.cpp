#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>

#include "utils.h"

using namespace std;


const char * addr = "0.0.0.0";
uint16_t port = 8888;


int main(){

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in clit_addr;
    memset(&clit_addr, 0, sizeof(clit_addr));
    clit_addr.sin_addr.s_addr = inet_addr(addr);
    clit_addr.sin_port = htons(port);
    clit_addr.sin_family = AF_INET;

    int conn_status = connect(client_fd, (const sockaddr *)&clit_addr, sizeof(clit_addr));
    PrintError(conn_status == -1, "connect failed!");

    while(true){
        char buff[1024];
        bzero(buff, sizeof(buff));
        cout << "write some message :" <<endl;
        cin.getline(buff, 1024);
        ssize_t write_bytes = send(client_fd, buff, sizeof(buff), 0);
        PrintError(write_bytes == -1, "write error");

        bzero(buff, sizeof(buff));

        ssize_t read_bytes = recv(client_fd, buff, sizeof(buff), 0);

        if (read_bytes == -1){
            PrintError(true, "read error");
            close(client_fd);
        } else if (read_bytes == 0){
            cout << "EOF" << endl;

        }else{
            cout << "echo : " << buff <<endl;
        }
    }
    return 0;
}