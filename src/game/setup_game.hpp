#pragma once

#include <memory>
#include <sputnik.hpp>

namespace OrbitDrift {
    void bind_inputs(Sputnik::Engine& engine);
    std::unique_ptr<Sputnik::Scene> create_main_scene(Sputnik::Engine* engine);
}
