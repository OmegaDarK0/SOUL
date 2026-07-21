#include "render.hpp"
#include "utils.hpp"

namespace soul {
    void render::draw_entities(registry& world, const VoidWindow* window,
        const float view_x, const float view_y, const float view_ppu,
        const float center_x, const float center_y) {
        for (const entity e : world.get_view<sprite, size, transform>()) {
            const auto& c_sprite = world.get_component<sprite>(e);
            const auto& c_size = world.get_component<size>(e);
            const auto& c_trans = world.get_component<transform>(e);
            const vec4u8 color = utils::to_u8(c_sprite.color);
            const float size_w = c_size.dimension.x * c_trans.scale.x * view_ppu;
            const float size_h = c_size.dimension.y * c_trans.scale.y * view_ppu;
            const float screen_x = (c_trans.position.x - view_x) * view_ppu + center_x - size_w / 2.0f;
            const float screen_y = (c_trans.position.y - view_y) * view_ppu + center_y - size_h / 2.0f;
            void_render_rect(window, screen_x, screen_y,
                size_w, size_h, color.r, color.g, color.b, color.a, true);
        }
    }
    void render::update(registry& world, const VoidWindow* window) {
        const auto [width, height, ppu] = utils::get_engine_config(world);
        void_render_clear(window, 0, 0, 0, 255);
        void_render_rect(window, 0, 0, width, height, 30, 30, 30, 255, true);
        float view_x = 0.0f;
        float view_y = 0.0f;
        float view_ppu = 1.0f;
        for (const entity e : world.get_view<viewport, transform>()) {
            const auto& c_trans = world.get_component<transform>(e);
            const auto& c_view = world.get_component<viewport>(e);
            view_x = c_trans.position.x;
            view_y = c_trans.position.y;
            view_ppu = c_view.ppu;
            break;
        }
        const float center_x = width / 2.0f;
        const float center_y = height / 2.0f;
        const float arena_w = width / ppu;
        const float arena_h = height / ppu;
        const float screen_w = arena_w * view_ppu;
        const float screen_h = arena_h * view_ppu;
        const float ground_x = (0.0f - view_x) * view_ppu + center_x - screen_w / 2.0f;
        const float ground_y = (0.0f - view_y) * view_ppu + center_y - screen_h / 2.0f;
        void_render_rect(window, ground_x, ground_y, screen_w, screen_h, 100, 100, 100, 255, true);
        draw_entities(world, window, view_x, view_y, view_ppu, center_x, center_y);
        void_render_present(window);
    }
}
