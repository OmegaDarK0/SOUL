#include <cstdlib>
#include <cstring>
#include "soul.hpp"
#include "input.hpp"
#include "physics.hpp"
#include "scene.hpp"
#include "collision.hpp"
#include "camera.hpp"
#include "render.hpp"
#include "job.hpp"

namespace soul {
    static VoidWindow* s_window = nullptr;
    static registry s_world;
    static asset_manager s_assets;
    engine::engine(const int argc, char* argv[], engine_config config) {
        for (int i = 1; i < argc; ++i) {
            if (strcmp(argv[i], "--width") == 0 && i + 1 < argc) {
                config.window_width = atoi(argv[++i]);
            } else if (strcmp(argv[i], "--height") == 0 && i + 1 < argc) {
                config.window_height = atoi(argv[++i]);
            } else if (strcmp(argv[i], "--memory") == 0 && i + 1 < argc) {
                config.memory_size = atoi(argv[++i]) * MB;
            }
        }
        if (!void_init()) abort();
        if (!void_memory_init(config.memory_size)) abort();
        if (config.grid_width <= 0) config.grid_width = static_cast<unsigned int>(config.world_width / config.cell_size) + 2;
        if (config.grid_height <= 0) config.grid_height = static_cast<unsigned int>(config.world_height / config.cell_size) + 2;
        s_world.init(config.max_entities);
        s_assets.init(config.max_assets);
        scene::init(config.max_entities, config.grid_width, config.grid_height, config.cell_size);
        collision::init(config.max_entities);
        render::init(config.max_render_commands);
        job::init(config.max_jobs);
        s_window = void_window_create(config.title, config.window_width, config.window_height);
        VOID_ASSERT(s_window != nullptr);
        void_window_set_logical_size(s_window, static_cast<int>(config.logic_width), static_cast<int>(config.logic_height));
        s_world.register_component<viewport>();
        s_world.register_component<transform>();
        s_world.set_context<config::window>({
            .logic_width = config.logic_width,
            .logic_height = config.logic_height
        });

        s_world.set_context<config::physics>({
            .world_width = config.world_width,
            .world_height = config.world_height
        });
        const entity camera = s_world.create_entity();
        s_world.add_component(camera, viewport{
            .ppu = config.ppu,
        });
        s_world.add_component(camera, transform{});
        VOID_LOG_OK("SOUL initialized! (window: %dx%d, mem: %llu KB)",
                    config.window_width, config.window_height, config.memory_size / KB);
    }
    engine::~engine() {
        job::exit();
        VOID_LOG_INFO("SOUL exited.");
        void_window_destroy(s_window);
        void_exit();
    }
    registry& engine::get_world() {
        return s_world;
    }
    asset engine::load_texture(const char* name) {
        return s_assets.load_texture(s_window, name);
    }
    void engine::run() const {
        while (void_window_is_running(s_window) && !void_window_should_close(s_window)) {
            const float dt = void_time_get_delta();
            void_window_poll_events(s_window);
            if (void_window_should_close(s_window) || void_input_is_key_pressed(VOID_KEY_ESCAPE)) {
                void_window_close(s_window);
            }
            input::update(s_world);
            if (m_game_update != nullptr) {
                m_game_update(s_world, dt);
            }
            physics::update(s_world, dt);
            scene::update(s_world);
            collision::update(s_world);
            camera::update(s_world);
            render::update(s_world, s_window, s_assets);
            void_log_flush();
            void_frame_free();
        }
    }
}