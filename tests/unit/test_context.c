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

static void assert_values_are_unique(const SortContext *ctx)
{
    for (size_t i = 0; i < ctx->length; i++) {
        size_t occurrences = 0;

        for (size_t j = 0; j < ctx->length; j++) {
            if (ctx->values[j] == (int)(i + 1))
                occurrences++;
        }
        ASSERT(occurrences == 1);
    }
}

static void test_randomize_unique_values(void)
{
    SortContext *ctx = context_create(40);
    ASSERT(ctx != NULL);
    assert_values_are_unique(ctx);
    context_randomize(ctx);
    assert_values_are_unique(ctx);
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

static void test_set_values(void)
{
    const int values[] = {9, -2, 9, 4};
    SortContext *ctx = context_create(2);
    ASSERT(ctx != NULL);
    ASSERT(context_set_values(ctx, values, 4));
    ASSERT(ctx->length == 4);
    for (size_t i = 0; i < ctx->length; i++)
        ASSERT(ctx->values[i] == values[i]);
    ASSERT(!context_set_values(ctx, NULL, 4));
    ASSERT(!context_set_values(NULL, values, 4));
    ASSERT(!context_set_values(ctx, values, 0));
    context_destroy(ctx);
}

static void test_resize(void)
{
    SortContext *ctx = context_create(4);
    ASSERT(ctx != NULL);
    ASSERT(context_resize(ctx, 12));
    ASSERT(ctx->length == 12);
    for (size_t i = 0; i < ctx->length; i++) {
        ASSERT(ctx->values[i] >= 1);
        ASSERT(ctx->values[i] <= (int)ctx->length);
    }
    assert_values_are_unique(ctx);
    ASSERT(!context_resize(ctx, 0));
    ASSERT(!context_resize(NULL, 12));
    context_destroy(ctx);
}

int main(void)
{
    test_create_valid();
    test_create_zero();
    test_randomize_bounds();
    test_randomize_unique_values();
    test_reset_stats();
    test_set_values();
    test_resize();
    TEST_SUMMARY();
}
