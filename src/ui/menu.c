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

static const size_t SIZES[] = {10, 20, 30, 40};

#define SIZE_COUNT ((int)(sizeof(SIZES) / sizeof(SIZES[0])))
#define MENU_ITEM_COUNT 3

typedef struct {
    int selected_item;
    int algorithm_index;
    int size_index;
} MenuState;

static void draw_menu_item(int row, int cols, bool selected, const char *label,
                           const char *value)
{
    int x = (cols - 38) / 2;
    if (x < 0)
        x = 0;

    if (selected)
        attron(A_REVERSE);
    mvprintw(row, x, "  %-15s %-17s  ", label, value);
    if (selected)
        attroff(A_REVERSE);
}

static void draw_main_menu(const MenuState *state, int rows, int cols)
{
    char size_label[16];

    erase();

    attron(COLOR_PAIR(COLOR_PAIR_BORDER) | A_BOLD);
    const char *title = "=== AlgoTri ===";
    mvprintw(2, (cols - (int)strlen(title)) / 2, "%s", title);
    attroff(COLOR_PAIR(COLOR_PAIR_BORDER) | A_BOLD);

    mvprintw(4, (cols - 20) / 2, "Configurez votre tri");

    snprintf(size_label, sizeof(size_label), "%zu valeurs", SIZES[state->size_index]);
    draw_menu_item(7, cols, state->selected_item == 0, "Algorithme",
                   ALGOS[state->algorithm_index].label);
    draw_menu_item(9, cols, state->selected_item == 1, "Taille", size_label);
    draw_menu_item(12, cols, state->selected_item == 2, "", "Lancer le tri");

    const char *hint = "Fleches : choisir   Entree : valider   Q : quitter";
    mvprintw(rows - 2, (cols - (int)strlen(hint)) / 2, "%s", hint);

    refresh();
}

static void run_sort(SortContext *ctx, const MenuState *state)
{
    if (!context_resize(ctx, SIZES[state->size_index]))
        return;

    ctx->algo_name = ALGOS[state->algorithm_index].label;
    ctx->render_fn = renderer_draw;

    ALGOS[state->algorithm_index].fn(ctx);

    ctx->active_index   = -1;
    ctx->compared_index = -1;
    renderer_wave(ctx);

    nodelay(stdscr, FALSE);
    const char *done_msg = "Tri termine - appuyez sur une touche";
    mvprintw(LINES - 1, (COLS - (int)strlen(done_msg)) / 2, "%s", done_msg);
    refresh();
    getch();
    nodelay(stdscr, TRUE);
}

void menu_run(SortContext *ctx)
{
    renderer_init();

    MenuState state = {
        .selected_item = 0,
        .algorithm_index = 0,
        .size_index = 1
    };
    bool running = true;

    while (running) {
        draw_main_menu(&state, LINES, COLS);

        int ch = getch();
        switch (ch) {
            case KEY_UP:
                state.selected_item =
                    (state.selected_item - 1 + MENU_ITEM_COUNT) % MENU_ITEM_COUNT;
                break;
            case KEY_DOWN:
                state.selected_item = (state.selected_item + 1) % MENU_ITEM_COUNT;
                break;
            case KEY_LEFT:
                if (state.selected_item == 0)
                    state.algorithm_index =
                        (state.algorithm_index - 1 + ALGO_COUNT) % ALGO_COUNT;
                else if (state.selected_item == 1)
                    state.size_index = (state.size_index - 1 + SIZE_COUNT) % SIZE_COUNT;
                break;
            case KEY_RIGHT:
                if (state.selected_item == 0)
                    state.algorithm_index = (state.algorithm_index + 1) % ALGO_COUNT;
                else if (state.selected_item == 1)
                    state.size_index = (state.size_index + 1) % SIZE_COUNT;
                break;
            case '\n': case KEY_ENTER:
                if (state.selected_item == 2)
                    run_sort(ctx, &state);
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
