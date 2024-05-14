#include <iostream>
#include <unistd.h>
#include <functional>

#include "src/utils.h"
#include "src/Socket.h"
#include "src/Connection.h"
#include "src/ThreadPool.h"

using namespace std;


void on_client()
{

    Socket clit_sock = Socket(Domain::IPv4, Type::TCP);
    auto connect_addr = inet_addr(Domain::IPv4, "0.0.0.0", 8888);
    clit_sock.connect(*connect_addr);

    Buffer r_buff, w_buff = Buffer();
    

    char buff[1024];
    int cnt = 100;
    sleep(5);
    while (cnt != 0)
    {
        w_buff.collect(string("ping:").append(to_string(cnt)));
        printf("count: %d, message to server: %s\n", cnt, w_buff.get_data().c_str());
        int w_state = send(clit_sock.get_fd(), w_buff.get_data().c_str(), w_buff.get_size(), 0);
        print_error(w_state == -1, "write error");
        while (1){
            bzero(buff, sizeof(buff));
            int r_size_num = recv(clit_sock.get_fd(), buff, sizeof(buff), 0);
            if (r_size_num > 0){
                r_buff.collect(buff);
            }else if(r_size_num == 0){
                printf("server disconnected!\n");
                exit(EXIT_SUCCESS);
            }else{
                cout << "r_size_num = " <<r_size_num<<endl;
            }
            if(r_buff.get_size() == w_buff.get_size()){
                w_buff.clear();
                printf("count: %d, message echo from server: %s\n", cnt--, r_buff.get_data().c_str());
                w_buff.clear();
                r_buff.clear();
                sleep(1);
                break;
            }
        }
    }

    close(clit_sock.get_fd());

}

int main (int argc, char *argv[])
{
    int threads = 10;
    int msgs = 100;
    int wait = 0;

    int o;
    const char *optstring = "t:m:w:";
    while ((o = getopt(argc, argv, optstring)) != -1) {
        switch (o) {
            case 't':
                threads = stoi(optarg);
                break;
            case 'm':
                msgs = stoi(optarg);
                break;
            case 'w':
                wait = stoi(optarg);
                break;
            case '?':
                printf("error optopt: %c\n", optopt);
                printf("error opterr: %d\n", opterr);
                break;
        }
    }

    ThreadPool thread_pool = ThreadPool(100);

    auto fn = bind(on_client);

    for (int i=0; i<threads; ++i){
        thread_pool.addTask(fn);
    }

}