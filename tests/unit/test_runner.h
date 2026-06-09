#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <stdio.h>

static int test_passed = 0;
static int test_failed = 0;

#define ASSERT(expr) do { \
    if (expr) { \
        test_passed++; \
    } else { \
        fprintf(stderr, "FAIL: %s:%d — %s\n", __FILE__, __LINE__, #expr); \
        test_failed++; \
    } \
} while (0)

#define TEST_SUMMARY() do { \
    printf("\nRésultats : %d passé(s), %d échoué(s)\n", test_passed, test_failed); \
    return test_failed == 0 ? 0 : 1; \
} while (0)

#endif
