#include "input.hpp"
#include "components.hpp"

namespace soul {
#ifndef NDEBUG
    static bool s_debug_enabled = false;
#endif
    struct key_trigger {
        VoidKeyCode key{};
        bool was_pressed = false;
        bool just_pressed() {
            const bool is_pressed = void_input_is_key_pressed(key);
            const bool triggered = is_pressed && !was_pressed;
            was_pressed = is_pressed;
            return triggered;
        }
    };
    void input::process_player_movement(registry& world) {
        for (const entity e : world.get_view<controller::player, velocity>()) {
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
#ifndef NDEBUG
        static key_trigger t_key{VOID_KEY_T};
        static key_trigger tab_key{VOID_KEY_TAB};
        if (t_key.just_pressed()) {
            for (const entity e : world.get_view<viewport, transform>()) {
                auto&[ppu, is_tracking] = world.get_component<viewport>(e);
                auto& t = world.get_component<transform>(e);
                is_tracking = !is_tracking;
                if (is_tracking) {
                    VOID_LOG_INFO("Camera Tracking : ENABLE");
                } else {
                    t.position.x = 0; t.position.y = 0;
                    VOID_LOG_INFO("Camera Tracking : DISABLE");
                }
            }
        }
        if (tab_key.just_pressed()) {
            s_debug_enabled = !s_debug_enabled;
            VOID_LOG_INFO(s_debug_enabled ? "Debug Draw : ENABLED" : "Debug Draw : DISABLED");
        }
#else
        (void)world;
#endif
    }
    void input::update(registry& world) {
        process_player_movement(world);
        process_global_controls(world);
    }
    bool input::is_debug_enabled() {
#ifndef NDEBUG
        return s_debug_enabled;
#else
        return false;
#endif
    }
}
