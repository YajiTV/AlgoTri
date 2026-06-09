#include <stdio.h>
#include <stdlib.h>

#include "core/context.h"
#include "ui/menu.h"

int main(void)
{
    SortContext *ctx = context_create(20);
    if (!ctx) {
        fprintf(stderr, "Erreur : impossible d'allouer le contexte.\n");
        return EXIT_FAILURE;
    }

    menu_run(ctx);

    context_destroy(ctx);
    return EXIT_SUCCESS;
}
