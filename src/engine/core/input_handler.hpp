#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <array>
#include <cstddef>
#include <vector>
#include <utility>

enum InputType {
    PRESSED,
    DOWN
};

class InputHandler {
    std::vector<std::pair<int, InputType>> active_key_codes_;
    std::array<std::pair<InputType, int>, 350> bindings_;

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
