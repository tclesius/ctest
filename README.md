# ctest
A tiny testing framework
<img width="1000" height="300" alt="image" src="https://github.com/user-attachments/assets/125211d3-916a-4821-b543-518f19f9c4cd" />

- ~160 lines of C99
- Zero-allocations
- Fixtures Support

## Usage
ctest is dead simple:
```c
#include "ctest.h"

void test_add(ctest_ctx_t *ctx) {
  int a = 21, b = 21;

  int got = a + b;
  int want = 42;

  if (got != want) {
    ctest_failf(ctx, "wanted %d, got %d", want, got);
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
