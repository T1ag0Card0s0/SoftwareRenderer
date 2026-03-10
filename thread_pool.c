/*
    portable_thread_pool_heavy.c  –  Mixed CPU + I/O stress demo

    Build:
      Linux/macOS/BSD:
        cc -O2 -Wall -Wextra -pthread -lm portable_thread_pool_heavy.c -o thread_pool_heavy

      Windows (MSVC):
        cl /O2 /W4 portable_thread_pool_heavy.c

    Workload design
    ---------------
    Each job has three phases, whose durations are drawn from realistic
    distributions so that no two jobs take exactly the same time:

      1. CPU phase  – spin on a SHA-256-like mixing loop (no math.h needed)
                      Base: CPU_BASE_ITERS iterations, ±50% uniform jitter.

      2. I/O phase  – simulate a disk/network round-trip with a sleep.
                      Base: IO_BASE_MS milliseconds, log-normally distributed
                      (geometric mean = IO_BASE_MS, σ ≈ 0.6 decades).

      3. CPU phase  – a second, shorter compute burst (result verification).
                      Base: CPU_BASE_ITERS / 4 iterations, ±25% uniform jitter.

    The combination gives a realistic "fetch data → crunch → validate" shape.

    Summary statistics printed at the end:
      • Per-job start, end, duration
      • Min / max / mean / std-dev of job durations
      • Observed wall-clock speedup vs serial estimate
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>   /* sqrt, log, exp */

#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <pthread.h>
    #include <unistd.h>
    #include <time.h>
    #include <sys/time.h>
#endif

/* =========================================================
   Platform abstraction
   ========================================================= */

#if defined(_WIN32)

typedef HANDLE            tp_thread_t;
typedef CRITICAL_SECTION  tp_mutex_t;
typedef CONDITION_VARIABLE tp_cond_t;

static int  tp_mutex_init   (tp_mutex_t *m) { InitializeCriticalSection(m); return 0; }
static void tp_mutex_destroy(tp_mutex_t *m) { DeleteCriticalSection(m); }
static void tp_mutex_lock   (tp_mutex_t *m) { EnterCriticalSection(m); }
static void tp_mutex_unlock (tp_mutex_t *m) { LeaveCriticalSection(m); }

static int  tp_cond_init     (tp_cond_t *c) { InitializeConditionVariable(c); return 0; }
static void tp_cond_destroy  (tp_cond_t *c) { (void)c; }
static void tp_cond_wait     (tp_cond_t *c, tp_mutex_t *m) { SleepConditionVariableCS(c, m, INFINITE); }
static void tp_cond_signal   (tp_cond_t *c) { WakeConditionVariable(c); }
static void tp_cond_broadcast(tp_cond_t *c) { WakeAllConditionVariable(c); }

typedef DWORD WINAPI tp_thread_ret_t;
#define TP_THREAD_CALL WINAPI

static int  tp_thread_create(tp_thread_t *t,
                              tp_thread_ret_t (TP_THREAD_CALL *fn)(void *),
                              void *arg)
{
    DWORD tid;
    *t = CreateThread(NULL, 0, fn, arg, 0, &tid);
    return (*t == NULL) ? -1 : 0;
}
static void tp_thread_join(tp_thread_t t) { WaitForSingleObject(t, INFINITE); CloseHandle(t); }

typedef LARGE_INTEGER tp_time_t;
static tp_time_t tp_time_now(void) { LARGE_INTEGER t; QueryPerformanceCounter(&t); return t; }
static double tp_time_diff_ms(tp_time_t s, tp_time_t e) {
    LARGE_INTEGER f; QueryPerformanceFrequency(&f);
    return (double)(e.QuadPart - s.QuadPart) * 1000.0 / (double)f.QuadPart;
}
static void tp_sleep_ms(int ms) { Sleep((DWORD)ms); }
static unsigned long tp_thread_id(void) { return (unsigned long)GetCurrentThreadId(); }

#else  /* POSIX */

typedef pthread_t       tp_thread_t;
typedef pthread_mutex_t tp_mutex_t;
typedef pthread_cond_t  tp_cond_t;

static int  tp_mutex_init   (tp_mutex_t *m) { return pthread_mutex_init(m, NULL); }
static void tp_mutex_destroy(tp_mutex_t *m) { pthread_mutex_destroy(m); }
static void tp_mutex_lock   (tp_mutex_t *m) { pthread_mutex_lock(m); }
static void tp_mutex_unlock (tp_mutex_t *m) { pthread_mutex_unlock(m); }

