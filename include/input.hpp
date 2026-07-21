#ifndef SOUL_INPUT_HPP
#define SOUL_INPUT_HPP

#include "ecs.hpp"

namespace soul {
    struct input {
        static void process_player_movement(registry& world);
        static void process_global_controls(registry& world);
        static void update(registry& world);
    };
}

#endif //SOUL_INPUT_HPP
