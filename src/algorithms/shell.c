#include "algorithms/sort.h"
#include "core/operations.h"

void sort_shell(SortContext *ctx)
{
    if (!ctx || !ctx->values || ctx->length < 2)
        return;

    context_reset_stats(ctx);
    for (size_t gap = ctx->length / 2; gap > 0; gap /= 2) {
        for (size_t i = gap; i < ctx->length; i++) {
            size_t current = i;

            while (current >= gap) {
                int comparison = ops_compare(ctx, current - gap, current);
                if (!ops_render_step(ctx))
                    goto finish;
                if (comparison <= 0)
                    break;
                ops_swap(ctx, current - gap, current);
                if (!ops_render_step(ctx))
                    goto finish;
                current -= gap;
            }
        }
    }

finish:
    ctx->active_index = -1;
    ctx->compared_index = -1;
}
