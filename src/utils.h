#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
void print_error(bool condition, const string &msg);
void set_non_blocking(int fd);