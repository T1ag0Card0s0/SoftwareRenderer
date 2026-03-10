#include "thread_pool.h"
#include "renderer/platform.h"
#include <stdlib.h>
#include <string.h>

int thread_pool_init(thread_pool_t *pool, size_t num_threads)
{
  size_t i;
  if (!pool || !num_threads)
  {
    return -1;
  }
  memset(pool, 0, sizeof(*pool));
  if (platform_mutex_init(&pool->mutex))
  {
    return -1;
  }
  if (platform_cond_init(&pool->jobs_available))
  {
    platform_mutex_destroy(&pool->mutex);
    return -1;
  }
  if (platform_cond_init(&pool->all_done))
  {
    platform_cond_destroy(&pool->jobs_available);
    platform_mutex_destroy(&pool->mutex);
    return -1;
  }
  pool->threads = (platform_thread_t *)calloc(num_threads, sizeof(platform_thread_t));
  if (!pool->threads)
  {
    platform_cond_destroy(&pool->all_done);
    platform_cond_destroy(&pool->jobs_available);
    platform_mutex_destroy(&pool->mutex);
    return -1;
  }
  pool->thread_count = num_threads;
  pool->stop = 0;
  pool->initialized = 1;
  for (i = 0; i < num_threads; ++i)
  {
    if (platform_thread_create(&pool->threads[i], thread_pool_worker, pool))
    {
      platform_mutex_lock(&pool->mutex);
      pool->stop = 1;
      platform_cond_broadcast(&pool->jobs_available);
      platform_mutex_unlock(&pool->mutex);
      while (i-- > 0)
      {
        platform_thread_join(pool->threads[i]);
      }
      free(pool->threads);
      platform_cond_destroy(&pool->all_done);
      platform_cond_destroy(&pool->jobs_available);
      platform_mutex_destroy(&pool->mutex);
      memset(pool, 0, sizeof(*pool));
      return -1;
    }
  }
  return 0;
}

int thread_pool_submit(thread_pool_t *pool, thread_pool_job_fn fn, void *arg)
{
  thread_pool_job_t *job;
  if (!pool || !fn || !pool->initialized)
  {
    return -1;
  }
  job = (thread_pool_job_t *)malloc(sizeof(*job));
  if (!job)
  {
    return -1;
  }
  job->fn = fn;
  job->arg = arg;
  job->next = NULL;
  platform_mutex_lock(&pool->mutex);
  if (pool->stop)
  {
    platform_mutex_unlock(&pool->mutex);
    free(job);
    return -1;
  }
  if (pool->job_tail)
  {
    pool->job_tail->next = job;
    pool->job_tail = job;
  }
  else
  {
    pool->job_head = pool->job_tail = job;
  }
  pool->queued_jobs++;
  platform_cond_signal(&pool->jobs_available);
  platform_mutex_unlock(&pool->mutex);
  return 0;
}

void thread_pool_wait(thread_pool_t *pool)
{
  if (!pool || !pool->initialized)
  {
    return;
  }
  platform_mutex_lock(&pool->mutex);
  while (pool->queued_jobs > 0 || pool->active_jobs > 0)
  {
    platform_cond_wait(&pool->all_done, &pool->mutex);
  }
  platform_mutex_unlock(&pool->mutex);
}

void thread_pool_destroy(thread_pool_t *pool)
{
  size_t i;
  thread_pool_job_t *j, *n;
  if (!pool || !pool->initialized)
  {
    return;
  }
  platform_mutex_lock(&pool->mutex);
  pool->stop = 1;
  platform_cond_broadcast(&pool->jobs_available);
  platform_mutex_unlock(&pool->mutex);
  for (i = 0; i < pool->thread_count; ++i)
  {
    platform_thread_join(pool->threads[i]);
  }
  for (j = pool->job_head; j; j = n)
  {
    n = j->next;
    free(j);
  }
  free(pool->threads);
  platform_cond_destroy(&pool->all_done);
  platform_cond_destroy(&pool->jobs_available);
  platform_mutex_destroy(&pool->mutex);
  memset(pool, 0, sizeof(*pool));
}

platform_thread_return_t PLATFORM_THREAD_CALL thread_pool_worker(void *arg)
{
  thread_pool_t *pool = (thread_pool_t *)arg;
  for (;;)
  {
    thread_pool_job_t *job = NULL;
    platform_mutex_lock(&pool->mutex);
    while (!pool->stop && !pool->job_head)
    {
      platform_cond_wait(&pool->jobs_available, &pool->mutex);
    }
    if (pool->stop && !pool->job_head)
    {
      platform_mutex_unlock(&pool->mutex);
      break;
    }
    job = pool->job_head;
    pool->job_head = job->next;
    if (!pool->job_head)
    {
      pool->job_tail = NULL;
    }
    pool->queued_jobs--;
    pool->active_jobs++;
    platform_mutex_unlock(&pool->mutex);
    job->fn(job->arg);
    free(job);
    platform_mutex_lock(&pool->mutex);
    pool->active_jobs--;
    if (!pool->queued_jobs && !pool->active_jobs)
    {
      platform_cond_broadcast(&pool->all_done);
    }
    platform_mutex_unlock(&pool->mutex);
  }
#if defined(_WIN32)
  return 0;
#else
  return NULL;
#endif
}
