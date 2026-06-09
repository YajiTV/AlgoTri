#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "ui/menu.h"
#include "ui/renderer.h"
#include "algorithms/sort.h"
#include "core/context.h"

typedef struct {
    const char *label;
    void      (*fn)(SortContext *);
} AlgoEntry;

static const AlgoEntry ALGOS[] = {
    {"Tri à bulles",      sort_bubble},
    {"Tri par sélection", sort_selection},
    {"Tri par insertion", sort_insertion},
    {"Tri rapide",        sort_quick},
};

#define ALGO_COUNT ((int)(sizeof(ALGOS) / sizeof(ALGOS[0])))

static void draw_main_menu(int selected, int rows, int cols)
{
    erase();

    attron(COLOR_PAIR(COLOR_PAIR_BORDER) | A_BOLD);
    const char *title = "=== AlgoTri ===";
    mvprintw(2, (cols - (int)strlen(title)) / 2, "%s", title);
    attroff(COLOR_PAIR(COLOR_PAIR_BORDER) | A_BOLD);

    mvprintw(4, (cols - 24) / 2, "Choisissez un algorithme");

    for (int i = 0; i < ALGO_COUNT; i++) {
        int row = 6 + i * 2;
        int x   = (cols - 30) / 2;
        if (i == selected) {
            attron(A_REVERSE);
            mvprintw(row, x, "  %d. %-26s", i + 1, ALGOS[i].label);
            attroff(A_REVERSE);
        } else {
            mvprintw(row, x, "  %d. %-26s", i + 1, ALGOS[i].label);
        }
    }

    const char *hint = "Fleches : naviguer   Entree : lancer   Q : quitter";
    mvprintw(rows - 2, (cols - (int)strlen(hint)) / 2, "%s", hint);

    refresh();
}

static void run_sort(SortContext *ctx, int algo_index)
{
    ctx->algo_name = ALGOS[algo_index].label;
    ctx->render_fn = renderer_draw;
    context_randomize(ctx);

    ALGOS[algo_index].fn(ctx);

    ctx->active_index   = -1;
    ctx->compared_index = -1;
    renderer_draw(ctx);

    nodelay(stdscr, FALSE);
    const char *done_msg = "Tri terminé — appuyez sur une touche";
    mvprintw(LINES - 1, (COLS - (int)strlen(done_msg)) / 2, "%s", done_msg);
    refresh();
    getch();
    nodelay(stdscr, TRUE);
}

void menu_run(SortContext *ctx)
{
    renderer_init();

    int selected = 0;
    bool running = true;

    while (running) {
        draw_main_menu(selected, LINES, COLS);

        int ch = getch();
        switch (ch) {
            case KEY_UP:
                selected = (selected - 1 + ALGO_COUNT) % ALGO_COUNT;
                break;
            case KEY_DOWN:
                selected = (selected + 1) % ALGO_COUNT;
                break;
            case '1': case '2': case '3': case '4':
                selected = ch - '1';
                run_sort(ctx, selected);
                break;
            case '\n': case KEY_ENTER:
                run_sort(ctx, selected);
                break;
            case 'q': case 'Q':
                running = false;
                break;
            default:
                break;
        }
    }

    renderer_cleanup();
}
