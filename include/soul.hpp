#ifndef SOUL_HPP
#define SOUL_HPP

#include "ecs.hpp"
#include "components.hpp"

namespace soul {

    // ============================================================================
    // ENGINE CONFIGURATION
    // ============================================================================
    // The Front-end configuration structure. This is passed by the user
    // to initialize the engine and is later split into Back-end contexts.
    struct engine_config {
        const char* title = "SOUL ENGINE";  // Window title bar text
        uint64 memory_size = 32768;         // Total memory allocated by the Global Arena
        uint32 window_width = 1280;         // Physical window width in pixels
        uint32 window_height = 720;         // Physical window height in pixels
        uint32 logic_width = 800;           // Logical resolution width for rendering
        uint32 logic_height = 600;          // Logical resolution height for rendering

        // Physics & World boundaries
        float world_width = 12.5f;          // Absolute world width in physical units
        float world_height = 9.375f;        // Absolute world height in physical units
        float ppu = 64.0f;                  // Pixels Per Unit (Render scale)

        // ECS & Memory limits
        uint32 max_entities = 64;           // Maximum number of active entities allowed
        uint32 max_assets = 64;             // Maximum number of loaded assets (textures, etc.)
        uint32 max_render_commands = 256;   // Maximum sprites drawn per frame
        uint32 max_jobs = 64;               // Maximum parallel jobs for the thread pool

        // Spatial Grid (Broad-phase collision)
        uint32 grid_width = 0;              // Auto-calculated if set to 0
        uint32 grid_height = 0;             // Auto-calculated if set to 0
        float cell_size = 2.0f;             // Optimal cell size for spatial partitioning
    };

    // Function pointer type for the user's custom gameplay loop
    typedef void (*game_update_fn)(registry& world, float dt);

    // ============================================================================
    // CORE ENGINE CLASS
    // ============================================================================
    // The main application wrapper. It manages the lifecycle of the window,
    // the global memory arena, and the core systems (Physics, Render, ECS).
    class SOUL_API engine {
        game_update_fn m_game_update = nullptr;

    public:
        // Initializes the engine subsystems (Window, Arena, ECS, Systems)
        engine(int argc, char* argv[], engine_config config);

        // Cleans up all resources and flushes memory upon exit
        ~engine();

        // Disable copy/assignment to prevent accidental engine duplication
        engine(const engine&) = delete;
        engine& operator=(const engine&) = delete;

        // Global accessors
        static registry& get_world();
        static asset load_texture(const char* name);

        // Registers the user's custom gameplay logic function
        void set_game_update(const game_update_fn func) { m_game_update = func; }

        // Starts the main engine loop (Blocking call)
        void run() const;
    };

    // ============================================================================
    // ENTRY POINT HELPER
    // ============================================================================
    // Inline helper to easily instantiate the engine from the main() function
    inline engine init(const int argc, char* argv[], const engine_config& config = {}) {
        return {argc, argv, config};
    }
}

#endif //SOUL_HPP