#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "context.h"

int  ops_compare(SortContext *ctx, size_t i, size_t j);
void ops_swap(SortContext *ctx, size_t i, size_t j);
bool ops_render_step(SortContext *ctx);

#endif