static int  tp_cond_init     (tp_cond_t *c) { return pthread_cond_init(c, NULL); }
static void tp_cond_destroy  (tp_cond_t *c) { pthread_cond_destroy(c); }
static void tp_cond_wait     (tp_cond_t *c, tp_mutex_t *m) { pthread_cond_wait(c, m); }
static void tp_cond_signal   (tp_cond_t *c) { pthread_cond_signal(c); }
static void tp_cond_broadcast(tp_cond_t *c) { pthread_cond_broadcast(c); }

typedef void * tp_thread_ret_t;
#define TP_THREAD_CALL

static int  tp_thread_create(tp_thread_t *t,
                              tp_thread_ret_t (TP_THREAD_CALL *fn)(void *),
                              void *arg)
{ return pthread_create(t, NULL, fn, arg); }
static void tp_thread_join(tp_thread_t t) { pthread_join(t, NULL); }

typedef struct timeval tp_time_t;
static tp_time_t tp_time_now(void) { struct timeval t; gettimeofday(&t, NULL); return t; }
static double tp_time_diff_ms(tp_time_t s, tp_time_t e) {
    return (double)(e.tv_sec  - s.tv_sec)  * 1000.0
         + (double)(e.tv_usec - s.tv_usec) / 1000.0;
}
static void tp_sleep_ms(int ms) { usleep((useconds_t)ms * 1000); }
static unsigned long tp_thread_id(void) { return (unsigned long)pthread_self(); }

#endif

/* =========================================================
   Thread pool (identical to original, unchanged)
   ========================================================= */

typedef void (*thread_pool_job_fn)(void *arg);

typedef struct thread_pool_job {
    thread_pool_job_fn  fn;
    void               *arg;
    struct thread_pool_job *next;
} thread_pool_job_t;

typedef struct thread_pool {
    tp_thread_t       *threads;
    size_t             thread_count;
    thread_pool_job_t *job_head;
    thread_pool_job_t *job_tail;
    size_t             queued_jobs;
    size_t             active_jobs;
    int                stop;
    int                initialized;
    tp_mutex_t         mutex;
    tp_cond_t          jobs_available;
    tp_cond_t          all_done;
} thread_pool_t;

static tp_thread_ret_t TP_THREAD_CALL thread_pool_worker(void *arg);

static int thread_pool_init(thread_pool_t *pool, size_t num_threads)
{
    size_t i;
    if (!pool || !num_threads) return -1;
    memset(pool, 0, sizeof(*pool));
    if (tp_mutex_init(&pool->mutex))               return -1;
    if (tp_cond_init(&pool->jobs_available)) { tp_mutex_destroy(&pool->mutex); return -1; }
    if (tp_cond_init(&pool->all_done))       { tp_cond_destroy(&pool->jobs_available); tp_mutex_destroy(&pool->mutex); return -1; }
    pool->threads = (tp_thread_t *)calloc(num_threads, sizeof(tp_thread_t));
    if (!pool->threads) { tp_cond_destroy(&pool->all_done); tp_cond_destroy(&pool->jobs_available); tp_mutex_destroy(&pool->mutex); return -1; }
    pool->thread_count = num_threads;
    pool->stop = 0; pool->initialized = 1;
    for (i = 0; i < num_threads; ++i) {
        if (tp_thread_create(&pool->threads[i], thread_pool_worker, pool)) {
            tp_mutex_lock(&pool->mutex); pool->stop = 1; tp_cond_broadcast(&pool->jobs_available); tp_mutex_unlock(&pool->mutex);
            while (i-- > 0) tp_thread_join(pool->threads[i]);
            free(pool->threads); tp_cond_destroy(&pool->all_done); tp_cond_destroy(&pool->jobs_available); tp_mutex_destroy(&pool->mutex);
            memset(pool, 0, sizeof(*pool)); return -1;
        }
    }
    return 0;
}

