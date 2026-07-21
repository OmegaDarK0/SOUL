#ifndef SOUL_COLLISION_HPP
#define SOUL_COLLISION_HPP

#include "ecs.hpp"

namespace soul {
    struct collision {
        static void resolve_world_bounds(registry& world);
        static void resolve_solids(registry& world);
        static void update(registry& world);
    };
}

#endif //SOUL_COLLISION_HPP
