#include <time.h>

#include "core/operations.h"

int ops_compare(SortContext *ctx, size_t i, size_t j)
{
    if (!ctx || !ctx->values || i >= ctx->length || j >= ctx->length)
        return 0;

    ctx->comparisons++;
    ctx->compared_index = (int)j;
    ctx->active_index   = (int)i;
    ctx->operation      = OPERATION_COMPARE;
    if (ctx->values[i] < ctx->values[j])
        return -1;
    if (ctx->values[i] > ctx->values[j])
        return 1;
    return 0;
}

void ops_swap(SortContext *ctx, size_t i, size_t j)
{
    if (!ctx || !ctx->values || i >= ctx->length || j >= ctx->length || i == j)
        return;

    ctx->swaps++;
    ctx->active_index   = (int)i;
    ctx->compared_index = (int)j;
    ctx->operation      = OPERATION_SWAP;
    int tmp         = ctx->values[i];
    ctx->values[i]  = ctx->values[j];
    ctx->values[j]  = tmp;
}

bool ops_render_step(SortContext *ctx)
{
    if (!ctx)
        return false;

    if (ctx->control_fn && !ctx->control_fn(ctx))
        return false;

    if (ctx->render_fn)
        ctx->render_fn(ctx);

    if (ctx->delay_ms == 0)
        return !ctx->interrupted;

    struct timespec ts = {
        .tv_sec  = ctx->delay_ms / 1000,
        .tv_nsec = (long)(ctx->delay_ms % 1000) * 1000000L
    };
    nanosleep(&ts, NULL);
    return !ctx->interrupted;
}
