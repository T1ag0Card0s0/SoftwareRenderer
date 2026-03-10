#ifndef PLATFORM_TYPES_H
#define  PLATFORM_TYPES_H

#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

typedef pthread_t       platform_thread_t;
typedef pthread_mutex_t platform_mutex_t;
typedef pthread_cond_t  platform_cond_t;
typedef struct timeval platform_time_t;

typedef void * platform_thread_return_t;
#define PLATFORM_THREAD_CALL

#endif // PLATFORM_TYPES_H
