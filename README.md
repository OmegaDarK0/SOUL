# SOUL Engine (v0.1.0-alpha)

*Read in other languages: [Français](README.md)*

## 1. Introduction and Engine Philosophy

The SOUL engine is a high-performance software solution developed in C++20 following a "C with classes" approach. It was designed to meet the strictest requirements for real-time computing and hardware resource management.

The architecture is entirely based on **Data-Oriented Design (DOD)** to maximize data locality and processor cache efficiency. We aim for a "Zero-Allocation" target during the main execution loop, guaranteeing minimal latency and system stability. The engine systematically favors algorithm clarity and contiguous memory organization to avoid *cache misses*.

## 2. Technical Architecture

SOUL maintains a symbiotic relationship with the VOID framework (C17): SOUL manages the high-level logic while VOID provides hardware abstraction (Back-end) via *Opaque Pointers*.

* **Pure ECS:** Entity management by binary signatures with contiguous storage within `component_array` structures for fast iteration.
* **Job System:** Strictly *lock-free* multi-threaded architecture exploiting the atomic primitives of the VOID framework, such as `void_atomic_compare_exchange`, to saturate detected CPU cores.
* **Memory Management:** Exclusive use of arenas via VOID. The global arena manages persistence while the frame arena (10% of total memory) is reset at each cycle at no cost via `void_frame_free`.

## 3. Component System and ECS

The engine exposes a suite of components optimized for "Structure of Arrays" (SoA) storage. The central registry drives entity creation and component assignment within the limit of `MAX_COMPONENTS` (32), ensuring a predictable memory footprint.

| Component | Description |
| :--- | :--- |
| **Transform** | Defines position, rotation, and scale in three dimensions. |
| **Velocity** | Captures direction vectors and scalar speed. |
| **Body** | Critical for physics: stores inverse mass (`imass`) and restitution coefficient (`bounciness`). A value of `imass = 0` defines an object of infinite mass (static). |
| **Size** | Defines the AABB (Axis-Aligned Bounding Box) dimensions. |
| **Sprite** | Contains texture references via the Asset Manager. |
| **Viewport** | Manages projection and tracking parameters. |

## 4. Systems and Physics

### Physics
The physics system integrates motion vectors to update transformations based on the `delta_time` (`dt`) calculated by the VOID back-end.

### Collision
AABB detection uses the `spatial_grid` to limit tests to immediate neighborhoods. Collision resolution applies impulses based on the `imass` of the involved entities.

### Scene (Spatial Grid)
The `spatial_grid` dynamically partitions space. Entities are sorted by cell index via `std::sort` in `scene.cpp` to guarantee ultra-high-performance neighborhood searches.

### Camera
The camera system manages dynamic target tracking and world coordinates conversion to the viewport's screen space.

### Render
The rendering engine implements a **Z-sorting** system. All rendering commands are sorted by depth (`std::sort` on the Z-axis) before being sent to the back-end to ensure the display order.

## 5. Dependencies and Prerequisites

* The engine dynamically depends on the `libVOID` library (v0.1.0-alpha).
* Third-party dependencies include **SDL2** and **SDL2_image** for graphics context and resource management.
* A compiler supporting the **C++20** standard (`g++` or `clang`) is required to use modern language features.
* The system is cross-platform compatible, natively supporting Windows (via MinGW/MSYS2) and Linux (via GCC/Clang).

## 6. Compilation and Usage

Compilation is automated via a strict `Makefile`.

To build the main binary:
```bash
make
```

To generate the shared library:
```bash
make lib
```

**Launch Arguments:**
* The SOUL binary accepts several arguments for front-end testing: `sandbox`, `kinetik`, or `survival`.
* The VOID test binary accepts low-level parameters to test subsystems: `memory`, `thread`, or `render`.

> **Memory Note:** To guarantee the absence of fragmentation, all allocations must strictly go through `void_arena_alloc` or the engine's `arena_make` functions.

## 7. Project Structure

The file tree clearly distinguishes interfaces from implementations:
* `include/` : Contains the interfaces (`.hpp`).
* `src/` : Contains the implementations (`.cpp`).
* `void.h` : Serves as the absolute interface bridge between the C kernel and the C++ logic.
* `shell.cpp` : Acts as the application entry point, orchestrating arena initialization and game loop launch.
* `ecs.cpp` and `job.cpp` : Encapsulate pure performance layers, isolated from business logic.

## 8. License

This project is distributed under the **GPL 3** license. The source code and integration with the VOID framework are protected by copyright. Any redistribution must maintain the original attribution notices and respect the terms of the GNU General Public License.