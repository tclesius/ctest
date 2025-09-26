#include "../src/ctest.h"
#include <stdlib.h>

typedef struct {
  int value;
} fixture_t;

void setup(void *env[]) {
  fixture_t *fixture = malloc(sizeof(*fixture));
  if (!fixture) {
    return;
  }

  fixture->value = 1;
  env[0] = fixture;
}

void teardown(void *env[]) {
  fixture_t *fixture = env[0];
  free(fixture);
}

void test_increment(ctest_ctx_t *ctx) {
  fixture_t *fixture = ctest_env(ctx, 0);
  fixture->value += 1;

  if (fixture->value != 2) {
    ctest_failf(ctx, "expected 2, got %d", fixture->value);
  }
}

void test_initial(ctest_ctx_t *ctx) {
  fixture_t *fixture = ctest_env(ctx, 0);

  if (fixture->value != 1) {
    ctest_failf(ctx, "setup should reset value to 1, got %d", fixture->value);
  }
}

int main(void) {
  ctest_test_t tests[] = {
      {"test_increment", test_increment},
      {"test_initial", test_initial},
      {NULL, NULL},
  };
  return ctest_run(tests, setup, teardown);
}
