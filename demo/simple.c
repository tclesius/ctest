
#include "../src/ctest.h"

void test_pass(ctest_ctx_t *ctx) {
  int a = 42, b = 42;

  int got = a + b;
  int expect = 84;

  if (got != expect) {
    ctest_failf(ctx, "wanted %d, got %d", expect, got);
  }
}

void test_skip(ctest_ctx_t *ctx) {
  ctest_skip(ctx, "This test isn't expected to be fixed");

  int a = 42, b = 42;

  int got = a + b;
  int expect = 420;

  if (got != expect) {
    ctest_failf(ctx, "wanted %d, got %d", expect, got);
  }
}

void test_fail(ctest_ctx_t *ctx) {
  int a = 42, b = 42;

  int got = a + b;
  int expect = 420;

  if (got != expect) {
    ctest_failf(ctx, "wanted %d, got %d", expect, got);
  }
}

int main(void) {
  ctest_test_t tests[] = {
      {"test_pass", test_pass},
      {"test_skip", test_skip},
      {"test_fail", test_fail},
      {NULL, NULL},
  };
  return ctest_run(tests, NULL, NULL);
}
