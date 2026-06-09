#ifndef CONTEXT_H
#define CONTEXT_H

#include <stddef.h>
#include <stdbool.h>

typedef struct SortContext SortContext;

typedef void (*RenderFn)(const SortContext *ctx);

struct SortContext {
    int          *values;
    size_t        length;
    size_t        comparisons;
    size_t        swaps;
    int           active_index;
    int           compared_index;
    unsigned int  delay_ms;
    bool          paused;
    bool          interrupted;
    RenderFn      render_fn;
    const char   *algo_name;
};

SortContext *context_create(size_t length);
void         context_destroy(SortContext *ctx);
void         context_randomize(SortContext *ctx);
void         context_reset_stats(SortContext *ctx);
bool         context_set_values(SortContext *ctx, const int *values, size_t length);

#endif
