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
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(addr);
    serv_addr.sin_port = htons(port);

    

    int bind_status = bind(socket_fd, (const sockaddr*)&serv_addr, sizeof(serv_addr));
    PrintError(bind_status == -1, "bind failed");
    // if(bind_status == -1){
    //     cout << "bind failed" << endl;
    //     return -1;
    // }

    int listen_status = listen(socket_fd, SOMAXCONN);
    PrintError(listen_status == -1, "listen failed");
    // if(listen_status == -1){
    //     cout << "listen failed" << endl;
    //     return -1;
    // }

    struct sockaddr_in clit_addr;
    socklen_t clit_sock_len = sizeof(clit_addr);
    memset(&clit_addr, 0, clit_sock_len);

    int clit_socket_fd = accept(socket_fd, (sockaddr *)&clit_addr, &clit_sock_len);
    printf("new client fd %d! IP: %s Port: %d\n", clit_socket_fd, inet_ntoa(clit_addr.sin_addr), ntohs(clit_addr.sin_port));

    while(true){
        char buff[1024];
        bzero(buff, sizeof(buff)); //清空buffer
        ssize_t read_bytes = read(clit_socket_fd, buff, sizeof(buff));
        if (read_bytes == -1){
            close(clit_socket_fd);
            PrintError(true, "socket read error");
        } else if (read_bytes == 0){
            cout << "EOF" << endl;
        } else {
            cout <<"get message from client : "<< buff <<endl;
            write(clit_socket_fd, buff, sizeof(buff));
        }
    }
 
    return 0;
}