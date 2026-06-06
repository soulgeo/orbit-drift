#include "raylib.h"
#include "input_handler.hpp"
#include <utility>
#include <vector>

struct InputHandlerImpl {
    std::vector<std::pair<int, InputType>> active_key_codes;
    std::array<std::pair<InputType, int>, 350> bindings;
};

InputHandler::InputHandler() {
    impl_ = new InputHandlerImpl();
}

InputHandler::~InputHandler() {
    delete impl_;
}

void InputHandler::bind_key(int key, InputType type, int command) {
    impl_->bindings[key].first = type;
    impl_->bindings[key].second = command;
    impl_->active_key_codes.push_back({key, type});
}

InputHandler::CommandList InputHandler::handle_input() {
    CommandList list;

    for (auto pair : impl_->active_key_codes) {
        if (list.count >= MAX_SIMULTANEOUS_INPUTS) break;
        auto key = pair.first;
        auto inputType = pair.second;

        if ((inputType == DOWN && IsKeyDown(key)) || 
            (inputType == PRESSED && IsKeyPressed(key))) {

            int cmd = impl_->bindings[key].second; 
            list.commands[list.count++] = cmd;
        }
    }
    return list;
}
