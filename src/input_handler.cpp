#include "raylib.h"
#include "input_handler.hpp"
#include <utility>
#include <vector>

struct InputHandlerImpl {
    std::vector<std::pair<int, InputType>> activeKeyCodes;
    std::array<std::pair<InputType, int>, 350> keyBindings;
};

InputHandler::InputHandler() {
    impl_ = new InputHandlerImpl();
}

InputHandler::~InputHandler() {
    delete impl_;
}

void InputHandler::bindKey(int key, InputType type, int command) {
    impl_->keyBindings[key].first = type;
    impl_->keyBindings[key].second = command;
    impl_->activeKeyCodes.push_back({key, type});
}

InputHandler::CommandList InputHandler::handleInput() {
    CommandList list;

    for (auto pair : impl_->activeKeyCodes) {
        if (list.count >= MAX_SIMULTANEOUS_INPUTS) break;
        auto key = pair.first;
        auto inputType = pair.second;

        if ((inputType == DOWN && IsKeyDown(key)) || 
            (inputType == PRESSED && IsKeyPressed(key))) {

            int cmd = impl_->keyBindings[key].second; 
            list.commands[list.count++] = cmd;
        }
    }
    return list;
}
