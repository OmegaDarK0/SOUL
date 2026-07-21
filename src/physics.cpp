#include "physics.hpp"
#include "components.hpp"

namespace soul {
    void physics::update(registry& world, const float dt) {
        for (const entity e : world.get_view<transform, velocity>()) {
            auto& t = world.get_component<transform>(e);
            auto&[direction, speed] = world.get_component<velocity>(e);
            t.position += direction * (speed * dt);
        }
    }
}
