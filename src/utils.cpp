#include "utils.h"
#include <errno.h>
#include <string>
#include <iostream>
#include<cstring>

void print_error(bool condition, const string &msg = ""){
    if(condition){
        cout << msg << endl;
        throw runtime_error(strerror(errno));
    }
}

void set_non_blocking(int sock_fd)
{
    int flags = fcntl(sock_fd, F_GETFL, 0);
    print_error(flags == -1, "Failed to get flags");
    int status = fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK);
    print_error(status == -1, "Failed to set non-blocking");
}