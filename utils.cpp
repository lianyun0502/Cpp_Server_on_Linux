#include "utils.h"
#include <string>

void PrintError(bool condition, const string &msg){
    if(condition){
        cout << msg << endl;
        exit(EXIT_FAILURE);
    }
}
