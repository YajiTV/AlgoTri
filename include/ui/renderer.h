#ifndef RENDERER_H
#define RENDERER_H

#include "core/context.h"

void renderer_init(void);
void renderer_cleanup(void);
void renderer_draw(const SortContext *ctx, const char *algo_name);

#endif
