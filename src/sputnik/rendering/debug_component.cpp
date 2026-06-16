#include "sputnik/rendering/debug_component.hpp"
#include "sputnik/ecs/component.hpp"
#include "sputnik/ecs/game_object.hpp"
#include <vector>

namespace Sputnik {

    DebugComponent::DebugComponent(GameObject* owner, Renderer* renderer) : Component(owner) {
        renderer->register_debug(this);
    }

    DebugComponent::~DebugComponent() = default;

    void DebugComponent::writeln(const char* line) {
        if (line) {
            lines_.push_back(line);
        }
    }

    void DebugComponent::clean() {
        lines_.clear();
    }

    void DebugComponent::early_update() {
        clean();
    }

    int DebugComponent::get_line_count() const {
        return static_cast<int>(lines_.size());
    }

    const char* DebugComponent::get_line(int index) const {
        if (index >= 0 && index < static_cast<int>(lines_.size())) {
            return lines_[index].c_str();
        }
        return nullptr;
    }

}
