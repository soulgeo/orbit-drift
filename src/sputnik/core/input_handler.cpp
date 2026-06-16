#include <raylib.h>
#include "sputnik/core/input_handler.hpp"
#include <utility>
#include <vector>

namespace Sputnik {

    InputHandler::InputHandler() {
    }

    InputHandler::~InputHandler() = default;

    void InputHandler::bind_key(int key, InputType type, int action_id) {
        bindings_[key].first = type;
        bindings_[key].second = action_id;
        active_key_codes_.push_back({key, type});
    }

    void InputHandler::update() {
        active_actions_.count = 0;

        for (auto pair : active_key_codes_) {
            if (active_actions_.count >= MAX_SIMULTANEOUS_INPUTS) break;
            auto key = pair.first;
            auto inputType = pair.second;

            if ((inputType == DOWN && IsKeyDown(key)) || 
                (inputType == PRESSED && IsKeyPressed(key))) {

                int action = bindings_[key].second; 
                active_actions_.commands[active_actions_.count++] = action;
            }
        }
    }

    bool InputHandler::is_action_active(int action_id) const {
        for (size_t i = 0; i < active_actions_.count; ++i) {
            if (active_actions_.commands[i] == action_id) {
                return true;
            }
        }
        return false;
    }

}
