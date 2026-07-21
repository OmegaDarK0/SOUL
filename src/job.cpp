#include <array>
#include "job.hpp"
#include "void.h"

namespace soul {
    struct job_decl {
        job_function func = nullptr;
        void* data = nullptr;
    };
    constexpr uint32 MAX_JOBS = 1024;
    static std::array<job_decl, MAX_JOBS> s_job_queue;
    static volatile uint32 s_queue_head = 0;
    static volatile uint32 s_queue_tail = 0;
    static volatile uint32 s_jobs_in_flight = 0;
    static volatile uint32 s_is_running = 1;
    static void worker_thread(void* user_data) {
        (void)user_data;
        while (s_is_running) {
            const uint32 current_tail = s_queue_tail;
            const uint32 current_head = s_queue_head;
            if (current_tail < current_head) {
                if (void_atomic_compare_exchange(&s_queue_tail, current_tail, current_tail + 1)) {
                    const uint32 index = current_tail % MAX_JOBS;
                    if (const auto [func, data] = s_job_queue[index]; func) {
                        func(data);
                    }
                    void_atomic_decrement(&s_jobs_in_flight);
                }
            } else {
                void_thread_sleep(1);
            }
        }
    }
    void job::init() {
        uint32 cores = void_system_get_core_count();
        if (cores < 2) cores = 2;
        const uint32 worker_count = cores - 1;
        for (uint32 i = 0; i < worker_count; ++i) {
            void_thread_create(worker_thread, nullptr);
        }
    }
    void job::dispatch(const job_function func, void* data) {
        void_atomic_increment(&s_jobs_in_flight);
        const uint32 current_head = s_queue_head;
        const uint32 index = current_head % MAX_JOBS;
        s_job_queue[index].func = func;
        s_job_queue[index].data = data;
        void_atomic_increment(&s_queue_head);
    }
    void job::wait() {
        while (s_jobs_in_flight > 0) {
            void_thread_sleep(1);
        }
    }
    void job::exit() {
        s_is_running = 0;
        void_thread_sleep(50);
    }
}
