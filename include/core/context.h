#ifndef CONTEXT_H
#define CONTEXT_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    int          *values;
    size_t        length;
    size_t        comparisons;
    size_t        swaps;
    int           active_index;
    int           compared_index;
    unsigned int  delay_ms;
    bool          paused;
} SortContext;

SortContext *context_create(size_t length);
void         context_destroy(SortContext *ctx);
void         context_randomize(SortContext *ctx);
void         context_reset_stats(SortContext *ctx);

#endif
