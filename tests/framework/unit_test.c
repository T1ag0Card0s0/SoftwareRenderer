#include "unit_test.h"
#include <assert.h>

unit_test_ctx_t ut_current_ctx = {0};
unit_test_ctx_t ut_global_ctx = {0};

void ut_run(unit_test_fn entry)
{
  assert(entry);

  ut_current_ctx.pass = 0;
  ut_current_ctx.fail = 0;

  entry();

  ut_global_ctx.pass += ut_current_ctx.pass;
  ut_global_ctx.fail += ut_current_ctx.fail;
}

int ut_result(void)
{
  return ut_global_ctx.fail;
}
