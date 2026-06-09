#include "algorithms/sort.h"
#include "core/operations.h"

static size_t partition(SortContext *ctx, size_t low, size_t high)
{
    size_t store = low;

    for (size_t i = low; i < high; i++) {
        int comparison = ops_compare(ctx, i, high);
        ops_render_step(ctx);
        if (comparison <= 0) {
            ops_swap(ctx, store, i);
            ops_render_step(ctx);
            store++;
        }
    }
    ops_swap(ctx, store, high);
    ops_render_step(ctx);
    return store;
}

static void quick_sort_range(SortContext *ctx, size_t low, size_t high)
{
    if (low >= high)
        return;

    size_t pivot = partition(ctx, low, high);

    if (pivot > low)
        quick_sort_range(ctx, low, pivot - 1);
    if (pivot < high)
        quick_sort_range(ctx, pivot + 1, high);
}

void sort_quick(SortContext *ctx)
{
    if (!ctx || !ctx->values || ctx->length < 2)
        return;

    context_reset_stats(ctx);
    quick_sort_range(ctx, 0, ctx->length - 1);
    ctx->active_index = -1;
    ctx->compared_index = -1;
}
