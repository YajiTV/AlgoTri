#include "test_runner.h"
#include "core/context.h"
#include "core/operations.h"

static bool interrupt_control(SortContext *ctx)
{
    ctx->interrupted = true;
    return false;
}

static void test_compare_tracks_operation(void)
{
    const int values[] = {4, 9};
    SortContext *ctx = context_create(2);
    ASSERT(ctx != NULL);
    ASSERT(context_set_values(ctx, values, 2));
    ASSERT(ops_compare(ctx, 0, 1) < 0);
    ASSERT(ctx->comparisons == 1);
    ASSERT(ctx->active_index == 0);
    ASSERT(ctx->compared_index == 1);
    ASSERT(ctx->operation == OPERATION_COMPARE);
    context_destroy(ctx);
}

static void test_swap_tracks_operation(void)
{
    const int values[] = {4, 9};
    SortContext *ctx = context_create(2);
    ASSERT(ctx != NULL);
    ASSERT(context_set_values(ctx, values, 2));
    ops_swap(ctx, 0, 1);
    ASSERT(ctx->values[0] == 9);
    ASSERT(ctx->values[1] == 4);
    ASSERT(ctx->swaps == 1);
    ASSERT(ctx->operation == OPERATION_SWAP);
    ops_swap(ctx, 0, 0);
    ASSERT(ctx->swaps == 1);
    context_destroy(ctx);
}

static void test_invalid_operations_are_ignored(void)
{
    SortContext *ctx = context_create(2);
    ASSERT(ctx != NULL);
    ASSERT(ops_compare(ctx, 0, 3) == 0);
    ASSERT(ctx->comparisons == 0);
    ops_swap(ctx, 0, 3);
    ASSERT(ctx->swaps == 0);
    ASSERT(ops_compare(NULL, 0, 0) == 0);
    ops_swap(NULL, 0, 0);
    ASSERT(!ops_render_step(NULL));
    context_destroy(ctx);
}

static void test_render_step_can_interrupt(void)
{
    SortContext *ctx = context_create(2);
    ASSERT(ctx != NULL);
    ctx->delay_ms = 0;
    ctx->control_fn = interrupt_control;
    ASSERT(!ops_render_step(ctx));
    ASSERT(ctx->interrupted);
    context_destroy(ctx);
}

int main(void)
{
    test_compare_tracks_operation();
    test_swap_tracks_operation();
    test_invalid_operations_are_ignored();
    test_render_step_can_interrupt();
    TEST_SUMMARY();
}
