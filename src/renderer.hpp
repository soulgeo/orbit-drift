#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "game.hpp"
struct RendererImpl;

class Renderer {
public:
    Renderer(Game& game);
    ~Renderer();

    void update(Game& game);
    void draw(Vector3 cameraPos, const Game& game);
private:
    Shader fog;
    RendererImpl* impl_;
};

#endif // !RENDERER_HPP
