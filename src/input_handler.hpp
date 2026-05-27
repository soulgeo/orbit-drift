#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <array>
#include <cstddef>

struct InputHandlerImpl;

enum InputType {
    PRESSED,
    DOWN
};

class InputHandler {
public:
    InputHandler();
    ~InputHandler();

    static constexpr size_t MAX_SIMULTANEOUS_INPUTS = 6;

    struct CommandList {
        std::array<int, MAX_SIMULTANEOUS_INPUTS> commands{};
        size_t count = 0;
    };

    CommandList handleInput();

    void bindKey(int key, InputType type, int command);

private:
    InputHandlerImpl* impl_; 
};

#endif // INPUTHANDLER_HPP
