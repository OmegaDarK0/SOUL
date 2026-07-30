#include <algorithm>
#include "scene.hpp"
#include "components.hpp"
#include "memory.hpp"

namespace soul {
    uint32 scene::grid_width = 0;
    uint32 scene::grid_height = 0;
    float scene::cell_size = 0.0f;
    uint32 scene::max_cells = 0;
    static spatial_entry* s_entries = nullptr;
    static uint32* s_cell_starts = nullptr;
    static uint32* s_cell_counts = nullptr;
    static uint32 s_entry_count = 0;
    static uint32 s_max_entities = 0;
    void scene::init(const uint32 max_entities, const uint32 grid_w, const uint32 grid_h, const float cell_s) {
        s_max_entities = max_entities;
        grid_width = grid_w;
        grid_height = grid_h;
        cell_size = cell_s;
        max_cells = grid_width * grid_height;
        s_entries = arena_make_array<spatial_entry>(max_entities);
        s_cell_starts = arena_make_array<uint32>(max_cells);
        s_cell_counts = arena_make_array<uint32>(max_cells);
        VOID_ASSERT(s_entries != nullptr && s_cell_starts != nullptr && s_cell_counts != nullptr);
    }
    void scene::build_spatial_grid(registry& world) {
        s_entry_count = 0;
        for (uint32 i = 0; i < max_cells; ++i) {
            s_cell_starts[i] = 0;
            s_cell_counts[i] = 0;
        }
        for (const entity e : world.get_view<transform, body, size>()) {
            if (s_entry_count >= s_max_entities) break;
            const auto& t = world.get_component<transform>(e);
            int cx = static_cast<int>((t.position.x + static_cast<float>(grid_width) * cell_size / 2.0f) / cell_size);
            int cy = static_cast<int>((t.position.y + static_cast<float>(grid_height) * cell_size / 2.0f) / cell_size);
            if (cx < 0) cx = 0;
            if (cy < 0) cy = 0;
            if (cx >= static_cast<int>(grid_width)) cx = static_cast<int>(grid_width) - 1;
            if (cy >= static_cast<int>(grid_height)) cy = static_cast<int>(grid_height) - 1;
            const auto cell_index = cx + cy * grid_width;
            s_entries[s_entry_count] = {cell_index, e};
            s_cell_counts[cell_index]++;
            s_entry_count++;
        }
        std::sort(s_entries, s_entries + s_entry_count, [](const spatial_entry& a, const spatial_entry& b) {
            return a.cell_index < b.cell_index;
        });
        uint32 current_start = 0;
        for (uint32 i = 0; i < max_cells; ++i) {
            s_cell_starts[i] = current_start;
            current_start += s_cell_counts[i];
        }
    }
    void scene::get_cell_entities(const uint32 cell_x, const uint32 cell_y, const spatial_entry*& out_entries, uint32& out_count) {
        if (cell_x >= grid_width || cell_y >= grid_height) {
            out_entries = nullptr;
            out_count = 0;
            return;
        }
        const uint32 cell_index = cell_x + cell_y * grid_width;
        out_entries = &s_entries[s_cell_starts[cell_index]];
        out_count = s_cell_counts[cell_index];
    }
    void scene::update(registry &world) {
        build_spatial_grid(world);
    }
}
