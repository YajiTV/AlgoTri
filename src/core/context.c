#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "core/context.h"

static void seed_random_once(void)
{
    static bool seeded = false;

    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = true;
    }
}

SortContext *context_create(size_t length)
{
    if (length == 0 || length > SIZE_MAX / sizeof(int))
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
    ctx->operation      = OPERATION_NONE;
    ctx->delay_ms       = 30;
    ctx->paused         = false;
    ctx->interrupted    = false;
    ctx->render_fn      = NULL;
    ctx->control_fn     = NULL;
    ctx->algo_name      = NULL;

    seed_random_once();
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
        ctx->values[i] = (int)(i + 1);

    for (size_t i = ctx->length - 1; i > 0; i--) {
        size_t j = (size_t)rand() % (i + 1);
        int tmp        = ctx->values[i];
        ctx->values[i] = ctx->values[j];
        ctx->values[j] = tmp;
    }

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
    ctx->operation      = OPERATION_NONE;
    ctx->paused         = false;
    ctx->interrupted    = false;
}

bool context_set_values(SortContext *ctx, const int *values, size_t length)
{
    if (!ctx || !values || length == 0 || length > SIZE_MAX / sizeof(int))
        return false;

    int *new_values = malloc(length * sizeof(int));
    if (!new_values)
        return false;

    memcpy(new_values, values, length * sizeof(int));
    free(ctx->values);
    ctx->values = new_values;
    ctx->length = length;
    context_reset_stats(ctx);
    return true;
}

bool context_resize(SortContext *ctx, size_t length)
{
    if (!ctx || length == 0 || length > SIZE_MAX / sizeof(int))
        return false;

    int *new_values = realloc(ctx->values, length * sizeof(int));
    if (!new_values)
        return false;

    ctx->values = new_values;
    ctx->length = length;
    context_randomize(ctx);
    return true;
}
