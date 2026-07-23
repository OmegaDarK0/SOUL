#ifndef SOUL_COMPONENTS_HPP
#define SOUL_COMPONENTS_HPP

#include "math.hpp"
#include "asset.hpp"

namespace soul {
    struct engine_config {
        uint32 width = 800, height = 600;
        float ppu = 64.0f;
    };
    struct world_bounds {
        float width = 12.5f, height = 9.375f;
    };
    struct player_controller {};
    struct viewport {
        float ppu = 64.0f;
        bool is_tracking = false;
    };
    struct sprite {
        vec4f color = {{{1.0f, 1.0f, 1.0f, 1.0f}}};
        asset texture = {};
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
    struct body {
        float imass = 1.0f, bounciness = 0.0f;
        bool is_ghost = false;
    };
}

#endif //SOUL_COMPONENTS_HPP
