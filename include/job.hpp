#ifndef SOUL_JOB_HPP
#define SOUL_JOB_HPP

#include "void.h"

namespace soul {
    using job_function = void(*)(void* data);
    struct job {
        static void init(uint32 max_jobs);
        static void dispatch(job_function func, void* data = nullptr);
        static void wait();
        static void exit();
    };
}

#endif //SOUL_JOB_HPP
