#include "global_control_component.hpp"
#include "sputnik/ecs/game_object.hpp"
#include "sputnik/core/engine.hpp"
#include "game_input.hpp"

namespace Sputnik {

    GlobalControlComponent::GlobalControlComponent(GameObject* owner) : Component(owner) {}

    GlobalControlComponent::~GlobalControlComponent() = default;

    void GlobalControlComponent::early_update() {
        Engine* e = owner_->engine();
        auto& input = e->input_handler();
        
        if (input.is_action_active(INPUT_PAUSE)) {
            e->toggle_pause();
        }

        if (input.is_action_active(INPUT_DEBUG)) {
            e->renderer().toggle_show_debug();
        }
    }

}
