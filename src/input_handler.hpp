#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <array>
#include <cstddef>
#include <memory>

enum InputType {
    PRESSED,
    DOWN
};

class InputHandler {
    struct Impl;
    std::unique_ptr<Impl> impl_; 
public:
    InputHandler();
    ~InputHandler();

    static constexpr size_t MAX_SIMULTANEOUS_INPUTS = 6;

    struct CommandList {
        std::array<int, MAX_SIMULTANEOUS_INPUTS> commands{};
        size_t count = 0;
    };

    CommandList handle_input();

    void bind_key(int key, InputType type, int command);
};

#endif // INPUTHANDLER_HPP
