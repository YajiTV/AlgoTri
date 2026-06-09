#include <time.h>

#include "core/operations.h"

int ops_compare(SortContext *ctx, size_t i, size_t j)
{
    ctx->comparisons++;
    ctx->compared_index = (int)j;
    ctx->active_index   = (int)i;
    return ctx->values[i] - ctx->values[j];
}

void ops_swap(SortContext *ctx, size_t i, size_t j)
{
    ctx->swaps++;
    int tmp         = ctx->values[i];
    ctx->values[i]  = ctx->values[j];
    ctx->values[j]  = tmp;
}

void ops_render_step(SortContext *ctx)
{
    struct timespec ts = {
        .tv_sec  = ctx->delay_ms / 1000,
        .tv_nsec = (long)(ctx->delay_ms % 1000) * 1000000L
    };
    nanosleep(&ts, NULL);
}
