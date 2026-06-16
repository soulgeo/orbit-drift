#ifndef DEBUG_COMPONENT_HPP
#define DEBUG_COMPONENT_HPP

#include "sputnik/ecs/component.hpp"
#include "sputnik/rendering/renderer.hpp"
#include <vector>
#include <string>

namespace Sputnik {

    class DebugComponent : public Component {
        std::vector<std::string> lines_;

    public:
        DebugComponent(GameObject* owner, Renderer* renderer);
        ~DebugComponent();

        void writeln(const char* line);
        void clean();

        void early_update() override;
        
        int line_count() const;
        const char* line(int index) const;
    };

}

#endif // DEBUG_HPP
