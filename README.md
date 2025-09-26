# ctest
A tiny testing framework

- ~160 lines of C99
- Zero-allocations
- Fixtures
- ANSI colored logs with timings
- Neat summary

## Usage
ctest is dead simple:
```c
#include "ctest.h"

void test_add(ctest_ctx_t *ctx) {
  int a = 21, b = 21;

  int got = a + b;
  int expect = 42;

  if (got != expect) {
    ctest_failf(ctx, "wanted %d, got %d", expect, got);
  }
}

int main(void) {
  ctest_test_t tests[] = {
      {"Test Add", test_add},
      {NULL, NULL},
  };
  return ctest_run(tests, NULL, NULL);
}
```
See the demo folder for further usage examples.

## License
This is free and unencumbered software released into the public domain. See LICENSE for details.