static int thread_pool_submit(thread_pool_t *pool, thread_pool_job_fn fn, void *arg)
{
    thread_pool_job_t *job;
    if (!pool || !fn || !pool->initialized) return -1;
    job = (thread_pool_job_t *)malloc(sizeof(*job));
    if (!job) return -1;
    job->fn = fn; job->arg = arg; job->next = NULL;
    tp_mutex_lock(&pool->mutex);
    if (pool->stop) { tp_mutex_unlock(&pool->mutex); free(job); return -1; }
    if (pool->job_tail) { pool->job_tail->next = job; pool->job_tail = job; }
    else                { pool->job_head = pool->job_tail = job; }
    pool->queued_jobs++;
    tp_cond_signal(&pool->jobs_available);
    tp_mutex_unlock(&pool->mutex);
    return 0;
}

static void thread_pool_wait(thread_pool_t *pool)
{
    if (!pool || !pool->initialized) return;
    tp_mutex_lock(&pool->mutex);
    while (pool->queued_jobs > 0 || pool->active_jobs > 0)
        tp_cond_wait(&pool->all_done, &pool->mutex);
    tp_mutex_unlock(&pool->mutex);
}

static void thread_pool_destroy(thread_pool_t *pool)
{
    size_t i; thread_pool_job_t *j, *n;
    if (!pool || !pool->initialized) return;
    tp_mutex_lock(&pool->mutex); pool->stop = 1; tp_cond_broadcast(&pool->jobs_available); tp_mutex_unlock(&pool->mutex);
    for (i = 0; i < pool->thread_count; ++i) tp_thread_join(pool->threads[i]);
    for (j = pool->job_head; j; j = n) { n = j->next; free(j); }
    free(pool->threads);
    tp_cond_destroy(&pool->all_done); tp_cond_destroy(&pool->jobs_available); tp_mutex_destroy(&pool->mutex);
    memset(pool, 0, sizeof(*pool));
}

static tp_thread_ret_t TP_THREAD_CALL thread_pool_worker(void *arg)
{
    thread_pool_t *pool = (thread_pool_t *)arg;
    for (;;) {
        thread_pool_job_t *job = NULL;
        tp_mutex_lock(&pool->mutex);
        while (!pool->stop && !pool->job_head) tp_cond_wait(&pool->jobs_available, &pool->mutex);
        if (pool->stop && !pool->job_head) { tp_mutex_unlock(&pool->mutex); break; }
        job = pool->job_head;
        pool->job_head = job->next;
        if (!pool->job_head) pool->job_tail = NULL;
        pool->queued_jobs--; pool->active_jobs++;
        tp_mutex_unlock(&pool->mutex);
        job->fn(job->arg); free(job);
        tp_mutex_lock(&pool->mutex);
        pool->active_jobs--;
        if (!pool->queued_jobs && !pool->active_jobs) tp_cond_broadcast(&pool->all_done);
        tp_mutex_unlock(&pool->mutex);
    }
#if defined(_WIN32)
    return 0;
#else
    return NULL;
#endif
}

/* =========================================================
   Simple LCG for in-job randomness (thread-local seed)
   ========================================================= */

static unsigned long lcg_next(unsigned long *s) {
    *s = *s * 6364136223846793005UL + 1442695040888963407UL;
    return *s;
}

/* =========================================================
   CPU stress: mixing loop (cheap SHA-256-like schedule)
   ========================================================= */

/* 32-bit rotate right */
#define ROTR32(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

static volatile unsigned int g_sink; /* prevent dead-code elimination */

static void cpu_burn(unsigned long long iters, unsigned long seed)
{
    unsigned int a = (unsigned int)seed ^ 0xDEADBEEFu;
    unsigned int b = (unsigned int)(seed >> 32) ^ 0xCAFEBABEu;
    unsigned int c = 0x12345678u;
    unsigned int d = 0xABCDEF01u;
    unsigned long long i;
    for (i = 0; i < iters; ++i) {
        a += b; b = ROTR32(b, 7)  ^ a;
        c += d; d = ROTR32(d, 13) ^ c;
        a ^= c; b += d;
        c  = ROTR32(c, 17); d ^= a;
    }
    g_sink = a ^ b ^ c ^ d;
}

/* =========================================================
   Log-normal sleep helper
   Box-Muller transform → log-normal sample
   ========================================================= */

/* Returns a log-normally distributed value with given mean_ms.
   sigma_log controls spread in log-space (0.6 ≈ 2x range).       */
