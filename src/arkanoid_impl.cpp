#include "arkanoid_impl.h"

#include <GLFW/glfw3.h>

WorldSpace Entity::world_space = {};

#ifdef USE_ARKANOID_IMPL
Arkanoid* create_arkanoid()
{
    return new ArkanoidImpl();
}
#endif

ArkanoidImpl::ArkanoidImpl()
{
    balls.push_back(Ball());
    balls.push_back(Ball());
    // balls.push_back(Ball());
}

void ArkanoidImpl::reset(const ArkanoidSettings& settings)
{
    // TODO:
    // Implement your game world, bricks and
    // carriage initialization
    // ...

    // TODO:
    // remove demo code
    Entity::world_space.world_size.x = settings.world_size[0];
    Entity::world_space.world_size.y = settings.world_size[1];

    for (auto& ball : balls)
    {
        ball.reset(settings);
    }
}

void ArkanoidImpl::update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed)
{
    Entity::world_space.world_to_screen =
        Vect(io.DisplaySize.x / Entity::world_space.world_size.x, io.DisplaySize.y / Entity::world_space.world_size.y);

    // TODO:
    // Implement you Arkanoid user input handling
    // and game logic.
    // ...

    // TODO:
    // remove demo code
    // demo_update(io, debug_data, elapsed);

    for (auto& ball : balls)
    {
        ball.update(io, debug_data, elapsed);
    }
}

void ArkanoidImpl::draw(ImGuiIO& io, ImDrawList& draw_list)
{
    // TODO:
    // Implement you Arkanoid drawing
    // ...

    // TODO:
    // remove demo code
    // demo_draw(io, draw_list);

    for (auto& ball : balls)
    {
        ball.draw(io, draw_list);
    }
}

// void ArkanoidImpl::demo_update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed)
// {
//     demo_world_to_screen = Vect(io.DisplaySize.x / demo_world_size.x, io.DisplaySize.y / demo_world_size.y);

//     // process user input
//     if (io.KeysDown[GLFW_KEY_A])
//         demo_ball_velocity.x -= 1.0f;

//     if (io.KeysDown[GLFW_KEY_D])
//         demo_ball_velocity.x += 1.0f;

//     if (io.KeysDown[GLFW_KEY_W])
//         demo_ball_velocity.y -= 1.0f;

//     if (io.KeysDown[GLFW_KEY_S])
//         demo_ball_velocity.y += 1.0f;

//     if (io.KeysDown[GLFW_KEY_ESCAPE])
//         demo_ball_velocity = Vect(demo_ball_initial_speed);

// }

// void ArkanoidImpl::demo_draw(ImGuiIO& io, ImDrawList& draw_list)
// {
//     Vect screen_pos = demo_ball_position * demo_world_to_screen;
//     float screen_radius = demo_ball_radius * demo_world_to_screen.x;
//     draw_list.AddCircleFilled(screen_pos, screen_radius, ImColor(100, 255, 100));

//     // draw_list.AddRectFilled(Vect(0, 0), Vect(io.DisplaySize.x / 2, io.DisplaySize.y / 2), ImColor(255, 255, 255));
// }

// void ArkanoidImpl::demo_add_debug_hit(ArkanoidDebugData& debug_data, const Vect& world_pos, const Vect& normal)
// {
//     ArkanoidDebugData::Hit hit;
//     hit.screen_pos = world_pos * world_to_screen;
//     hit.normal = normal;
//     debug_data.hits.push_back(std::move(hit));
// }