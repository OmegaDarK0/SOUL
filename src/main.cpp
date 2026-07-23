#include "soul.hpp"

using namespace soul;

void test_ecs() {
    VOID_LOG_INFO("ECS tests running...");
    registry world;
    world.register_component<transform>();
    world.register_component<velocity>();
    const entity player = world.create_entity();
    world.add_component(player, transform{});
    world.add_component(player, velocity{{{{1.0f}}}});
    const entity enemy = world.create_entity();
    world.add_component(enemy, transform{{{{10.0f}}}});
    auto& t = world.get_component<transform>(player);
    auto&[direction, speed] = world.get_component<velocity>(player);
    t.position += direction;
    //VOID_ASSERT(p_trans.position.x == 1.0f);
    world.destroy_entity(enemy);
    VOID_LOG_OK("ECS tests successful!");
}

void test_app() {
    application app("SOUL ENGINE", 1920, 1200);
    registry& world = app.get_world();
    world.register_component<engine_config>();
    world.register_component<world_bounds>();
    world.register_component<player_controller>();
    world.register_component<viewport>();
    world.register_component<sprite>();
    world.register_component<transform>();
    world.register_component<velocity>();
    world.register_component<size>();
    world.register_component<body>();
    const entity engine = world.create_entity();
    world.add_component(engine, engine_config{});
    world.add_component(engine, world_bounds{});
    const entity player = world.create_entity();
    world.add_component(player, player_controller{});
    world.add_component(player, sprite{
        .color = {{{0.8f, 0.5f, 0.1f}}}
    });
    world.add_component(player, transform{});
    world.add_component(player, velocity{
        .speed = 8.0f
    });
    world.add_component(player, size{
        .dimension = {{{1.0f, 1.0f}}}
    });
    world.add_component(player, body{});
    const entity camera = world.create_entity();
    world.add_component(camera, viewport{
        .ppu = 64.0f
    });
    world.add_component(camera, transform{});
    const entity wall1 = world.create_entity();
    world.add_component(wall1, size{
        .dimension = {{{3.0f, 1.0f}}}
    });
    world.add_component(wall1, transform{
        .position = {{{3.0f, 2.0f}}}
    });
    world.add_component(wall1, velocity{
        .direction = {{{0.0f, 1.0f}}},
        .speed = 4.0f
    });
    world.add_component(wall1, sprite{
        .color = {{{0.2f, 0.5f, 0.9f}}}
    });
    world.add_component(wall1, body{
        .imass = 0.0f,
        .bounciness = 1.0f
    });
    const entity wall2 = world.create_entity();
    world.add_component(wall2, size{
        .dimension = {{{1.0f, 3.0f}}}
    });
    world.add_component(wall2, transform{
        .position = {{{2.0f, 3.0f}}}
    });
    world.add_component(wall2, velocity{
        .direction = {{{1.0f, 0.0f}}},
        .speed = 4.0f
    });
    world.add_component(wall2, sprite{
        .color = {{{0.2f, 0.5f, 0.9f}}}
    });
    world.add_component(wall2, body{
        .imass = 0.0f,
        .bounciness = 1.0f
    });
    void_memory_print();
    app.run();
}

int main(const int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    void_init();
    void_memory_init(KB);
    test_app();
    void_exit();
    return 0;
}