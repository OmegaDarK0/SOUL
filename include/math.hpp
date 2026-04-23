#ifndef SOUL_MATH_HPP
#define SOUL_MATH_HPP

#include <cmath>

namespace soul {
    template <typename T>
    struct vec2 {
        union {
            struct { T x, y; };
            struct { T u, v; };
            struct { T s, t; };
            T data[2];
        };
        constexpr vec2 &operator+=(vec2 v) { x += v.x; y += v.y; return *this; }
        constexpr vec2 &operator-=(vec2 v) { x -= v.x; y -= v.y; return *this; }
        constexpr vec2 &operator*=(vec2 v) { x *= v.x; y *= v.y; return *this; }
        constexpr vec2 &operator/=(vec2 v) { x /= v.x; y /= v.y; return *this; }
        constexpr vec2 &operator+=(T s) { x += s; y += s; return *this; }
        constexpr vec2 &operator-=(T s) { x -= s; y -= s; return *this; }
        constexpr vec2 &operator*=(T s) { x *= s; y *= s; return *this; }
        constexpr vec2 &operator/=(T s) { x /= s; y /= s; return *this; }
        constexpr vec2 &operator++() { ++x; ++y; return *this; }
        constexpr vec2 &operator--() { --x; --y; return *this; }
        [[nodiscard]] constexpr vec2 operator+(vec2 v) const { return {x + v.x, y + v.y}; }
        [[nodiscard]] constexpr vec2 operator-(vec2 v) const { return {x - v.x, y - v.y}; }
        [[nodiscard]] constexpr vec2 operator*(vec2 v) const { return {x * v.x, y * v.y}; }
        [[nodiscard]] constexpr vec2 operator/(vec2 v) const { return {x / v.x, y / v.y}; }
        [[nodiscard]] constexpr vec2 operator+(T s) const { return {x + s, y + s}; }
        [[nodiscard]] constexpr vec2 operator-(T s) const { return {x - s, y - s}; }
        [[nodiscard]] constexpr vec2 operator*(T s) const { return {x * s, y * s}; }
        [[nodiscard]] constexpr vec2 operator/(T s) const { return {x / s, y / s}; }
        [[nodiscard]] constexpr bool operator==(vec2 v) const { return x == v.x && y == v.y; }
        [[nodiscard]] constexpr bool operator!=(vec2 v) const { return x != v.x || y != v.y; }
        [[nodiscard]] constexpr vec2 operator-() const { return {-x, -y}; }
        [[nodiscard]] constexpr T dot(vec2 v) const { return x * v.x + y * v.y; }
        [[nodiscard]] constexpr T dist(vec2 v) const {
            return std::sqrt(
                (x - v.x) * (x - v.x) +
                (y - v.y) * (y - v.y)
                );
        }
        [[nodiscard]] constexpr T norm() const { return std::sqrt(x * x + y * y); }
        [[nodiscard]] constexpr vec2 unit() const {
            T length = norm();
            if (length == 0) return {0, 0};
            return {x / length, y / length};
        }
    };
    using vec2d = vec2<double>;
    using vec2f = vec2<float>;
    using vec2i = vec2<int>;
    using vec2u = vec2<unsigned int>;
    template <typename T>
    struct vec3 {
        union {
            struct { T x, y, z; };
            struct { T r, g, b; };
            struct { T s, t, p; };
            T data[3];
        };
        constexpr vec3 &operator+=(vec3 v) { x += v.x; y += v.y; z += v.z; return *this; }
        constexpr vec3 &operator-=(vec3 v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
        constexpr vec3 &operator*=(vec3 v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
        constexpr vec3 &operator/=(vec3 v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
        constexpr vec3 &operator+=(T s) { x += s; y += s; z += s; return *this; }
        constexpr vec3 &operator-=(T s) { x -= s; y -= s; z -= s; return *this; }
        constexpr vec3 &operator*=(T s) { x *= s; y *= s; z *= s; return *this; }
        constexpr vec3 &operator/=(T s) { x /= s; y /= s; z /= s; return *this; }
        constexpr vec3 &operator++() { ++x; ++y; ++z; return *this; }
        constexpr vec3 &operator--() { --x; --y; --z; return *this; }
        [[nodiscard]] constexpr vec3 operator+(vec3 v) const { return {x + v.x, y + v.y, z + v.z}; }
        [[nodiscard]] constexpr vec3 operator-(vec3 v) const { return {x - v.x, y - v.y, z - v.z}; }
        [[nodiscard]] constexpr vec3 operator*(vec3 v) const { return {x * v.x, y * v.y, z * v.z}; }
        [[nodiscard]] constexpr vec3 operator/(vec3 v) const { return {x / v.x, y / v.y, z / v.z}; }
        [[nodiscard]] constexpr vec3 operator+(T s) const { return {x + s, y + s, z + s}; }
        [[nodiscard]] constexpr vec3 operator-(T s) const { return {x - s, y - s, z - s}; }
        [[nodiscard]] constexpr vec3 operator*(T s) const { return {x * s, y * s, z * s}; }
        [[nodiscard]] constexpr vec3 operator/(T s) const { return {x / s, y / s, z / s}; }
        [[nodiscard]] constexpr bool operator==(vec3 v) const { return x == v.x && y == v.y && z == v.z; }
        [[nodiscard]] constexpr bool operator!=(vec3 v) const { return x != v.x || y != v.y || z != v.z; }
        [[nodiscard]] constexpr vec3 operator-() const { return {-x, -y, -z}; }
        [[nodiscard]] constexpr T dot(vec3 v) const { return x * v.x + y * v.y + z * v.z; }
        [[nodiscard]] constexpr T dist(vec3 v) const {
            return std::sqrt(
                (x - v.x) * (x - v.x) +
                (y - v.y) * (y - v.y) +
                (z - v.z) * (z - v.z)
                );
        }
        [[nodiscard]] constexpr T norm() const { return std::sqrt(x * x + y * y + z * z); }
        [[nodiscard]] constexpr vec3 unit() const {
            T length = norm();
            if (length == 0) return {0, 0, 0};
            return {x / length, y / length, z / length};
        }
        [[nodiscard]] constexpr vec3 cross(vec3 v) const {
            return {
                y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x
            };
        }
    };
    using vec3d = vec3<double>;
    using vec3f = vec3<float>;
    using vec3i = vec3<int>;
    using vec3u = vec3<unsigned int>;
    template <typename T>
    struct vec4 {
        union {
            struct { T x, y, z, w; };
            struct { T r, g, b, a; };
            struct { T s, t, p, q; };
            T data[4];
        };
        constexpr vec4 &operator+=(vec4 v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
        constexpr vec4 &operator-=(vec4 v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
        constexpr vec4 &operator*=(vec4 v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
        constexpr vec4 &operator/=(vec4 v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
        constexpr vec4 &operator+=(T s) { x += s; y += s; z += s; w += s; return *this; }
        constexpr vec4 &operator-=(T s) { x -= s; y -= s; z -= s; w -= s; return *this; }
        constexpr vec4 &operator*=(T s) { x *= s; y *= s; z *= s; w *= s; return *this; }
        constexpr vec4 &operator/=(T s) { x /= s; y /= s; z /= s; w /= s; return *this; }
        constexpr vec4 &operator++() { ++x; ++y; ++z; ++w; return *this; }
        constexpr vec4 &operator--() { --x; --y; --z; --w; return *this; }
        [[nodiscard]] constexpr vec4 operator+(vec4 v) const { return {x + v.x, y + v.y, z + v.z, w + v.w}; }
        [[nodiscard]] constexpr vec4 operator-(vec4 v) const { return {x - v.x, y - v.y, z - v.z, w - v.w}; }
        [[nodiscard]] constexpr vec4 operator*(vec4 v) const { return {x * v.x, y * v.y, z * v.z, w * v.w}; }
        [[nodiscard]] constexpr vec4 operator/(vec4 v) const { return {x / v.x, y / v.y, z / v.z, w / v.w}; }
        [[nodiscard]] constexpr vec4 operator+(T s) const { return {x + s, y + s, z + s, w + s}; }
        [[nodiscard]] constexpr vec4 operator-(T s) const { return {x - s, y - s, z - s, w - s}; }
        [[nodiscard]] constexpr vec4 operator*(T s) const { return {x * s, y * s, z * s, w * s}; }
        [[nodiscard]] constexpr vec4 operator/(T s) const { return {x / s, y / s, z / s, w / s}; }
        [[nodiscard]] constexpr bool operator==(vec4 v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
        [[nodiscard]] constexpr bool operator!=(vec4 v) const { return x != v.x || y != v.y || z != v.z || w != v.w; }
        [[nodiscard]] constexpr vec4 operator-() const { return {-x, -y, -z, -w}; }
        [[nodiscard]] constexpr T dot(vec4 v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
        [[nodiscard]] constexpr T dist(vec4 v) const {
            return std::sqrt(
                (x - v.x) * (x - v.x) +
                (y - v.y) * (y - v.y) +
                (z - v.z) * (z - v.z) +
                (w - v.w) * (w - v.w)
                );
        }
        [[nodiscard]] constexpr T norm() const { return std::sqrt(x * x + y * y + z * z + w * w); }
        [[nodiscard]] constexpr vec4 unit() const {
            T length = norm();
            if (length == 0) return {0, 0, 0, 0};
            return {x / length, y / length, z / length, w / length};
        }
        [[nodiscard]] constexpr vec3<T> div_w() const {
            if (w == 0) return {0, 0, 0};
            return {x / w, y / w, z / w};
        }
    };
    using vec4d = vec4<double>;
    using vec4f = vec4<float>;
    using vec4i = vec4<int>;
    using vec4u = vec4<unsigned int>;
    template <typename T>
    struct mat3 {
        union {
            vec3<T> v[3];
            T m[3][3];
            T data[9];
        };
        static constexpr mat3 id() {
            return {{
                vec3<T>{1, 0, 0},
                vec3<T>{0, 1, 0},
                vec3<T>{0, 0, 1}
            }};
        }
        [[nodiscard]] constexpr vec3<T> operator*(vec3<T> v) const {
            return {
                this->v[0].x * v.x + this->v[1].x * v.y + this->v[2].x * v.z,
                this->v[0].y * v.x + this->v[1].y * v.y + this->v[2].y * v.z,
                this->v[0].z * v.x + this->v[1].z * v.y + this->v[2].z * v.z
            };
        }
        [[nodiscard]] constexpr mat3 operator*(mat3 const &m) const {
            return {
                *this * m.v[0],
                *this * m.v[1],
                *this * m.v[2]
            };
        }
        [[nodiscard]] constexpr mat3 operator+(mat3 const &m) const {
            return {{this->v[0] + m.v[0], this->v[1] + m.v[1], this->v[2] + m.v[2]}};
        }
        [[nodiscard]] constexpr mat3 operator-(mat3 const &m) const {
            return {{this->v[0] - m.v[0], this->v[1] - m.v[1], this->v[2] - m.v[2]}};
        }
    };
    using mat3d = mat3<double>;
    using mat3f = mat3<float>;
    using mat3i = mat3<int>;
    using mat3u = mat3<unsigned int>;
    template <typename T>
    struct mat4 {
        union {
            vec4<T> v[4];
            T m[4][4];
            T data[16];
        };
        static constexpr mat4 id() {
            return {{
                vec4<T>{1, 0, 0, 0},
                vec4<T>{0, 1, 0, 0},
                vec4<T>{0, 0, 1, 0},
                vec4<T>{0, 0, 0, 1}
            }};
        }
        [[nodiscard]] constexpr vec4<T> operator*(vec4<T> v) const {
            return {
                this->v[0].x * v.x + this->v[1].x * v.y + this->v[2].x * v.z + this->v[3].x * v.w,
                this->v[0].y * v.x + this->v[1].y * v.y + this->v[2].y * v.z + this->v[3].y * v.w,
                this->v[0].z * v.x + this->v[1].z * v.y + this->v[2].z * v.z + this->v[3].z * v.w,
                this->v[0].w * v.x + this->v[1].w * v.y + this->v[2].w * v.z + this->v[3].w * v.w
            };
        }
        [[nodiscard]] constexpr mat4 operator*(mat4 const &m) const {
            return {
                *this * m.v[0],
                *this * m.v[1],
                *this * m.v[2],
                *this * m.v[3]
            };
        }
        [[nodiscard]] constexpr mat4 operator+(mat4 const &m) const {
            return {{this->v[0] + m.v[0], this->v[1] + m.v[1], this->v[2] + m.v[2], this->v[3] + m.v[3]}};
        }
        [[nodiscard]] constexpr mat4 operator-(mat4 const &m) const {
            return {{this->v[0] - m.v[0], this->v[1] - m.v[1], this->v[2] - m.v[2], this->v[3] - m.v[3]}};
        }
    };
    using mat4d = mat4<double>;
    using mat4f = mat4<float>;
    using mat4i = mat4<int>;
    using mat4u = mat4<unsigned int>;
}

#endif //SOUL_MATH_HPP
