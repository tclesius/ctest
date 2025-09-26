
/**
 * Copyright (c) 2025 Tom Clesius
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `ctest.c` for details.
 */

#ifndef CTEST_H
#define CTEST_H

#include <stddef.h>

#define CTEST_VERSION "0.1.0"

typedef struct ctest_ctx ctest_ctx_t;

typedef struct {
  const char *name;
  void (*fn)(ctest_ctx_t *t);
} ctest_test_t;

void *ctest_env(ctest_ctx_t *ctx, size_t index);

void ctest_failf(ctest_ctx_t *ctx, const char *fmt, ...);
void ctest_fail(ctest_ctx_t *ctx);
void ctest_skip(ctest_ctx_t *ctx, const char *reason);

int ctest_run(ctest_test_t tests[], void (*setup)(void *env[]), void (*teardown)(void *env[]));

#endif
