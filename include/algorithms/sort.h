#ifndef SORT_H
#define SORT_H

#include "core/context.h"

void sort_bubble(SortContext *ctx);
void sort_selection(SortContext *ctx);
void sort_insertion(SortContext *ctx);
void sort_quick(SortContext *ctx);
void sort_merge(SortContext *ctx);
void sort_heap(SortContext *ctx);
void sort_shell(SortContext *ctx);
void sort_cocktail(SortContext *ctx);
void sort_comb(SortContext *ctx);
void sort_gnome(SortContext *ctx);

#endif
