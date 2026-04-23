#ifndef SOUL_MEMORY_HPP
#define SOUL_MEMORY_HPP

#include <new>
#include <utility>
#include "void.h"

namespace soul {
    template <typename T, typename... Args>
    T *arena_make(Args&&... args) {
        void *ptr = void_arena_alloc(sizeof(T), alignof(T));
        if (ptr == nullptr) return nullptr;
        return new(ptr) T(std::forward<Args>(args)...);
    }
    template <typename T, typename... Args>
    T *frame_make(Args&&... args) {
        void *ptr = void_frame_alloc(sizeof(T), alignof(T));
        if (ptr == nullptr) return nullptr;
        return new(ptr) T(std::forward<Args>(args)...);
    }
}

#endif //SOUL_MEMORY_HPP
