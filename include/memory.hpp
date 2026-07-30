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
    template <typename T>
    T* arena_make_array(const size_t count) {
        void* ptr = void_arena_alloc(count * sizeof(T), alignof(T));
        if (ptr == nullptr) return nullptr;
        T* arr = static_cast<T*>(ptr);
        if constexpr (!std::is_trivially_default_constructible_v<T>) {
            for (size_t i = 0; i < count; ++i) {
                new(&arr[i]) T();
            }
        }
        return arr;
    }
    template <typename T>
    T* frame_make_array(const size_t count) {
        void* ptr = void_frame_alloc(count * sizeof(T), alignof(T));
        if (ptr == nullptr) return nullptr;
        T* arr = static_cast<T*>(ptr);
        if constexpr (!std::is_trivially_default_constructible_v<T>) {
            for (size_t i = 0; i < count; ++i) {
                new(&arr[i]) T();
            }
        }
        return arr;
    }
}

#endif //SOUL_MEMORY_HPP
