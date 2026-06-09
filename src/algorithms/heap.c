#include "algorithms/sort.h"
#include "core/operations.h"

static bool sift_down(SortContext *ctx, size_t root, size_t end)
{
    while (root * 2 + 1 <= end) {
        size_t child = root * 2 + 1;

        if (child + 1 <= end) {
            int comparison = ops_compare(ctx, child, child + 1);
            if (!ops_render_step(ctx))
                return false;
            if (comparison < 0)
                child++;
        }

        int comparison = ops_compare(ctx, root, child);
        if (!ops_render_step(ctx))
            return false;
        if (comparison >= 0)
            return true;

        ops_swap(ctx, root, child);
        if (!ops_render_step(ctx))
            return false;
        root = child;
    }
    return true;
}

void sort_heap(SortContext *ctx)
{
    if (!ctx || !ctx->values || ctx->length < 2)
        return;

    context_reset_stats(ctx);
    for (size_t start = ctx->length / 2; start > 0; start--) {
        if (!sift_down(ctx, start - 1, ctx->length - 1))
            goto finish;
    }

    for (size_t end = ctx->length - 1; end > 0; end--) {
        ops_swap(ctx, 0, end);
        if (!ops_render_step(ctx) || !sift_down(ctx, 0, end - 1))
            goto finish;
    }

finish:
    ctx->active_index = -1;
    ctx->compared_index = -1;
}
