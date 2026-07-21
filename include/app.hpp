#ifndef SOUL_APP_HPP
#define SOUL_APP_HPP

#include "ecs.hpp"

namespace soul {
    class application {
        VoidWindow* m_window;
        registry m_world;
    public:
        application(const char* title, const uint32 win_w, const uint32 win_h, const uint32 logic_w = 800, const uint32 logic_h = 600){
            m_window = void_window_create(title, win_w, win_h);
            VOID_ASSERT(m_window != nullptr);
            void_window_set_logical_size(m_window, static_cast<int>(logic_w), static_cast<int>(logic_h));
        }
        ~application() {
            void_window_destroy(m_window);
        }
        registry& get_world() { return m_world; }
        void run();
    };
}

#endif //SOUL_APP_HPP
