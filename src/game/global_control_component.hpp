#ifndef GLOBAL_CONTROL_COMPONENT_HPP
#define GLOBAL_CONTROL_COMPONENT_HPP

#include "sputnik/ecs/component.hpp"

namespace Sputnik {
    class GlobalControlComponent : public Component {
    public:
        GlobalControlComponent(GameObject* owner);
        ~GlobalControlComponent() override;

        void early_update() override;
    };
}

#endif // GLOBAL_CONTROL_COMPONENT_HPP
