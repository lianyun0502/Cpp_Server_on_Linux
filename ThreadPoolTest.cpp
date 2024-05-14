#include <iostream>
#include <string>
#include <functional>
#include <chrono>
#include <unistd.h>

#include "src/ThreadPool.h"

using namespace std;
using namespace std::chrono;



int main(){
    function<void(int)> f = [](int i ){
        cout << i << ": hello world" << endl;
        sleep(1);
    };
    auto start = steady_clock::now();
    // f();
    // f();
    // f();
    auto elasped = steady_clock::now() - start;
    cout << duration_cast<seconds>(elasped).count() << " s" << endl;

    // cout << i << endl;
    ThreadPool pool(100);

    start = steady_clock::now();
    for (int i = 0; i < 100; i++){
        pool.addTask(f, i);
    }
    pool.stop();
    elasped = steady_clock::now() - start;
    cout << duration_cast<seconds>(elasped).count() << " s" << endl;


    return 0;

}