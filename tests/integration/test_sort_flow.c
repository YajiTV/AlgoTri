#include <stddef.h>

#include "test_runner.h"
#include "algorithms/sort.h"
#include "core/context.h"
#include "core/operations.h"

typedef void (*SortFn)(SortContext *);

static const SortFn ALGOS[] = {
    sort_bubble,
    sort_selection,
    sort_insertion,
    sort_quick,
};

#define ALGO_COUNT (sizeof(ALGOS) / sizeof(ALGOS[0]))

static bool is_sorted(const SortContext *ctx)
{
    for (size_t i = 1; i < ctx->length; i++) {
        if (ctx->values[i - 1] > ctx->values[i])
            return false;
    }
    return true;
}

static bool has_unique_values(const SortContext *ctx)
{
    for (size_t i = 0; i < ctx->length; i++) {
        for (size_t j = i + 1; j < ctx->length; j++) {
            if (ctx->values[i] == ctx->values[j])
                return false;
        }
    }
    return true;
}

/* Full pipeline: create -> randomize -> sort -> verify -> destroy */
static void test_full_sort_pipeline(void)
{
    const size_t sizes[] = {1, 2, 10, 20, 40};

    for (size_t s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
        for (size_t a = 0; a < ALGO_COUNT; a++) {
            SortContext *ctx = context_create(sizes[s]);
            ASSERT(ctx != NULL);
            ctx->delay_ms = 0;

            ALGOS[a](ctx);

            ASSERT(is_sorted(ctx));
            ASSERT(ctx->active_index == -1);
            ASSERT(ctx->compared_index == -1);

            context_destroy(ctx);
        }
    }
}

/* Randomize guarantees unique values in range [1..N] */
static void test_randomize_produces_permutation(void)
{
    SortContext *ctx = context_create(30);
    ASSERT(ctx != NULL);

    for (int trial = 0; trial < 5; trial++) {
        context_randomize(ctx);
        ASSERT(has_unique_values(ctx));
        for (size_t i = 0; i < ctx->length; i++) {
            ASSERT(ctx->values[i] >= 1);
            ASSERT(ctx->values[i] <= (int)ctx->length);
        }
    }
    context_destroy(ctx);
}

/* Resize then sort: memory is reallocated, sort still works */
static void test_resize_then_sort(void)
{
    SortContext *ctx = context_create(5);
    ASSERT(ctx != NULL);
    ctx->delay_ms = 0;

    ASSERT(context_resize(ctx, 25));
    ASSERT(ctx->length == 25);

    sort_quick(ctx);
    ASSERT(is_sorted(ctx));

    ASSERT(context_resize(ctx, 3));
    sort_bubble(ctx);
    ASSERT(is_sorted(ctx));

    context_destroy(ctx);
}

/* Consecutive sorts on the same context */
static void test_consecutive_sorts(void)
{
    SortContext *ctx = context_create(15);
    ASSERT(ctx != NULL);
    ctx->delay_ms = 0;

    for (size_t a = 0; a < ALGO_COUNT; a++) {
        context_randomize(ctx);
        ALGOS[a](ctx);
        ASSERT(is_sorted(ctx));
        ASSERT(ctx->comparisons > 0);
    }
    context_destroy(ctx);
}

/* Statistics reset between sorts */
static void test_stats_reset_between_sorts(void)
{
    const int sorted_values[] = {1, 2, 3, 4, 5};
    SortContext *ctx = context_create(5);
    ASSERT(ctx != NULL);
    ctx->delay_ms = 0;

    ASSERT(context_set_values(ctx, sorted_values, 5));
    ctx->comparisons = 999;
    ctx->swaps = 999;
    sort_bubble(ctx);
    ASSERT(ctx->comparisons == 4);
    ASSERT(ctx->swaps == 0);

    context_destroy(ctx);
}

/* ops_compare and ops_swap update indices and counters correctly */
static void test_operations_update_state(void)
{
    SortContext *ctx = context_create(5);
    ASSERT(ctx != NULL);
    ctx->delay_ms = 0;

    size_t before_cmp = ctx->comparisons;
    ops_compare(ctx, 0, 1);
    ASSERT(ctx->comparisons == before_cmp + 1);
    ASSERT(ctx->active_index == 0);
    ASSERT(ctx->compared_index == 1);
    ASSERT(ctx->operation == OPERATION_COMPARE);

    int val0 = ctx->values[0];
    int val1 = ctx->values[1];
    ops_swap(ctx, 0, 1);
    ASSERT(ctx->values[0] == val1);
    ASSERT(ctx->values[1] == val0);
    ASSERT(ctx->swaps == 1);
    ASSERT(ctx->operation == OPERATION_SWAP);

    context_destroy(ctx);
}

int main(void)
{
    test_full_sort_pipeline();
    test_randomize_produces_permutation();
    test_resize_then_sort();
    test_consecutive_sorts();
    test_stats_reset_between_sorts();
    test_operations_update_state();
    TEST_SUMMARY();
}
