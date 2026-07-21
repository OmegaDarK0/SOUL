#ifndef SOUL_UTILS_HPP
#define SOUL_UTILS_HPP

#include "components.hpp"

namespace soul::utils {
    struct config_view {
        float width = 800.0f;
        float height = 600.0f;
        float ppu = 64.0f;
    };
    inline config_view get_engine_config(registry& world) {
        config_view cfg;
        for (const entity e : world.get_view<engine_config>()) {
            const auto&[width, height, ppu] = world.get_component<engine_config>(e);
            cfg.width = static_cast<float>(width);
            cfg.height = static_cast<float>(height);
            cfg.ppu = ppu;
            break;
        }
        return cfg;
    }
    inline bool check_aabb(const transform& t1, const size& s1, const transform& t2, const size& s2,
                       float& out_overlap_x, float& out_overlap_y, float& out_dx, float& out_dy) {
        const float half_w1 = s1.dimension.x * t1.scale.x / 2.0f;
        const float half_h1 = s1.dimension.y * t1.scale.y / 2.0f;
        const float half_w2 = s2.dimension.x * t2.scale.x / 2.0f;
        const float half_h2 = s2.dimension.y * t2.scale.y / 2.0f;
        out_dx = t1.position.x - t2.position.x;
        out_dy = t1.position.y - t2.position.y;
        out_overlap_x = half_w1 + half_w2 - std::abs(out_dx);
        out_overlap_y = half_h1 + half_h2 - std::abs(out_dy);
        return out_overlap_x > 0.0f && out_overlap_y > 0.0f;
    }
    template <typename T>
    [[nodiscard]] constexpr vec2u8 to_u8(const vec2<T>& v) {
        return {{
            {
                static_cast<uint8>(v.x * 255.0f),
                static_cast<uint8>(v.y * 255.0f)
            }}};
    }
    template <typename T>
    [[nodiscard]] constexpr vec3u8 to_u8(const vec3<T>& v) {
        return {{
            {
                static_cast<uint8>(v.x * 255.0f),
                static_cast<uint8>(v.y * 255.0f),
                static_cast<uint8>(v.z * 255.0f)
            }}};
    }
    template <typename T>
    [[nodiscard]] constexpr vec4u8 to_u8(const vec4<T>& v) {
        return {{
            {
                static_cast<uint8>(v.x * 255.0f),
                static_cast<uint8>(v.y * 255.0f),
                static_cast<uint8>(v.z * 255.0f),
                static_cast<uint8>(v.w * 255.0f)
            }}};
    }
}

#endif //SOUL_UTILS_HPP
