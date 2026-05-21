#include "raylib.h"
#include "input_handler.hpp"
#include <vector>

struct InputHandlerImpl {
    std::vector<int> activeKeyCodes;
};

InputHandler::InputHandler() {
    impl_ = new InputHandlerImpl();
}

InputHandler::~InputHandler() {
    delete impl_;
}

void InputHandler::bindKey(int key, Command *command) {
    keyBindings_[key] = command;
    impl_->activeKeyCodes.push_back(key);
}

InputHandler::CommandList InputHandler::handleInput() {
    CommandList list;
    
    for (int key : impl_->activeKeyCodes) {
        if (list.count >= MAX_SIMULTANEOUS_INPUTS) break;
        
        if (IsKeyDown(key)) {
            Command* cmd = keyBindings_[key];
            if (cmd != nullptr) {
                list.commands[list.count++] = cmd;
            }
        }
    }
    return list;
}
