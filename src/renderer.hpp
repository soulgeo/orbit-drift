#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "scene.hpp"
#include "game_object.hpp"
struct RendererImpl;

class Renderer {
public:
    Renderer(Scene* scene);
    ~Renderer();

    Camera camera = {0};
    void update();
    void draw3D();
    void drawUI();
private:
    Scene* scene;
    GameObject* cameraTarget;
    Shader fog;
    RendererImpl* impl_;
    float dt;
};

#endif // !RENDERER_HPP
