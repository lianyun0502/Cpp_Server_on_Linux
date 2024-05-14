// 設計一個 thread pool 來管理多個 thread
// 這個 thread pool 會有一個 queue 來儲存要執行的任務
// 並且會有一個 vector 來儲存所有的 thread
// 這個 thread pool 會有一個 mutex 來保護 queue 的存取
// 這個 thread pool 會有一個 condition variable 來通知 thread 有新的任務要執行
// 這個 thread pool 會有一個 flag 來表示 thread pool 是否要結束
// 這個 thread pool 會有一個 function 來執行任務
// 這個 thread pool 會有一個 function 來新增任務
// 這個 thread pool 會有一個 function 來結束 thread pool


#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <future>

using namespace std;

class ThreadPool
{
    vector<thread> _threads;
    queue<function<void()>> _task_q;
    mutex _mutex;
    condition_variable* _cv;
    bool _stop = false;
public:
    ThreadPool(int t_num)
    {
        for (int i=0;i<t_num;++i)
        {
           _threads.emplace_back(thread([this]()
            {
                while (true)
                {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock = unique_lock<mutex>(_mutex);
                        _cv->wait(lock, [this](){return _stop || !_task_q.empty();});
                        if (_stop && _task_q.empty())
                        {
                            return;
                        }
                        task = _task_q.front();
                        _task_q.pop();
                    }
                    task();
                }
            })
            );
        }
    }
    ~ThreadPool(){
        cout<<"ThreadPool destructor"<<endl;
        if (!_stop){
            stop();
        }
    }


    // void addTask(function<void()> task)
    // {   
    //     {
    //         lock_guard<mutex> lock = lock_guard<mutex>(_mutex);
    //         _task_q.emplace(task);
    //         //  cout<<"addTask : "<< typeid(task).hash_code() << endl;
    //     }
    //     _cv->notify_one();
    // }
    template<typename F, typename... Args>
    future<typename result_of<F(Args...)>::type>
    addTask(F&& func, Args&&... args)
    {   
        using result_type = typename result_of<F(Args...)>::type;
        auto task = make_shared<packaged_task<result_type()>>(
            bind(forward<F>(func), forward<Args>(args)...)
        );
        future<result_type> res = task->get_future();
        {
            lock_guard<mutex> lock = lock_guard<mutex>(_mutex);
            _task_q.emplace([task](){(*task)();});
            //  cout<<"addTask : "<< typeid(task).hash_code() << endl;
        }
        _cv->notify_one();
        return res;
    }
    void stop()
    {
        {
            lock_guard<mutex> lock = lock_guard<mutex>(_mutex);
            _stop = true;
        }
        _cv->notify_all();
        for (auto& t : _threads)
        {
            if (t.joinable()){
                t.join();
            }
            
        }
        cout<<"ThreadPool stop"<<endl;
    }

};





#endif // THREADPOOL_H