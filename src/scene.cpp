#include <algorithm>
#include "scene.hpp"
#include "components.hpp"

namespace soul {
    static spatial_entry s_entries[scene::MAX_ENTITIES];
    static uint32 s_entry_count = 0;
    static uint32 s_cell_starts[scene::MAX_CELLS];
    static uint32 s_cell_counts[scene::MAX_CELLS];
    void scene::build_spatial_grid(registry& world) {
        s_entry_count = 0;
        for (uint32 i = 0; i < MAX_CELLS; ++i) {
            s_cell_starts[i] = 0;
            s_cell_counts[i] = 0;
        }
        for (const entity e : world.get_view<transform, body, size>()) {
            if (s_entry_count >= MAX_ENTITIES) break;
            const auto& t = world.get_component<transform>(e);
            int cx = static_cast<int>((t.position.x + GRID_WIDTH * CELL_SIZE / 2.0f) / CELL_SIZE);
            int cy = static_cast<int>((t.position.y + GRID_HEIGHT * CELL_SIZE / 2.0f) / CELL_SIZE);
            if (cx < 0) cx = 0;
            if (cy < 0) cy = 0;
            if (cx >= static_cast<int>(GRID_WIDTH)) cx = GRID_WIDTH - 1;
            if (cy >= static_cast<int>(GRID_HEIGHT)) cy = GRID_HEIGHT - 1;
            const auto cell_index = cx + cy * GRID_WIDTH;
            s_entries[s_entry_count] = {cell_index, e};
            s_cell_counts[cell_index]++;
            s_entry_count++;
        }
        std::sort(s_entries, s_entries + s_entry_count, [](const spatial_entry& a, const spatial_entry& b) {
            return a.cell_index < b.cell_index;
        });
        uint32 current_start = 0;
        for (uint32 i = 0; i < MAX_CELLS; ++i) {
            s_cell_starts[i] = current_start;
            current_start += s_cell_counts[i];
        }
    }
    void scene::get_cell_entities(const uint32 cell_x, const uint32 cell_y, const spatial_entry*& out_entries, uint32& out_count) {
        if (cell_x >= GRID_WIDTH || cell_y >= GRID_HEIGHT) {
            out_entries = nullptr;
            out_count = 0;
            return;
        }
        const uint32 cell_index = cell_x + cell_y * GRID_WIDTH;
        out_entries = &s_entries[s_cell_starts[cell_index]];
        out_count = s_cell_counts[cell_index];
    }
    void scene::update(registry &world) {
        build_spatial_grid(world);
    }
}
