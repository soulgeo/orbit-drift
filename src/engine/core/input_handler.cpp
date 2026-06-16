#include "raylib.h"
#include "input_handler.hpp"
#include <utility>
#include <vector>

InputHandler::InputHandler() {
}

InputHandler::~InputHandler() = default;

void InputHandler::bind_key(int key, InputType type, int command) {
    bindings_[key].first = type;
    bindings_[key].second = command;
    active_key_codes_.push_back({key, type});
}

InputHandler::CommandList InputHandler::handle_input() {
    CommandList list;

    for (auto pair : active_key_codes_) {
        if (list.count >= MAX_SIMULTANEOUS_INPUTS) break;
        auto key = pair.first;
        auto inputType = pair.second;

        if ((inputType == DOWN && IsKeyDown(key)) || 
            (inputType == PRESSED && IsKeyPressed(key))) {

            int cmd = bindings_[key].second; 
            list.commands[list.count++] = cmd;
        }
    }
    return list;
}
