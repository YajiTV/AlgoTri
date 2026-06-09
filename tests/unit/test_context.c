#include <stddef.h>

#include "test_runner.h"
#include "core/context.h"

static void test_create_valid(void)
{
    SortContext *ctx = context_create(10);
    ASSERT(ctx != NULL);
    ASSERT(ctx->values != NULL);
    ASSERT(ctx->length == 10);
    ASSERT(ctx->comparisons == 0);
    ASSERT(ctx->swaps == 0);
    ASSERT(ctx->paused == false);
    context_destroy(ctx);
}

static void test_create_zero(void)
{
    SortContext *ctx = context_create(0);
    ASSERT(ctx == NULL);
}

static void test_randomize_bounds(void)
{
    SortContext *ctx = context_create(50);
    ASSERT(ctx != NULL);
    context_randomize(ctx);
    for (size_t i = 0; i < ctx->length; i++) {
        ASSERT(ctx->values[i] >= 1);
        ASSERT(ctx->values[i] <= (int)ctx->length);
    }
    context_destroy(ctx);
}

static void test_reset_stats(void)
{
    SortContext *ctx = context_create(5);
    ASSERT(ctx != NULL);
    ctx->comparisons = 10;
    ctx->swaps       = 5;
    context_reset_stats(ctx);
    ASSERT(ctx->comparisons == 0);
    ASSERT(ctx->swaps == 0);
    context_destroy(ctx);
}

int main(void)
{
    test_create_valid();
    test_create_zero();
    test_randomize_bounds();
    test_reset_stats();
    TEST_SUMMARY();
}
