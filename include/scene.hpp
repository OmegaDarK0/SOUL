#ifndef SOUL_SCENE_HPP
#define SOUL_SCENE_HPP

#include "ecs.hpp"

namespace soul {
    struct spatial_entry {
        uint32 cell_index;
        entity e;
    };
    struct scene {
        static constexpr float CELL_SIZE = 2.0f;
        static constexpr uint32 GRID_WIDTH = 100;
        static constexpr uint32 GRID_HEIGHT = 100;
        static constexpr uint32 MAX_CELLS = GRID_WIDTH * GRID_HEIGHT;
        static constexpr uint32 MAX_ENTITIES = 10000;
        static void build_spatial_grid(registry& world);
        static void get_cell_entities(uint32 cell_x, uint32 cell_y, const spatial_entry*& out_entries, uint32& out_count);
        static void update(registry& world);
    };
}

#endif //SOUL_SCENE_HPP
