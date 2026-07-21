#ifndef SOUL_COMPONENTS_HPP
#define SOUL_COMPONENTS_HPP

#include "math.hpp"

namespace soul {
    struct engine_config {
        uint32 width = 800, height = 600;
        float ppu = 64.0f;
    };
    struct player_controller {};
    struct collider {};
    struct viewport {
        float ppu = 64.0f;
        bool is_tracking = false;
    };
    struct sprite {
        vec4f color = {{{1.0f, 1.0f, 1.0f, 1.0f}}};
    };
    struct transform {
        vec3f
        position = {{{0.0f, 0.0f, 0.0f}}},
        rotation = {{{0.0f, 0.0f, 0.0f}}},
        scale = {{{1.0f, 1.0f, 1.0f}}};
    };
    struct velocity {
        vec3f direction = {{{0.0f, 0.0f, 0.0f}}};
        float speed = 0.0f;
    };
    struct size {
        vec2f dimension = {{{1.0f, 1.0f}}};
    };
}

#endif //SOUL_COMPONENTS_HPP
