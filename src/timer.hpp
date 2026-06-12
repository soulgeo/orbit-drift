#include <memory>

class Timer {
    struct Impl;
    std::unique_ptr<Impl> impl_;
public:
    Timer(const char* name);

    ~Timer();

    void stop();
};
