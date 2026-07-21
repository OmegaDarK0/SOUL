#include "input.hpp"
#include "components.hpp"

namespace soul {
    void input::process_player_movement(registry& world) {
        for (const entity e : world.get_view<player_controller, velocity>()) {
            auto&[direction, speed] = world.get_component<velocity>(e);
            direction = {{{0.0f, 0.0f, 0.0f}}};
            if (void_input_is_key_pressed(VOID_KEY_W)) direction.y -= 1.0f;
            if (void_input_is_key_pressed(VOID_KEY_S)) direction.y += 1.0f;
            if (void_input_is_key_pressed(VOID_KEY_A)) direction.x -= 1.0f;
            if (void_input_is_key_pressed(VOID_KEY_D)) direction.x += 1.0f;
            if (direction.norm() > 0.0f) {
                direction = direction.unit();
            }
        }
    }
    void input::process_global_controls(registry& world) {
        static bool was_pressed = false;
        const bool is_pressed = void_input_is_key_pressed(VOID_KEY_T);
        if (is_pressed && !was_pressed) {
            for (const entity e : world.get_view<viewport, transform>()) {
                auto&[ppu, is_tracking] = world.get_component<viewport>(e);
                auto& t = world.get_component<transform>(e);
                is_tracking = !is_tracking;
                if (is_tracking) {
                    VOID_LOG_INFO("Camera Tracking : ENABLE");
                } else {
                    t.position.x = 0;
                    t.position.y = 0;
                    VOID_LOG_INFO("Camera Tracking : DISABLE");
                }
            }
        }
        was_pressed = is_pressed;
    }
    void input::update(registry& world) {
        process_player_movement(world);
        process_global_controls(world);
    }
}
