#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "platform_types.h"

#include <stddef.h>

typedef void (*thread_pool_job_fn)(void *arg);

typedef struct thread_pool_job
{
    thread_pool_job_fn fn;
    void *arg;
    struct thread_pool_job *next;
} thread_pool_job_t;

typedef struct 
{
    platform_thread_t *threads;
    size_t thread_count;
    thread_pool_job_t *job_head;
    thread_pool_job_t *job_tail;
    size_t queued_jobs;
    size_t active_jobs;
    int stop;
    int initialized;
    platform_mutex_t mutex;
    platform_cond_t jobs_available;
    platform_cond_t all_done;
} thread_pool_t;

int thread_pool_init(thread_pool_t *pool, size_t num_threads);

int thread_pool_submit(thread_pool_t *pool, thread_pool_job_fn fn, void *arg);

void thread_pool_wait(thread_pool_t *pool);

void thread_pool_destroy(thread_pool_t *pool);

platform_thread_return_t PLATFORM_THREAD_CALL thread_pool_worker(void *arg);

#endif // THREAD_POOL_H
