#ifndef SOUL_PHYSICS_HPP
#define SOUL_PHYSICS_HPP

#include "ecs.hpp"

namespace soul {
    struct physics {
        static void update(registry& world, float dt);
    };
}

#endif //SOUL_PHYSICS_HPP
