#include "algorithms/sort.h"
#include "core/operations.h"

static bool partition(SortContext *ctx, size_t low, size_t high, size_t *pivot)
{
    size_t store = low;

    for (size_t i = low; i < high; i++) {
        int comparison = ops_compare(ctx, i, high);
        if (!ops_render_step(ctx))
            return false;
        if (comparison <= 0) {
            ops_swap(ctx, store, i);
            if (!ops_render_step(ctx))
                return false;
            store++;
        }
    }
    ops_swap(ctx, store, high);
    if (!ops_render_step(ctx))
        return false;
    *pivot = store;
    return true;
}

static bool quick_sort_range(SortContext *ctx, size_t low, size_t high)
{
    if (low >= high)
        return true;

    size_t pivot;
    if (!partition(ctx, low, high, &pivot))
        return false;

    if (pivot > low && !quick_sort_range(ctx, low, pivot - 1))
        return false;
    if (pivot < high && !quick_sort_range(ctx, pivot + 1, high))
        return false;
    return true;
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
