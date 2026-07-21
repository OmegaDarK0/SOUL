#ifndef SOUL_CAMERA_HPP
#define SOUL_CAMERA_HPP

#include "ecs.hpp"

namespace soul {
    struct camera {
        static void update(registry& world);
    };
}

#endif //SOUL_CAMERA_HPP
