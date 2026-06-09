#include "algorithms/sort.h"
#include "core/operations.h"

void sort_insertion(SortContext *ctx)
{
    if (!ctx || !ctx->values || ctx->length < 2)
        return;

    context_reset_stats(ctx);
    for (size_t i = 1; i < ctx->length; i++) {
        size_t current = i;

        while (current > 0) {
            int comparison = ops_compare(ctx, current - 1, current);
            ops_render_step(ctx);
            if (comparison <= 0)
                break;
            ops_swap(ctx, current - 1, current);
            ops_render_step(ctx);
            current--;
        }
    }
    ctx->active_index = -1;
    ctx->compared_index = -1;
}
