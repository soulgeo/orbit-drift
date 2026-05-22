#include "raylib.h"
#include "input_handler.hpp"
#include <utility>
#include <vector>
#include <string>

struct InputHandlerImpl {
    std::vector<std::pair<int, InputType>> activeKeyCodes;
    std::array<std::pair<InputType, std::string>, 350> keyBindings;
};

InputHandler::InputHandler() {
    impl_ = new InputHandlerImpl();
}

InputHandler::~InputHandler() {
    delete impl_;
}

void InputHandler::bindKey(int key, InputType type, std::string command) {
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

        if (inputType == DOWN) {
            if (IsKeyDown(key)) {
                std::string cmd = impl_->keyBindings[key].second; 
                list.commands[list.count++] = cmd;
            }
        } else if (inputType == PRESSED) {
            if (IsKeyPressed(key)) {
                std::string cmd = impl_->keyBindings[key].second; 
                list.commands[list.count++] = cmd;
            }
        }
    }
    return list;
}
