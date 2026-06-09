#include "algorithms/sort.h"
#include "core/operations.h"

void sort_bubble(SortContext *ctx)
{
    if (!ctx || !ctx->values || ctx->length < 2)
        return;

    context_reset_stats(ctx);
    for (size_t end = ctx->length; end > 1; end--) {
        bool swapped = false;

        for (size_t i = 0; i + 1 < end; i++) {
            int comparison = ops_compare(ctx, i, i + 1);
            if (!ops_render_step(ctx))
                goto finish;
            if (comparison > 0) {
                ops_swap(ctx, i, i + 1);
                if (!ops_render_step(ctx))
                    goto finish;
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
finish:
    ctx->active_index = -1;
    ctx->compared_index = -1;
}
