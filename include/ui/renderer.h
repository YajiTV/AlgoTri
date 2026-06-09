#ifndef RENDERER_H
#define RENDERER_H

#include "core/context.h"

#define COLOR_PAIR_BORDER   1
#define COLOR_PAIR_NORMAL   2
#define COLOR_PAIR_COMPARED 3
#define COLOR_PAIR_SWAPPED  4
#define COLOR_PAIR_SORTED   5

#define RENDERER_MIN_COLS 40
#define RENDERER_MIN_ROWS 15

void renderer_init(void);
void renderer_cleanup(void);
void renderer_draw(const SortContext *ctx);

#endif
