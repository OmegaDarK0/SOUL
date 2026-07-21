#include "collision.hpp"
#include "utils.hpp"

namespace soul {
    void collision::resolve_world_bounds(registry& world) {
        const auto [width, height, ppu] = utils::get_engine_config(world);
        const float half_world_w = width / ppu / 2.0f;
        const float half_world_h = height / ppu / 2.0f;
        const float world_min_x = -half_world_w;
        const float world_max_x = half_world_w;
        const float world_min_y = -half_world_h;
        const float world_max_y = half_world_h;
        for (const entity e : world.get_view<player_controller, transform, size>()) {
            auto& trans = world.get_component<transform>(e);
            const auto&[dimension] = world.get_component<size>(e);
            const float half_ent_w = dimension.x * trans.scale.x / 2.0f;
            const float half_ent_h = dimension.y * trans.scale.y / 2.0f;
            if (trans.position.x - half_ent_w < world_min_x) {
                trans.position.x = world_min_x + half_ent_w;
            }
            else if (trans.position.x + half_ent_w > world_max_x) {
                trans.position.x = world_max_x - half_ent_w;
            }
            if (trans.position.y - half_ent_h < world_min_y) {
                trans.position.y = world_min_y + half_ent_h;
            }
            else if (trans.position.y + half_ent_h > world_max_y) {
                trans.position.y = world_max_y - half_ent_h;
            }
        }
    }
    void collision::resolve_solids(registry& world) {
        for (const entity p_entity : world.get_view<player_controller, transform, size>()) {
            auto& p_t = world.get_component<transform>(p_entity);
            const auto& p_s = world.get_component<size>(p_entity);
            for (const entity obstacle : world.get_view<collider, transform, size>()) {
                if (p_entity == obstacle) continue;
                const auto& o_t = world.get_component<transform>(obstacle);
                const auto& o_s = world.get_component<size>(obstacle);
                float overlap_x = 0.0f;
                float overlap_y = 0.0f;
                float dx = 0.0f;
                float dy = 0.0f;
                if (utils::check_aabb(p_t, p_s, o_t, o_s, overlap_x, overlap_y, dx, dy)) {
                    if (overlap_x < overlap_y) {
                        if (dx > 0.0f) p_t.position.x += overlap_x;
                        else p_t.position.x -= overlap_x;
                    } else {
                        if (dy > 0.0f) p_t.position.y += overlap_y;
                        else p_t.position.y -= overlap_y;
                    }
                }
            }
        }
    }
    void collision::update(registry& world) {
        resolve_world_bounds(world);
        resolve_solids(world);
    }
}
