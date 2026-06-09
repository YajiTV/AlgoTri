#include "algorithms/sort.h"
#include "core/operations.h"

void sort_comb(SortContext *ctx)
{
    if (!ctx || !ctx->values || ctx->length < 2)
        return;

    context_reset_stats(ctx);
    size_t gap = ctx->length;
    bool swapped = true;

    while (gap > 1 || swapped) {
        gap = gap * 10 / 13;
        if (gap < 1)
            gap = 1;
        swapped = false;

        for (size_t i = 0; i + gap < ctx->length; i++) {
            int comparison = ops_compare(ctx, i, i + gap);
            if (!ops_render_step(ctx))
                goto finish;
            if (comparison > 0) {
                ops_swap(ctx, i, i + gap);
                if (!ops_render_step(ctx))
                    goto finish;
                swapped = true;
            }
        }
    }

finish:
    ctx->active_index = -1;
    ctx->compared_index = -1;
}
