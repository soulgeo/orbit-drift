#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "game.hpp"
#include "game_object.hpp"
struct RendererImpl;

class Renderer {
public:
    Renderer(Game& game);
    ~Renderer();

    Camera camera = {0};
    void update(Game& game);
    void draw3D(const Game& game);
    void drawUI(const Game& game);
private:
    GameObject* cameraTarget;
    Shader fog;
    RendererImpl* impl_;
};

#endif // !RENDERER_HPP
