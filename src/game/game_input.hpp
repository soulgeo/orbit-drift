#ifndef GAME_INPUT_HPP
#define GAME_INPUT_HPP

namespace Sputnik {

    enum GameAction {
        INPUT_NONE = 0,
        INPUT_PAUSE,
        INPUT_DEBUG,
        INPUT_MOVE_UP,
        INPUT_MOVE_DOWN,
        INPUT_MOVE_FORWARD,
        INPUT_MOVE_BACK,
        INPUT_MOVE_LEFT,
        INPUT_MOVE_RIGHT,
        INPUT_ROLL_CW,
        INPUT_ROLL_CCW,
    };

}

#endif // GAME_INPUT_HPP
