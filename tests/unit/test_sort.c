#include <stddef.h>

#include "test_runner.h"
#include "algorithms/sort.h"
#include "core/context.h"

typedef void (*SortFunction)(SortContext *);

static const SortFunction algorithms[] = {
    sort_bubble,
    sort_selection,
    sort_insertion,
    sort_quick
};

static bool interrupt_control(SortContext *ctx)
{
    ctx->interrupted = true;
    return false;
}

static bool is_sorted(const SortContext *ctx)
{
    for (size_t i = 1; i < ctx->length; i++) {
        if (ctx->values[i - 1] > ctx->values[i])
            return false;
    }
    return true;
}

static void assert_algorithm_sorts(SortFunction sort, const int *values, size_t length)
{
    SortContext *ctx = context_create(length);
    ASSERT(ctx != NULL);
    ASSERT(context_set_values(ctx, values, length));
    ctx->delay_ms = 0;
    sort(ctx);
    ASSERT(is_sorted(ctx));
    ASSERT(ctx->active_index == -1);
    ASSERT(ctx->compared_index == -1);
    context_destroy(ctx);
}

static void test_common_inputs(void)
{
    const int reversed[] = {5, 4, 3, 2, 1};
    const int sorted[] = {1, 2, 3, 4, 5};
    const int duplicates[] = {4, 2, 4, 1, 2, 1};
    const int mixed[] = {0, -8, 12, -3, 12, 5};

    for (size_t i = 0; i < sizeof(algorithms) / sizeof(algorithms[0]); i++) {
        assert_algorithm_sorts(algorithms[i], reversed, 5);
        assert_algorithm_sorts(algorithms[i], sorted, 5);
        assert_algorithm_sorts(algorithms[i], duplicates, 6);
        assert_algorithm_sorts(algorithms[i], mixed, 6);
    }
}

static void test_single_and_empty_contexts(void)
{
    SortContext empty = {0};

    for (size_t i = 0; i < sizeof(algorithms) / sizeof(algorithms[0]); i++) {
        SortContext *single = context_create(1);
        ASSERT(single != NULL);
        algorithms[i](single);
        ASSERT(is_sorted(single));
        context_destroy(single);
        algorithms[i](&empty);
        algorithms[i](NULL);
    }
}

static void test_statistics_are_tracked(void)
{
    const int values[] = {3, 1, 2};

    for (size_t i = 0; i < sizeof(algorithms) / sizeof(algorithms[0]); i++) {
        SortContext *ctx = context_create(3);
        ASSERT(ctx != NULL);
        ASSERT(context_set_values(ctx, values, 3));
        ctx->delay_ms = 0;
        algorithms[i](ctx);
        ASSERT(ctx->comparisons > 0);
        ASSERT(ctx->swaps > 0);
        context_destroy(ctx);
    }
}

static void test_algorithms_can_be_interrupted(void)
{
    const int values[] = {5, 4, 3, 2, 1};

    for (size_t i = 0; i < sizeof(algorithms) / sizeof(algorithms[0]); i++) {
        SortContext *ctx = context_create(5);
        ASSERT(ctx != NULL);
        ASSERT(context_set_values(ctx, values, 5));
        ctx->delay_ms = 0;
        ctx->control_fn = interrupt_control;
        algorithms[i](ctx);
        ASSERT(ctx->interrupted);
        ASSERT(ctx->active_index == -1);
        ASSERT(ctx->compared_index == -1);
        context_destroy(ctx);
    }
}

int main(void)
{
    test_common_inputs();
    test_single_and_empty_contexts();
    test_statistics_are_tracked();
    test_algorithms_can_be_interrupted();
    TEST_SUMMARY();
}
