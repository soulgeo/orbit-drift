#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <array>
#include <cstddef>
#include <vector>
#include <utility>

namespace Sputnik {

    enum InputType {
        PRESSED,
        DOWN
    };

    class InputHandler {
    public:
        static constexpr size_t MAX_SIMULTANEOUS_INPUTS = 16;

        struct CommandList {
            std::array<int, MAX_SIMULTANEOUS_INPUTS> commands{};
            size_t count = 0;
        };

    private:
        std::vector<std::pair<int, InputType>> active_key_codes_;
        std::array<std::pair<InputType, int>, 350> bindings_;
        CommandList active_actions_;

    public:
        InputHandler();
        ~InputHandler();

        void update();
        bool is_action_active(int action_id) const;

        void bind_key(int key, InputType type, int action_id);
    };

}

#endif // INPUTHANDLER_HPP
