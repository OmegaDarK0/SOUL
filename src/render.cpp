#include <algorithm>
#include "render.hpp"
#include "scene.hpp"
#include "input.hpp"
#include "memory.hpp"
#include "utils.hpp"

namespace soul {
    struct render_command {
        float x{}, y{}, z{}, w{}, h{};
        uint8 r{}, g{}, b{}, a{};
        asset texture;
    };
    static render_command* s_render_queue = nullptr;
    static uint32 s_max_render_commands = 0;
    void render::init(const uint32 max_commands) {
        s_max_render_commands = max_commands;
        s_render_queue = arena_make_array<render_command>(max_commands);
        VOID_ASSERT(s_render_queue != nullptr);
    }
    void render::draw_entities(registry& world, const VoidWindow* window, const asset_manager& assets,
        const float view_x, const float view_y, const float view_ppu,
        const float center_x, const float center_y) {
        uint32 cmd_count = 0;
        for (const entity e : world.get_view<sprite, size, transform>()) {
            const auto& c_sprite = world.get_component<sprite>(e);
            const auto& c_size = world.get_component<size>(e);
            const auto& c_trans = world.get_component<transform>(e);
            const vec4u8 color = utils::to_u8(c_sprite.color);
            const float size_w = c_size.dimension.x * c_trans.scale.x * view_ppu;
            const float size_h = c_size.dimension.y * c_trans.scale.y * view_ppu;
            const float screen_x = (c_trans.position.x - view_x * c_sprite.scroll.x) * view_ppu + center_x - size_w / 2.0f;
            const float screen_y = (c_trans.position.y - view_y * c_sprite.scroll.y) * view_ppu + center_y - size_h / 2.0f;
            s_render_queue[cmd_count++] = {
                screen_x, screen_y, c_trans.position.z, size_w, size_h,
                color.r, color.g, color.b, color.a,
                c_sprite.texture
            };
            std::sort(s_render_queue, s_render_queue + cmd_count, [](const render_command& a, const render_command& b) {
                return a.z < b.z;
            });
            for (uint32 i = 0; i < cmd_count; ++i) {
                const auto& cmd = s_render_queue[i];
                if (const VoidTexture* texture = assets.get_texture(cmd.texture)) {
                    const int texture_w = void_texture_get_width(texture);
                    const int texture_h = void_texture_get_height(texture);
                    void_texture_draw(window, texture, 0, 0, texture_w, texture_h, cmd.x, cmd.y, cmd.w, cmd.h);
                } else {
                    void_render_rect(window, cmd.x, cmd.y, cmd.w, cmd.h, cmd.r, cmd.g, cmd.b, cmd.a, true);
                }
            }
        }
    }
    void render::draw_debug(registry& world, const VoidWindow* window,
        const float view_x, const float view_y, const float view_ppu,
        const float center_x, const float center_y) {
        const float half_grid_w = static_cast<float>(scene::grid_width) * scene::cell_size / 2.0f;
        const float half_grid_h = static_cast<float>(scene::grid_height) * scene::cell_size / 2.0f;
        const float start_x = -half_grid_w;
        const float start_y = -half_grid_h;
        for (uint32 i = 0; i <= scene::grid_width; ++i) {
            const float world_x = start_x + static_cast<float>(i) * scene::cell_size;
            const float screen_x = (world_x - view_x) * view_ppu + center_x;
            const float screen_y_start = (start_y - view_y) * view_ppu + center_y;
            const float screen_y_end = (start_y + static_cast<float>(scene::grid_height) * scene::cell_size - view_y) * view_ppu + center_y;
            void_render_line(window, screen_x, screen_y_start, screen_x, screen_y_end, 50, 50, 50, 255);
        }
        for (uint32 i = 0; i <= scene::grid_height; ++i) {
            const float world_y = start_y + static_cast<float>(i) * scene::cell_size;
            const float screen_y = (world_y - view_y) * view_ppu + center_y;
            const float screen_x_start = (start_x - view_x) * view_ppu + center_x;
            const float screen_x_end = (start_x + static_cast<float>(scene::grid_width) * scene::cell_size - view_x) * view_ppu + center_x;
            void_render_line(window, screen_x_start, screen_y, screen_x_end, screen_y, 50, 50, 50, 255);
        }
        for (const entity e : world.get_view<transform, size>()) {
            const auto& t = world.get_component<transform>(e);
            const auto&[dimension] = world.get_component<size>(e);
            const float w = dimension.x * t.scale.x * view_ppu;
            const float h = dimension.y * t.scale.y * view_ppu;
            const float x = (t.position.x - view_x) * view_ppu + center_x - w / 2.0f;
            const float y = (t.position.y - view_y) * view_ppu + center_y - h / 2.0f;
            void_render_line(window, x, y, x + w, y, 255, 0, 0, 255);
            void_render_line(window, x, y + h, x + w, y + h, 255, 0, 0, 255);
            void_render_line(window, x, y, x, y + h, 0, 0, 255, 255);
            void_render_line(window, x + w, y, x + w, y + h, 0, 0, 255, 255);
            //void_render_rect(window, x, y, w, h, 0, 255, 0, 255, false);
        }
    }
    void render::update(registry& world, const VoidWindow* window, const asset_manager& assets) {
        const auto [logic_width, logic_height] = world.get_context<config::window>();
        void_render_clear(window, 0, 0, 0, 255);
        float view_x = 0.0f;
        float view_y = 0.0f;
        float view_ppu = 1.0f;
        for (const entity e : world.get_view<viewport, transform>()) {
            const auto& c_trans = world.get_component<transform>(e);
            const auto&[ppu, is_tracking] = world.get_component<viewport>(e);
            view_x = c_trans.position.x;
            view_y = c_trans.position.y;
            view_ppu = ppu;
            break;
        }
        const float center_x = static_cast<float>(logic_width) / 2.0f;
        const float center_y = static_cast<float>(logic_height) / 2.0f;
        draw_entities(world, window, assets, view_x, view_y, view_ppu, center_x, center_y);
#ifndef NDEBUG
        if (input::is_debug_enabled()) {
            draw_debug(world, window, view_x, view_y, view_ppu, center_x, center_y);
        }
#endif
        void_render_present(window);
    }
}
