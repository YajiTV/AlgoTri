#include "algorithms/sort.h"
#include "core/operations.h"

void sort_selection(SortContext *ctx)
{
    if (!ctx || !ctx->values || ctx->length < 2)
        return;

    context_reset_stats(ctx);
    for (size_t i = 0; i + 1 < ctx->length; i++) {
        size_t minimum = i;

        for (size_t j = i + 1; j < ctx->length; j++) {
            int comparison = ops_compare(ctx, j, minimum);
            ops_render_step(ctx);
            if (comparison < 0)
                minimum = j;
        }
        if (minimum != i) {
            ops_swap(ctx, i, minimum);
            ops_render_step(ctx);
        }
    }
    ctx->active_index = -1;
    ctx->compared_index = -1;
}
