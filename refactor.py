import os
import re

ENGINE_HEADERS = {
    "engine.hpp": "core/engine.hpp",
    "timer.hpp": "core/timer.hpp",
    "input_handler.hpp": "core/input_handler.hpp",
    "resource_manager.hpp": "core/resource_manager.hpp",
    "component.hpp": "ecs/component.hpp",
    "game_object.hpp": "ecs/game_object.hpp",
    "scene.hpp": "ecs/scene.hpp",
    "transform_component.hpp": "ecs/transform_component.hpp",
    "physics.hpp": "physics/physics.hpp",
    "physics_component.hpp": "physics/physics_component.hpp",
    "collider_component.hpp": "physics/collider_component.hpp",
    "renderer.hpp": "rendering/renderer.hpp",
    "renderable_component.hpp": "rendering/renderable_component.hpp",
    "camera_component.hpp": "rendering/camera_component.hpp",
    "debug_component.hpp": "rendering/debug_component.hpp"
}

ENGINE_CLASSES = [
    "Engine", "Timer", "InputHandler", "ResourceManager",
    "Component", "GameObject", "Scene", "TransformComponent",
    "Physics", "PhysicsComponent", "ColliderComponent",
    "Renderer", "RenderableComponent", "CameraComponent", "DebugComponent",
    "UserInput"
]

def update_includes(content):
    for old, new in ENGINE_HEADERS.items():
        # Match #include "old" but not #include "something/old"
        content = re.sub(fr'#include\s+"{re.escape(old)}"', f'#include "{new}"', content)
    return content

def process_engine_file(path):
    with open(path, 'r') as f:
        content = f.read()
    
    content = update_includes(content)
    
    if path.endswith(".hpp"):
        # Inside guards
        match = re.search(r'(#define\s+\w+)', content)
        if match:
            pos = match.end()
            # Avoid inserting into existing namespace if we run twice
            if "namespace sputnik {" not in content:
                content = content[:pos] + "\n\nnamespace sputnik {\n" + content[pos:]
                # Find last #endif
                last_endif = content.rfind("#endif")
                if last_endif != -1:
                    content = content[:last_endif] + "} // namespace sputnik\n\n" + content[last_endif:]
                else:
                    content += "\n} // namespace sputnik\n"
        else:
            # No guard found
            if "namespace sputnik {" not in content:
                content = "namespace sputnik {\n\n" + content + "\n\n} // namespace sputnik\n"
    else:
        # .cpp file, after includes
        if "namespace sputnik {" not in content:
            lines = content.splitlines()
            last_include_idx = -1
            for i, line in enumerate(lines):
                if line.strip().startswith("#include"):
                    last_include_idx = i
            
            insert_idx = last_include_idx + 1
            lines.insert(insert_idx, "\nnamespace sputnik {")
            lines.append("\n} // namespace sputnik")
            content = "\n".join(lines)
        
    with open(path, 'w') as f:
        f.write(content)

def process_game_file(path):
    with open(path, 'r') as f:
        content = f.read()
    
    content = update_includes(content)
    
    if path.endswith(".hpp"):
        # Prefix engine classes with sputnik::
        for cls in ENGINE_CLASSES:
            # Use negative lookbehind to avoid sputnik::sputnik:: and other pitfalls
            pattern = fr'(?<!sputnik::)(?<![a-zA-Z0-9_])\b{cls}\b(?![a-zA-Z0-9_])'
            content = re.sub(pattern, f'sputnik::{cls}', content)
    else:
        # .cpp file, add using namespace sputnik
        if "using namespace sputnik;" not in content:
            lines = content.splitlines()
            last_include_idx = -1
            for i, line in enumerate(lines):
                if line.strip().startswith("#include"):
                    last_include_idx = i
            
            insert_idx = last_include_idx + 1
            lines.insert(insert_idx, "\nusing namespace sputnik;")
            content = "\n".join(lines)
        
    with open(path, 'w') as f:
        f.write(content)

# Main execution
for root, _, files in os.walk("src/engine"):
    for f in files:
        if f.endswith((".hpp", ".cpp")):
            process_engine_file(os.path.join(root, f))

for root, _, files in os.walk("src/game"):
    for f in files:
        if f.endswith((".hpp", ".cpp")):
            process_game_file(os.path.join(root, f))
