#include "timer.hpp"
#include <chrono>
#include <iostream>
#include <memory>

struct Timer::Impl {
    const char* name;
    std::chrono::time_point<std::chrono::steady_clock> start_timepoint;
    bool stopped;
};

Timer::Timer(const char* name) {
    impl_ = std::make_unique<Impl>();
    impl_->name = name;
    impl_->start_timepoint = std::chrono::steady_clock::now();
}

Timer::~Timer() {
    if (!impl_->stopped) stop();
}

void Timer::stop(){
    auto end_timepoint = std::chrono::steady_clock::now();

    long long start = 
        std::chrono::time_point_cast<std::chrono::microseconds>(impl_->start_timepoint)
        .time_since_epoch().count();
    long long end = 
        std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint)
        .time_since_epoch().count();

    impl_->stopped = true;

    float duration = (end - start) * 0.001f;
    std::cout << impl_->name << ": " << duration << "ms" << std::endl;
}
