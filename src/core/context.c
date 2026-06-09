#include <stdlib.h>
#include <time.h>

#include "core/context.h"

SortContext *context_create(size_t length)
{
    if (length == 0)
        return NULL;

    SortContext *ctx = malloc(sizeof(SortContext));
    if (!ctx)
        return NULL;

    ctx->values = malloc(length * sizeof(int));
    if (!ctx->values) {
        free(ctx);
        return NULL;
    }

    ctx->length         = length;
    ctx->comparisons    = 0;
    ctx->swaps          = 0;
    ctx->active_index   = -1;
    ctx->compared_index = -1;
    ctx->delay_ms       = 30;
    ctx->paused         = false;

    srand((unsigned int)time(NULL));
    context_randomize(ctx);
    return ctx;
}

void context_destroy(SortContext *ctx)
{
    if (!ctx)
        return;
    free(ctx->values);
    free(ctx);
}

void context_randomize(SortContext *ctx)
{
    if (!ctx)
        return;
    for (size_t i = 0; i < ctx->length; i++)
        ctx->values[i] = (int)(rand() % (int)ctx->length) + 1;
    context_reset_stats(ctx);
}

void context_reset_stats(SortContext *ctx)
{
    if (!ctx)
        return;
    ctx->comparisons    = 0;
    ctx->swaps          = 0;
    ctx->active_index   = -1;
    ctx->compared_index = -1;
    ctx->paused         = false;
}
