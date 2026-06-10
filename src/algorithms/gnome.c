#include "algorithms/sort.h"
#include "core/operations.h"

void sort_gnome(SortContext *ctx)
{
    if (!ctx || !ctx->values || ctx->length < 2)
        return;

    context_reset_stats(ctx);
    size_t index = 1;

    while (index < ctx->length) {
        int comparison = ops_compare(ctx, index - 1, index);
        if (!ops_render_step(ctx))
            goto finish;
        if (comparison <= 0) {
            index++;
        } else {
            ops_swap(ctx, index - 1, index);
            if (!ops_render_step(ctx))
                goto finish;
            if (index > 1)
                index--;
            else
                index = 1;
        }
    }

finish:
    ctx->active_index = -1;
    ctx->compared_index = -1;
}
