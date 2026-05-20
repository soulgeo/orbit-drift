#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include "commands.hpp"
#include <array>

class InputHandler {
public:
    static constexpr size_t MAX_SIMULTANEOUS_INPUTS = 6;

    struct CommandList {
        std::array<Command*, MAX_SIMULTANEOUS_INPUTS> commands{};
        size_t count = 0;
    };

    CommandList handleInput();

    void bindButtonW(Command* command) { buttonW_ = command; }
    void bindButtonA(Command* command) { buttonA_ = command; }
    void bindButtonS(Command* command) { buttonS_ = command; }
    void bindButtonD(Command* command) { buttonD_ = command; }
    void bindButtonQ(Command* command) { buttonQ_ = command; }
    void bindButtonE(Command* command) { buttonE_ = command; }

private:
    Command* buttonW_ = nullptr;
    Command* buttonA_ = nullptr;
    Command* buttonS_ = nullptr;
    Command* buttonD_ = nullptr;
    Command* buttonQ_ = nullptr;
    Command* buttonE_ = nullptr;
};

#endif // INPUTHANDLER_HPP
