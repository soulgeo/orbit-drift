#include "raylib.h"
#include "input_handler.hpp"

InputHandler::CommandList InputHandler::handleInput() {
    CommandList list;
    auto addCommand = [&](Command* cmd) {
        if (cmd && list.count < MAX_SIMULTANEOUS_INPUTS) {
            list.commands[list.count++] = cmd;
        }
    };

    if (IsKeyDown(KEY_X)) addCommand(buttonW_);
    if (IsKeyDown(KEY_A)) addCommand(buttonA_);
    if (IsKeyDown(KEY_S)) addCommand(buttonS_);
    if (IsKeyDown(KEY_D)) addCommand(buttonD_);
    if (IsKeyDown(KEY_Q)) addCommand(buttonQ_);
    if (IsKeyDown(KEY_E)) addCommand(buttonE_);

    return list;
}
