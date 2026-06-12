#include "debug_component.hpp"
#include "component.hpp"
#include "game_object.hpp"
#include <memory>
#include <vector>
#include <string>

struct DebugComponent::Impl {
    std::vector<std::string> lines;
};

DebugComponent::DebugComponent(GameObject* owner, Renderer* renderer) : Component(owner) {
    impl_ = std::make_unique<Impl>();
    renderer->register_debug(this);
}

DebugComponent::~DebugComponent() = default;

void DebugComponent::writeln(const char* line) {
    if (line) {
        impl_->lines.push_back(line);
    }
}

void DebugComponent::clean() {
    impl_->lines.clear();
}

void DebugComponent::early_update() {
    clean();
}

int DebugComponent::get_line_count() const {
    return static_cast<int>(impl_->lines.size());
}

const char* DebugComponent::get_line(int index) const {
    if (index >= 0 && index < static_cast<int>(impl_->lines.size())) {
        return impl_->lines[index].c_str();
    }
    return nullptr;
}
