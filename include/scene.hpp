#ifndef SOUL_SCENE_HPP
#define SOUL_SCENE_HPP

#include "ecs.hpp"

namespace soul {
    struct spatial_entry {
        uint32 cell_index;
        entity e;
    };
    struct scene {
        static uint32 grid_width;
        static uint32 grid_height;
        static float cell_size;
        static uint32 max_cells;
        static void init(uint32 max_entities, uint32 grid_w, uint32 grid_h, float cell_s);
        static void build_spatial_grid(registry& world);
        static void get_cell_entities(uint32 cell_x, uint32 cell_y, const spatial_entry*& out_entries, uint32& out_count);
        static void update(registry& world);
    };
}

#endif //SOUL_SCENE_HPP
