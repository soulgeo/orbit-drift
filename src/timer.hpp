#include <memory>

class Timer {
public:
    Timer(const char* name);

    ~Timer();

    void stop();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
