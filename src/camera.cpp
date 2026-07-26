#include "camera.hpp"
#include "components.hpp"

namespace soul {
    void camera::update(registry& world) {
        vec3f target_pos;
        bool has_target = false;
        for (const entity e : world.get_view<controller::player, transform>()) {
            target_pos = world.get_component<transform>(e).position;
            has_target = true;
            break;
        }
        if (!has_target) return;
        for (const entity e : world.get_view<viewport, transform>()) {
            const auto&[ppu, is_tracking] = world.get_component<viewport>(e);
            auto& view_trans = world.get_component<transform>(e);
            if (is_tracking) {
                view_trans.position.x = target_pos.x;
                view_trans.position.y = target_pos.y;
            }
        }
    }
}
