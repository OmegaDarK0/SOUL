#include "collision.hpp"
#include "scene.hpp"
#include "job.hpp"
#include "memory.hpp"
#include "utils.hpp"

namespace soul {
    struct collision_job_data {
        registry* world;
        const entity* active_entities;
        uint32 start_idx;
        uint32 end_idx;
    };
    static entity* s_active_entities = nullptr;
    static uint32 s_max_entities = 0;
    void collision::init(const uint32 max_entities) {
        s_max_entities = max_entities;
        s_active_entities = arena_make_array<entity>(max_entities);
        VOID_ASSERT(s_active_entities != nullptr);
    }
    static void collision_worker_task(void* user_data) {
        const auto* data = static_cast<collision_job_data*>(user_data);
        registry& world = *data->world;
        for (uint32 idx = data->start_idx; idx < data->end_idx; ++idx) {
            const entity p_entity = data->active_entities[idx];
            auto& p_trans = world.get_component<transform>(p_entity);
            const auto& p_size = world.get_component<size>(p_entity);
            auto&[direction, speed] = world.get_component<velocity>(p_entity);
            const auto&[imass, bounciness, is_ghost] = world.get_component<body>(p_entity);
            if (is_ghost) continue;
            const int cx = static_cast<int>((p_trans.position.x + static_cast<float>(scene::grid_width) * scene::cell_size / 2.0f) / scene::cell_size);
            const int cy = static_cast<int>((p_trans.position.y + static_cast<float>(scene::grid_height) * scene::cell_size / 2.0f) / scene::cell_size);
            for (int ny = cy - 1; ny <= cy + 1; ++ny) {
                for (int nx = cx - 1; nx <= cx + 1; ++nx) {
                    if (nx < 0 || ny < 0 || nx >= static_cast<int>(scene::grid_width) || ny >= static_cast<int>(scene::grid_height)) continue;
                    const spatial_entry* entries = nullptr;
                    uint32 count = 0;
                    scene::get_cell_entities(nx, ny, entries, count);
                    for (uint32 i = 0; i < count; ++i) {
                        const entity obstacle = entries[i].e;
                        if (p_entity == obstacle) continue;
                        const auto& o_trans = world.get_component<transform>(obstacle);
                        const auto& o_size = world.get_component<size>(obstacle);
                        const auto& o_col = world.get_component<body>(obstacle);
                        if (o_col.is_ghost) continue;
                        float overlap_x = 0.0f, overlap_y = 0.0f, dx = 0.0f, dy = 0.0f;
                        if (utils::check_aabb(p_trans, p_size, o_trans, o_size, overlap_x, overlap_y, dx, dy)) {
                            const float sum_imass = imass + o_col.imass;
                            if (const float push_ratio = sum_imass == 0.0f ? 1.0f : imass / sum_imass; push_ratio > 0.0f) {
                                if (overlap_x < overlap_y) {
                                    const float move_x = dx > 0.0f ? overlap_x : -overlap_x;
                                    p_trans.position.x += move_x * push_ratio;
                                    if (bounciness == 0.0f) direction.x = 0.0f;
                                    else direction.x *= -bounciness;
                                }
                                else {
                                    const float move_y = dy > 0.0f ? overlap_y : -overlap_y;
                                    p_trans.position.y += move_y * push_ratio;
                                    if (bounciness == 0.0f) direction.y = 0.0f;
                                    else direction.y *= -bounciness;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    void collision::resolve_world_bounds(registry& world) {
        const auto [world_width, world_height] = world.get_context<config::physics>();
        const float half_world_w = world_width / 2.0f;
        const float half_world_h = world_height / 2.0f;
        const float world_min_x = -half_world_w;
        const float world_max_x = half_world_w;
        const float world_min_y = -half_world_h;
        const float world_max_y = half_world_h;
        for (const entity e : world.get_view<transform, size, velocity, body>()) {
            auto& trans = world.get_component<transform>(e);
            auto&[direction, speed] = world.get_component<velocity>(e);
            const auto&[dimension] = world.get_component<size>(e);
            const auto& col = world.get_component<body>(e);
            const float half_ent_w = dimension.x * trans.scale.x / 2.0f;
            const float half_ent_h = dimension.y * trans.scale.y / 2.0f;
            if (trans.position.x - half_ent_w < world_min_x) {
                trans.position.x = world_min_x + half_ent_w;
                direction.x *= -col.bounciness;
            }
            else if (trans.position.x + half_ent_w > world_max_x) {
                trans.position.x = world_max_x - half_ent_w;
                direction.x *= -col.bounciness;
            }
            if (trans.position.y - half_ent_h < world_min_y) {
                trans.position.y = world_min_y + half_ent_h;
                direction.y *= -col.bounciness;
            }
            else if (trans.position.y + half_ent_h > world_max_y) {
                trans.position.y = world_max_y - half_ent_h;
                direction.y *= -col.bounciness;
            }
        }
    }
    void collision::resolve_solids(registry& world) {
        uint32 active_count = 0;
        for (const entity p_entity : world.get_view<transform, size, velocity, body>()) {
            if (active_count < s_max_entities) {
                s_active_entities[active_count++] = p_entity;
            }
        }
        if (active_count == 0) return;
        const uint32 num_jobs = void_system_get_core_count();
        const uint32 chunk_size = (active_count + num_jobs - 1) / num_jobs;
        static collision_job_data s_jobs[64];
        uint32 jobs_dispatched = 0;
        for (uint32 i = 0; i < active_count; i += chunk_size) {
            uint32 end = i + chunk_size;
            if (end > active_count) end = active_count;
            s_jobs[jobs_dispatched] = collision_job_data{&world, s_active_entities, i, end};
            job::dispatch(collision_worker_task, &s_jobs[jobs_dispatched]);
            jobs_dispatched++;
        }
        job::wait();
    }
    void collision::update(registry& world) {
        resolve_solids(world);
        resolve_world_bounds(world);
    }
}
