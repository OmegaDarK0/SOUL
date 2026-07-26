#ifndef SOUL_RENDER_HPP
#define SOUL_RENDER_HPP

#include "ecs.hpp"
#include "asset.hpp"

namespace soul {
    struct render {
        static void init(uint32 max_commands);
        static void draw_entities(registry& world, const VoidWindow* window, const asset_manager& assets,
            float view_x, float view_y, float view_ppu, float center_x, float center_y);
        static void draw_debug(registry& world, const VoidWindow* window,
            float view_x, float view_y, float view_ppu, float center_x, float center_y);
        static void update(registry& world, const VoidWindow* window, const asset_manager& assets);
    };
}

#endif //SOUL_RENDER_HPP
