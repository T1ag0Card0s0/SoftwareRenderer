#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include <stdio.h>

typedef void (*unit_test_fn)(void);

typedef struct
{
    unsigned int pass;
    unsigned int fail;
} unit_test_ctx_t;

extern unit_test_ctx_t ut_current_ctx;
extern unit_test_ctx_t ut_global_ctx;

void ut_run(unit_test_fn entry);
int  ut_result(void);

#define UT_RUN(fn)      \
  do { \
    ut_run((fn)); \
    printf("Result — Passed:%u Failed:%u\n", \
           ut_current_ctx.pass, \
           ut_current_ctx.fail); \
  } while(0);

#define UT_RESULT()     ut_result()

#define UT_ASSERT_TRUE(cond)                                  \
    do {                                                      \
        if (cond) {                                           \
            ut_current_ctx.pass++;                            \
        } else {                                              \
            ut_current_ctx.fail++;                            \
            printf("FAIL %s:%d: %s\n",                        \
                   __FILE__, __LINE__, #cond);                \
        }                                                     \
    } while(0)


#define UT_ASSERT_EQ(lhs, rhs)                                        \
    do {                                                               \
        long long _l = (long long)(lhs);                              \
        long long _r = (long long)(rhs);                              \
        if (_l == _r) {                                                \
            ut_current_ctx.pass++;                                     \
        } else {                                                       \
            ut_current_ctx.fail++;                                     \
            printf("FAIL %s:%d: %s != %s (%lld vs %lld)\n",            \
                   __FILE__, __LINE__, #lhs, #rhs, _l, _r);            \
        }                                                              \
    } while(0)

#endif

