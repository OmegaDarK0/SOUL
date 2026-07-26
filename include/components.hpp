#ifndef SOUL_COMPONENTS_HPP
#define SOUL_COMPONENTS_HPP

#include "math.hpp"
#include "asset.hpp"

namespace soul {
    namespace config {
        struct window {
            uint32 logic_width = 0;
            uint32 logic_height = 0;
        };
        struct physics {
            float world_width = 0.0f;
            float world_height = 0.0f;
        };
    }
    namespace controller {
        struct player{};
    }
    struct viewport {
        float ppu = 1.0f;
        bool is_tracking = false;
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
    struct body {
        float imass = 1.0f, bounciness = 0.0f;
        bool is_ghost = false;
    };
    struct size {
        vec2f dimension = {{{1.0f, 1.0f}}};
    };
    struct sprite {
        asset texture = {};
        vec4f color = {{{1.0f, 1.0f, 1.0f, 1.0f}}};
        vec2f scroll = {{{1.0f, 1.0f}}};
    };
}

#endif //SOUL_COMPONENTS_HPP
