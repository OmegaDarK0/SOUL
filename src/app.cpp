#include "app.hpp"
#include "input.hpp"
#include "physics.hpp"
#include "scene.hpp"
#include "collision.hpp"
#include "camera.hpp"
#include "render.hpp"
#include "job.hpp"

namespace soul {
    void application::run() {
        job::init();
        while (void_window_is_running(m_window) && !void_window_should_close(m_window)) {
            const float dt = void_time_get_delta();
            void_window_poll_events(m_window);
            if (void_window_should_close(m_window) || void_input_is_key_pressed(VOID_KEY_ESCAPE)) {
                void_window_close(m_window);
            }
            input::update(m_world);
            physics::update(m_world, dt);
            scene::update(m_world);
            collision::update(m_world);
            camera::update(m_world);
            render::update(m_world, m_window, m_assets);
            void_log_flush();
            void_frame_free();
        }
        job::exit();
    }
}
