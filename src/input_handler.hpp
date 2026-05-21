#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include "commands.hpp"
#include <array>

struct InputHandlerImpl;

class InputHandler {
public:
    InputHandler();
    ~InputHandler();

    static constexpr size_t MAX_SIMULTANEOUS_INPUTS = 6;

    struct CommandList {
        std::array<Command*, MAX_SIMULTANEOUS_INPUTS> commands{};
        size_t count = 0;
    };

    CommandList handleInput();
    void bindKey(int key, Command* command);

private:
    std::array<Command*, 350> keyBindings_{nullptr};
    InputHandlerImpl* impl_; 
};

#endif // INPUTHANDLER_HPP
