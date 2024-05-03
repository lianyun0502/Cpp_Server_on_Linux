#include <iostream>
#include <string>
#include <functional>
#include <chrono>
#include <unistd.h>

#include "src/ThreadPool.h"

using namespace std;
using namespace std::chrono;



int main(){
    int i = 0;
    function<void() > f = [&i](){
        cout << i << ": hello world" << endl;
        i++;
        sleep(1);
    };
    auto start = steady_clock::now();
    f();
    f();
    f();
    auto elasped = steady_clock::now() - start;
    cout << duration_cast<seconds>(elasped).count() << " s" << endl;

    cout << i << endl;
    ThreadPool pool(10);

    start = steady_clock::now();
    for (int i = 0; i < 100; i++){
        pool.addTask(f);
    }
    pool.stop();
    elasped = steady_clock::now() - start;
    cout << duration_cast<seconds>(elasped).count() << " s" << endl;


    return 0;

}