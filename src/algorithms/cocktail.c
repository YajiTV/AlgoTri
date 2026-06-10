#include "algorithms/sort.h"
#include "core/operations.h"

void sort_cocktail(SortContext *ctx)
{
    if (!ctx || !ctx->values || ctx->length < 2)
        return;

    context_reset_stats(ctx);
    size_t start = 0;
    size_t end = ctx->length - 1;
    bool swapped = true;

    while (swapped && start < end) {
        swapped = false;
        for (size_t i = start; i < end; i++) {
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

        swapped = false;
        end--;
        for (size_t i = end; i > start; i--) {
            int comparison = ops_compare(ctx, i - 1, i);
            if (!ops_render_step(ctx))
                goto finish;
            if (comparison > 0) {
                ops_swap(ctx, i - 1, i);
                if (!ops_render_step(ctx))
                    goto finish;
                swapped = true;
            }
        }
        start++;
    }

finish:
    ctx->active_index = -1;
    ctx->compared_index = -1;
}
