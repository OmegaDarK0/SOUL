#include <ctime>
#include <cstring>
#include <cstdlib>
#include "soul.hpp"

using namespace soul;

// ============================================================================
// SHARED UTILITIES
// ============================================================================

void setup_world_backgrounds(registry& world) {
    const entity screen_bg = world.create_entity();
    world.add_component(screen_bg, transform{ .position = {{{0.0f, 0.0f, -10.0f}}} });
    world.add_component(screen_bg, size{ .dimension = {{{100.0f, 100.0f}}} });
    world.add_component(screen_bg, sprite{ .color = {{{0.117f, 0.117f, 0.117f, 1.0f}}} });
    const entity world_bg = world.create_entity();
    world.add_component(world_bg, transform{ .position = {{{0.0f, 0.0f, -5.0f}}} });
    world.add_component(world_bg, size{ .dimension = {{{12.5f, 9.375f}}} });
    world.add_component(world_bg, sprite{ .color = {{{0.392f, 0.392f, 0.392f, 1.0f}}} });
}

void spawn_wall(registry& world, const float x, const float y, const float w, const float h) {
    const entity wall = world.create_entity();
    world.add_component(wall, size{ .dimension = {{{w, h}}} });
    world.add_component(wall, transform{ .position = {{{x, y}}} });
    world.add_component(wall, sprite{ .color = {{{0.2f, 0.2f, 0.2f, 1.0f}}} });
    world.add_component(wall, body{ .imass = 0.0f, .bounciness = 1.0f }); // Masse infinie = Statique
}

// ============================================================================
// GAME 1: SANDBOX
// ============================================================================
void game_sandbox_init(registry& world) {
    VOID_LOG_INFO("[GAME] Loading 'Sandbox'...");
    const entity player = world.create_entity();
    world.add_component(player, controller::player{});
    world.add_component(player, sprite{ .color = {{{0.8f, 0.5f, 0.1f, 1.0f}}} });
    world.add_component(player, transform{});
    world.add_component(player, velocity{ .speed = 8.0f });
    world.add_component(player, size{ .dimension = {{{1.0f, 1.0f}}} });
    world.add_component(player, body{ .imass = 1.0f, .bounciness = 0.0f });
    const entity wall1 = world.create_entity();
    world.add_component(wall1, size{ .dimension = {{{3.0f, 1.0f}}} });
    world.add_component(wall1, transform{ .position = {{{3.0f, 2.0f, 0.0f}}} });
    world.add_component(wall1, velocity{ .direction = {{{0.0f, 1.0f, 0.0f}}}, .speed = 4.0f });
    world.add_component(wall1, sprite{ .color = {{{0.2f, 0.5f, 0.9f, 1.0f}}} });
    world.add_component(wall1, body{ .imass = 0.0f, .bounciness = 1.0f });
    const entity wall2 = world.create_entity();
    world.add_component(wall2, size{ .dimension = {{{1.0f, 3.0f}}} });
    world.add_component(wall2, transform{ .position = {{{2.0f, 3.0f, 0.0f}}} });
    world.add_component(wall2, velocity{ .direction = {{{1.0f, 0.0f, 0.0f}}}, .speed = 4.0f });
    world.add_component(wall2, sprite{ .color = {{{0.2f, 0.5f, 0.9f, 1.0f}}} });
    world.add_component(wall2, body{ .imass = 0.0f, .bounciness = 1.0f });
}

void game_sandbox_update(registry& world, const float dt) {
    (void)dt;
    for (const entity e : world.get_view<sprite, transform, body>()) {
        if (world.has_component<controller::player>(e)) continue;
        auto& spr = world.get_component<sprite>(e);
        const auto& trans = world.get_component<transform>(e);
        spr.color.r = (std::sin(trans.position.x) + 1.0f) * 0.5f;
        spr.color.b = (std::cos(trans.position.y) + 1.0f) * 0.5f;
    }
}

// ============================================================================
// GAME 2: KINETIK (Physics & ECS Stress Test)
// ============================================================================
void game_kinetik_init(registry& world) {
    VOID_LOG_INFO("[GAME] Loading 'Kinetik' (Stress Test)...");
    srand(static_cast<unsigned int>(time(nullptr)));
    spawn_wall(world, 0.0f, -4.5f, 12.0f, 1.0f); // Top
    spawn_wall(world, 0.0f, 4.5f, 12.0f, 1.0f);  // Bottom
    spawn_wall(world, -6.0f, 0.0f, 1.0f, 9.0f);  // Left
    spawn_wall(world, 6.0f, 0.0f, 1.0f, 9.0f);   // Right
    for (int i = 0; i < 150; ++i) {
        const entity box = world.create_entity();
        const float rx = rand() % 100 / 100.0f * 10.0f - 5.0f;
        const float ry = rand() % 100 / 100.0f * 6.0f - 3.0f;
        const float vx = rand() % 100 / 100.0f * 2.0f - 1.0f;
        const float vy = rand() % 100 / 100.0f * 2.0f - 1.0f;
        const float r = rand() % 100 / 100.0f;
        const float g = rand() % 100 / 100.0f;
        const float b = rand() % 100 / 100.0f;
        world.add_component(box, transform{ .position = {{{rx, ry, 0.0f}}} });
        world.add_component(box, size{ .dimension = {{{0.3f, 0.3f}}} });
        world.add_component(box, velocity{ .direction = {{{vx, vy, 0.0f}}}, .speed = 6.0f });
        world.add_component(box, body{ .imass = 1.0f, .bounciness = 1.0f });
        world.add_component(box, sprite{ .color = {{{r, g, b, 1.0f}}} });
    }
}

