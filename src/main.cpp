#include "soul.hpp"

using namespace soul;

void test_math() {
    VOID_LOG_INFO("Démarrage des tests mathématiques...");
    vec3f v1 = {1.0f, 0.0f, 0.0f};
    vec3f v2 = {0.0f, 1.0f, 0.0f};
    vec3f expected_cross = {0.0f, 0.0f, 1.0f};
    SOUL_ASSERT(v1.cross(v2) == expected_cross);
    mat4f m = mat4f::id();
    vec4f pos = {5.0f, -3.2f, 42.0f, 1.0f};
    SOUL_ASSERT((m * pos) == pos);
    mat4f trans = mat4f::id();
    trans.v[3] = {10.0f, 20.0f, 30.0f, 1.0f};
    vec4f origine = {0.0f, 0.0f, 0.0f, 1.0f};
    vec4f expected_pos = {10.0f, 20.0f, 30.0f, 1.0f};
    SOUL_ASSERT((trans * origine) == expected_pos);
    VOID_LOG_OK("Succès de tous les tests mathématiques !");
}

int main(const int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    void_log_init();
    test_math();
    void_log_exit();
    return 0;
}