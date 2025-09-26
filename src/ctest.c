/*
 * Copyright (c) 2025 Tom Clesius
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "ctest.h"
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#define CTEST_COLOR_RESET "\033[0m"
#define CTEST_COLOR_PASS "\033[32m"
#define CTEST_COLOR_FAIL "\033[31m"
#define CTEST_COLOR_SKIP "\033[33m"

typedef enum {
  CTEST_FAIL,
  CTEST_PASS,
  CTEST_SKIP
} ctest_state_t;

struct ctest_ctx {
  ctest_state_t state;
  char message[256];
  const char *name;
  jmp_buf jump;
  void **env;
};

void *ctest_env(ctest_ctx_t *ctx, size_t index) {
  if (!ctx) {
    return NULL;
  }
  return ctx->env[index];
}

void ctest_failf(ctest_ctx_t *ctx, const char *fmt, ...) {
  if (!ctx) {
    return;
  }
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(ctx->message, sizeof(ctx->message), fmt, ap);
  ctx->state = CTEST_FAIL;
  va_end(ap);
}

void ctest_fail(ctest_ctx_t *ctx) {
  if (!ctx) {
    return;
  }
  ctx->state = CTEST_FAIL;
}

void ctest_skip(ctest_ctx_t *ctx, const char *reason) {
  if (!ctx) {
    return;
  }
  ctx->state = CTEST_SKIP;
  snprintf(ctx->message, sizeof(ctx->message), "%s", reason);
  longjmp(ctx->jump, 1);
}

int ctest_run(ctest_test_t tests[], void (*setup)(void *env[]), void (*teardown)(void *env[])) {
  size_t failed = 0, passed = 0, skipped = 0;
  double total_ms = 0.0;

  for (int i = 0; tests[i].fn != NULL; i++) {
    void *env[255] = {0};
    if (setup) {
      setup(env);
    }

    ctest_ctx_t t = {
        .state = CTEST_PASS,
        .env = env,
        .name = tests[i].name,
        .message = "\0",
    };

    struct timespec begin = {0}, end = {0};
    clock_gettime(CLOCK_MONOTONIC, &begin);

    if (setjmp(t.jump) == 0) {
      tests[i].fn(&t);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed_ms = (end.tv_sec - begin.tv_sec) * 1000.0 +
                        (end.tv_nsec - begin.tv_nsec) / 1000000.0;
    total_ms += elapsed_ms;

    if (teardown) {
      teardown(env);
    }

    switch (t.state) {
    case CTEST_PASS:
      printf(CTEST_COLOR_PASS "PASS" CTEST_COLOR_RESET ": %s\t(%.2f ms)\n", t.name, elapsed_ms);
      passed++;
      break;
    case CTEST_FAIL:
      printf(CTEST_COLOR_FAIL "FAIL" CTEST_COLOR_RESET ": %s\t(%.2f ms)\n", t.name, elapsed_ms);
      printf("\t%s\n", t.message);
      failed++;
      break;
    case CTEST_SKIP:
      printf(CTEST_COLOR_SKIP "SKIP" CTEST_COLOR_RESET ": %s\t(%s)\n", t.name, t.message);
      skipped++;
      break;
    default:
      printf(CTEST_COLOR_FAIL "INVALID STATE" CTEST_COLOR_RESET ": %s\n", t.name);
    }
  }
  size_t total = passed + failed + skipped;
  printf("\n%lu failed, %lu passed, %lu skipped, %lu total (%.2f ms)\n", failed, passed, skipped, total, total_ms);
  return -failed;
}
