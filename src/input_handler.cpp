#include "raylib.h"
#include "input_handler.hpp"
#include <vector>
#include <string>

struct InputHandlerImpl {
    std::vector<int> activeKeyCodes;
    std::array<std::string, 350> keyBindings;
};

InputHandler::InputHandler() {
    impl_ = new InputHandlerImpl();
}

InputHandler::~InputHandler() {
    delete impl_;
}

void InputHandler::bindKey(int key, std::string command) {
    impl_->keyBindings[key] = command;
    impl_->activeKeyCodes.push_back(key);
}

InputHandler::CommandList InputHandler::handleInput() {
    CommandList list;

    for (int key : impl_->activeKeyCodes) {
        if (list.count >= MAX_SIMULTANEOUS_INPUTS) break;

        if (IsKeyDown(key)) {
            std::string cmd = impl_->keyBindings[key]; 
            list.commands[list.count++] = cmd;
        }
    }
    return list;
}
