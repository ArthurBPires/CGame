#ifndef TIMER_H
#define TIMER_H

#include "Scene.h"
#include <chrono>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

//Taken from https://stackoverflow.com/questions/41152393/c-non-blocking-async-timer

class Timer{
public:
    Timer(size_t time, const std::function<void(void)>& f) : time{std::chrono::milliseconds{time}}, f{f} {}
    ~Timer() { wait_thread.join(); }

    //static std::vector<Object *> objects;
private:
    void wait_then_call()
    {
        std::unique_lock<std::mutex> lck{mtx};
        cv.wait_for(lck, time);
        f();
    }
    std::mutex mtx;
    std::condition_variable cv{};
    std::chrono::milliseconds time;
    std::function <void(void)> f;
    std::thread wait_thread{[this]() {wait_then_call(); }};
};
#endif // TIMER_H
