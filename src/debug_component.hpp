#ifndef DEBUG_COMPONENT_HPP
#define DEBUG_COMPONENT_HPP

#include "component.hpp"
#include "renderer.hpp"
#include <vector>
#include <string>

class DebugComponent : public Component{
    std::vector<std::string> lines_;

public:
    DebugComponent(GameObject* owner, Renderer* renderer);
    ~DebugComponent();

    void writeln(const char* line);
    void clean();

    void early_update() override;
    
    int get_line_count() const;
    const char* get_line(int index) const;
};

#endif // DEBUG_HPP
