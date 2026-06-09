#include "algorithms/sort.h"
#include "core/operations.h"

static bool merge_ranges(SortContext *ctx, size_t left, size_t middle, size_t right)
{
    size_t first = left;
    size_t second = middle;

    while (first < second && second < right) {
        int comparison = ops_compare(ctx, first, second);
        if (!ops_render_step(ctx))
            return false;
        if (comparison <= 0) {
            first++;
            continue;
        }

        for (size_t current = second; current > first; current--) {
            ops_swap(ctx, current - 1, current);
            if (!ops_render_step(ctx))
                return false;
        }
        first++;
        second++;
    }
    return true;
}

static bool merge_sort_range(SortContext *ctx, size_t left, size_t right)
{
    if (right - left < 2)
        return true;

    size_t middle = left + (right - left) / 2;
    if (!merge_sort_range(ctx, left, middle))
        return false;
    if (!merge_sort_range(ctx, middle, right))
        return false;
    return merge_ranges(ctx, left, middle, right);
}

void sort_merge(SortContext *ctx)
{
    if (!ctx || !ctx->values || ctx->length < 2)
        return;

    context_reset_stats(ctx);
    merge_sort_range(ctx, 0, ctx->length);
    ctx->active_index = -1;
    ctx->compared_index = -1;
}
