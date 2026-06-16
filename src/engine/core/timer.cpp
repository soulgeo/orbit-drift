#include "timer.hpp"
#include <chrono>
#include <iostream>

Timer::Timer(const char* name) 
    : name_(name), start_timepoint_(std::chrono::steady_clock::now()), stopped_(false)
{
}

Timer::~Timer() {
    if (!stopped_) stop();
}

void Timer::stop(){
    auto end_timepoint = std::chrono::steady_clock::now();

    long long start = 
        std::chrono::time_point_cast<std::chrono::microseconds>(start_timepoint_)
        .time_since_epoch().count();
    long long end = 
        std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint)
        .time_since_epoch().count();

    stopped_ = true;

    float duration = (end - start) * 0.001f;
    std::cout << name_ << ": " << duration << "ms" << std::endl;
}
