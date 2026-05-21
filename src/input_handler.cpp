#include "raylib.h"
#include "input_handler.hpp"
#include <vector>

struct InputHandlerImpl {
    std::vector<int> activeKeyCodes;
    std::array<Command*, 350> keyBindings{nullptr};
};

InputHandler::InputHandler() {
    impl_ = new InputHandlerImpl();
}

InputHandler::~InputHandler() {
    delete impl_;
}

void InputHandler::bindKey(int key, Command *command) {
    impl_->keyBindings[key] = command;
    impl_->activeKeyCodes.push_back(key);
}

InputHandler::CommandList InputHandler::handleInput() {
    CommandList list;
    
    for (int key : impl_->activeKeyCodes) {
        if (list.count >= MAX_SIMULTANEOUS_INPUTS) break;
        
        if (IsKeyDown(key)) {
            Command* cmd = impl_->keyBindings[key];
            if (cmd != nullptr) {
                list.commands[list.count++] = cmd;
            }
        }
    }
    return list;
}