static int lognormal_ms(double mean_ms, double sigma_log, unsigned long *rng)
{
    /* Two uniform samples in (0,1) */
    double u1 = ((double)(lcg_next(rng) & 0x7FFFFFFF) + 1.0) / 2147483649.0;
    double u2 = ((double)(lcg_next(rng) & 0x7FFFFFFF) + 1.0) / 2147483649.0;
    /* Box-Muller normal */
    double z  = sqrt(-2.0 * log(u1)) * cos(2.0 * 3.14159265358979323846 * u2);
    /* Convert to log-normal: median = mean_ms, spread = sigma_log */
    double sample = mean_ms * exp(sigma_log * z);
    if (sample < 1.0)       sample = 1.0;
    if (sample > mean_ms * 6.0) sample = mean_ms * 6.0; /* cap outliers */
    return (int)sample;
}

/* Uniform jitter: returns base ± frac*base */
static unsigned long long uniform_iters(unsigned long long base, double frac, unsigned long *rng)
{
    double r = (double)(lcg_next(rng) & 0xFFFF) / 65535.0; /* 0..1 */
    double scale = 1.0 - frac + 2.0 * frac * r;
    return (unsigned long long)(base * scale);
}

/* =========================================================
   Demo configuration
   ========================================================= */

#define NUM_JOBS        48
#define NUM_THREADS     2

/* CPU phase base iterations (tune so each phase ~100-300 ms on your hw) */
#define CPU_BASE_ITERS  20000000ULL   /* ~100 ms on a modern core at -O2 */

/* I/O simulation base: mean sleep in milliseconds */
#define IO_BASE_MS      250.0

/* =========================================================
   Job definition
   ========================================================= */

typedef struct {
    int    job_id;
    /* durations actually used (set by job itself) */
    unsigned long long cpu1_iters;
    unsigned long long cpu2_iters;
    int    io_ms;
    /* timing */
    double started_at_ms;
    double finished_at_ms;
    double duration_ms;
} heavy_job_arg_t;

static tp_mutex_t g_print_mutex;
static tp_time_t  g_batch_start;

static void heavy_job(void *arg)
{
    heavy_job_arg_t *a = (heavy_job_arg_t *)arg;
    tp_time_t now;
    unsigned long rng_seed;

    /* Seed per-job RNG from job_id so runs are reproducible */
    rng_seed = (unsigned long)a->job_id * 2654435761UL ^ 0xFEEDF00D;

    /* --- derive variable workload parameters --- */
    a->cpu1_iters = uniform_iters(CPU_BASE_ITERS,        0.50, &rng_seed);
    a->cpu2_iters = uniform_iters(CPU_BASE_ITERS / 4ULL, 0.25, &rng_seed);
    a->io_ms      = lognormal_ms(IO_BASE_MS, 0.6, &rng_seed);

    /* Record start */
    now = tp_time_now();
    a->started_at_ms = tp_time_diff_ms(g_batch_start, now);

    tp_mutex_lock(&g_print_mutex);
    printf("  [T %6lu]  job %2d  START  t=%7.0f ms  "
           "(cpu1=%4.0fM iters, io=%3d ms, cpu2=%4.0fM iters)\n",
           tp_thread_id(), a->job_id, a->started_at_ms,
           (double)a->cpu1_iters / 1e6,
           a->io_ms,
           (double)a->cpu2_iters / 1e6);
    fflush(stdout);
    tp_mutex_unlock(&g_print_mutex);

    /* === Phase 1: CPU crunch (simulate data processing) === */
    cpu_burn(a->cpu1_iters, rng_seed);

    /* === Phase 2: I/O wait (simulate network/disk latency) === */
    tp_sleep_ms(a->io_ms);

    /* === Phase 3: CPU crunch (simulate result validation) === */
    cpu_burn(a->cpu2_iters, rng_seed ^ 0x5A5A5A5AUL);

    /* Record finish */
    now = tp_time_now();
    a->finished_at_ms = tp_time_diff_ms(g_batch_start, now);
    a->duration_ms    = a->finished_at_ms - a->started_at_ms;

    tp_mutex_lock(&g_print_mutex);
    printf("  [T %6lu]  job %2d  END    t=%7.0f ms  duration=%5.0f ms\n",
           tp_thread_id(), a->job_id, a->finished_at_ms, a->duration_ms);
    fflush(stdout);
    tp_mutex_unlock(&g_print_mutex);

    /* arg is owned by caller (main) – do NOT free here */
}

/* =========================================================
   Statistics helpers
   ========================================================= */

