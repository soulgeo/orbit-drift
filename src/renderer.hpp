#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "scene.hpp"
#include "game_object.hpp"
struct RendererImpl;

class Renderer {
public:
    Renderer(Scene& game);
    ~Renderer();

    Camera camera = {0};
    void update(Scene& scene);
    void draw3D(const Scene& scene);
    void drawUI(const Scene& scene);
private:
    GameObject* cameraTarget;
    Shader fog;
    RendererImpl* impl_;
};

#endif // !RENDERER_HPP