void game_kinetik_update(registry& world, const float dt) {
    for (const entity box : world.get_view<transform, velocity>()) {
        const auto& trans = world.get_component<transform>(box);
        auto&[direction, speed] = world.get_component<velocity>(box);
        float dir_x = -trans.position.x;
        float dir_y = -trans.position.y;
        if (const float length = std::sqrt(dir_x * dir_x + dir_y * dir_y); length > 0.1f) {
            dir_x /= length;
            dir_y /= length;
            direction.x += dir_x * dt * 0.5f;
            direction.y += dir_y * dt * 0.5f;
            if (const float v_len = std::sqrt(direction.x * direction.x + direction.y * direction.y); v_len > 0.0f) {
                direction.x /= v_len;
                direction.y /= v_len;
            }
        }
    }
}

// ============================================================================
// GAME 3: SURVIVAL (Dodge the cubes)
// ============================================================================
void game_survival_init(registry& world) {
    VOID_LOG_INFO("[GAME] Loading 'Survival'...");
    const entity player = world.create_entity();
    world.add_component(player, controller::player{});
    world.add_component(player, sprite{ .color = {{{0.2f, 0.8f, 0.4f, 1.0f}}} }); // Vert
    world.add_component(player, transform{ .position = {{{0.0f, 0.0f, 0.0f}}} });
    world.add_component(player, velocity{ .speed = 10.0f });
    world.add_component(player, size{ .dimension = {{{0.8f, 0.8f}}} });
    world.add_component(player, body{ .imass = 1.0f, .bounciness = 0.2f });
    for (int i = 0; i < 4; i++) {
        const entity enemy = world.create_entity();
        const float start_x = i % 2 == 0 ? -4.0f : 4.0f;
        const float start_y = i < 2 ? -3.0f : 3.0f;
        world.add_component(enemy, size{ .dimension = {{{1.5f, 1.5f}}} });
        world.add_component(enemy, transform{ .position = {{{start_x, start_y, 0.0f}}} });
        world.add_component(enemy, sprite{ .color = {{{0.9f, 0.1f, 0.2f, 1.0f}}} });
        world.add_component(enemy, velocity{
            .direction = {{{start_x * -1.0f, start_y * -1.0f, 0.0f}}},
            .speed = 12.0f
        });
        world.add_component(enemy, body{ .imass = 0.1f, .bounciness = 1.0f });
    }
}

void game_survival_update(registry& world, const float dt) {
    vec3f player_pos = {{{0.0f, 0.0f, 0.0f}}};
    for (const entity p : world.get_view<controller::player, transform>()) {
        player_pos = world.get_component<transform>(p).position;
        break;
    }
    for (const entity enemy : world.get_view<sprite, velocity, body, transform>()) {
        if (world.has_component<controller::player>(enemy)) continue;
        auto&[direction, speed] = world.get_component<velocity>(enemy);
        const auto& trans = world.get_component<transform>(enemy);
        speed += 1.0f * dt;
        if (speed > 20.0f) speed = 20.0f;
        float dir_x = player_pos.x - trans.position.x;
        float dir_y = player_pos.y - trans.position.y;
        if (const float length = std::sqrt(dir_x * dir_x + dir_y * dir_y); length > 0.0f) {
            dir_x /= length;
            dir_y /= length;
            direction.x += dir_x * dt * 1.5f;
            direction.y += dir_y * dt * 1.5f;
            if (const float v_len = std::sqrt(direction.x * direction.x + direction.y * direction.y); v_len > 0.0f) {
                direction.x /= v_len;
                direction.y /= v_len;
            }
        }
    }
}

// ============================================================================
// ENTRY POINT
// ============================================================================
int main(const int argc, char* argv[]) {
    auto engine = init(argc, argv, {
        .memory_size = 256 * KB,
        .window_width = 1920,
        .window_height = 1080,
        .max_entities = 1024,
        .max_render_commands = 1024
    });
    registry& world = engine::get_world();
    world.register_component<velocity>();
    world.register_component<body>();
    world.register_component<size>();
    world.register_component<sprite>();
    world.register_component<controller::player>();
    setup_world_backgrounds(world);
    bool game_launched = false;
    if (argc > 1) {
        if (strcmp(argv[1], "sandbox") == 0) {
            game_sandbox_init(world);
            engine.set_game_update(game_sandbox_update);
            game_launched = true;
        } else if (strcmp(argv[1], "kinetik") == 0) {
            game_kinetik_init(world);
            engine.set_game_update(game_kinetik_update);
            game_launched = true;
        } else if (strcmp(argv[1], "survival") == 0) {
            game_survival_init(world);
            engine.set_game_update(game_survival_update);
            game_launched = true;
        } else {
            VOID_LOG_WARN("Unknown game: %s", argv[1]);
        }
    }
    if (!game_launched) {
        VOID_LOG_INFO("Usage: %s [sandbox] [kinetik] [survival]", argv[0]);
        VOID_LOG_INFO("No valid argument provided. Launching 'Sandbox' by default.");
        game_sandbox_init(world);
        engine.set_game_update(game_sandbox_update);
    }
#ifndef NDEBUG
    void_memory_print();
#endif
    engine.run();
    return 0;
}