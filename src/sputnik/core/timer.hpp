#include <chrono>

namespace Sputnik {

    class Timer {
        const char* name_;
        std::chrono::time_point<std::chrono::steady_clock> start_timepoint_;
        bool stopped_ = false;
    public:
        Timer(const char* name);

        ~Timer();

        void stop();
    };

}