static double arr_min(double *v, int n) { double m = v[0]; int i; for (i=1;i<n;i++) if(v[i]<m) m=v[i]; return m; }
static double arr_max(double *v, int n) { double m = v[0]; int i; for (i=1;i<n;i++) if(v[i]>m) m=v[i]; return m; }
static double arr_mean(double *v, int n) { double s=0; int i; for(i=0;i<n;i++) s+=v[i]; return s/n; }
static double arr_stddev(double *v, int n, double mean) {
    double s=0; int i; for(i=0;i<n;i++) { double d=v[i]-mean; s+=d*d; } return sqrt(s/n);
}

/* =========================================================
   main
   ========================================================= */

int main(void)
{
    thread_pool_t    pool;
    heavy_job_arg_t  jobs[NUM_JOBS];
    tp_time_t        wall_start, wall_end;
    double           wall_ms, serial_est_ms;
    double           durations[NUM_JOBS];
    double           d_min, d_max, d_mean, d_std;
    int              i;

    printf("=============================================================\n");
    printf("  Thread-pool  –  Heavy mixed workload demo\n");
    printf("  Jobs: %d   |   Threads: %d\n", NUM_JOBS, NUM_THREADS);
    printf("  Per-job phases: [CPU ±50%%]  [I/O log-normal ~%.0fms]  [CPU ±25%%]\n",
           IO_BASE_MS);
    printf("  CPU base: %.0fM iters/phase\n", (double)CPU_BASE_ITERS / 1e6);
    printf("=============================================================\n\n");

    if (tp_mutex_init(&g_print_mutex)) {
        fprintf(stderr, "Failed to init print mutex\n"); return 1;
    }

    if (thread_pool_init(&pool, NUM_THREADS)) {
        fprintf(stderr, "Failed to init thread pool\n");
        tp_mutex_destroy(&g_print_mutex); return 1;
    }

    /* Initialise job descriptors */
    for (i = 0; i < NUM_JOBS; ++i) {
        memset(&jobs[i], 0, sizeof(jobs[i]));
        jobs[i].job_id = i;
    }

    g_batch_start = tp_time_now();
    wall_start    = g_batch_start;

    for (i = 0; i < NUM_JOBS; ++i) {
        if (thread_pool_submit(&pool, heavy_job, &jobs[i])) {
            fprintf(stderr, "Failed to submit job %d\n", i);
            thread_pool_destroy(&pool); tp_mutex_destroy(&g_print_mutex); return 1;
        }
    }

    thread_pool_wait(&pool);

    wall_end = tp_time_now();
    wall_ms  = tp_time_diff_ms(wall_start, wall_end);

    /* Collect duration array and compute serial estimate */
    serial_est_ms = 0.0;
    for (i = 0; i < NUM_JOBS; ++i) {
        durations[i]   = jobs[i].duration_ms;
        serial_est_ms += durations[i];
    }

    d_min  = arr_min  (durations, NUM_JOBS);
    d_max  = arr_max  (durations, NUM_JOBS);
    d_mean = arr_mean (durations, NUM_JOBS);
    d_std  = arr_stddev(durations, NUM_JOBS, d_mean);

    printf("\n=============================================================\n");
    printf("  All %d jobs complete.\n\n", NUM_JOBS);
    printf("  Job duration statistics (ms):\n");
    printf("    Min    : %6.0f ms\n", d_min);
    printf("    Max    : %6.0f ms\n", d_max);
    printf("    Mean   : %6.0f ms\n", d_mean);
    printf("    Std Dev: %6.0f ms\n", d_std);
    printf("    CV     : %6.1f %%   (coefficient of variation)\n",
           100.0 * d_std / d_mean);
    printf("\n");
    printf("  Wall-clock time  : %7.0f ms\n", wall_ms);
    printf("  Serial estimate  : %7.0f ms  (sum of all job durations)\n", serial_est_ms);
    printf("  Observed speedup : %7.2fx\n",   serial_est_ms / wall_ms);
    printf("  Theoretical max  : %7.2fx  (%d threads)\n",
           (double)NUM_THREADS, NUM_THREADS);
    printf("  Efficiency       : %7.1f %%\n",
           100.0 * (serial_est_ms / wall_ms) / NUM_THREADS);
    printf("=============================================================\n");

    thread_pool_destroy(&pool);
    tp_mutex_destroy(&g_print_mutex);
    return 0;
}
