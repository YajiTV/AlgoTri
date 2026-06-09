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

typedef struct {
    const char  *label;
    unsigned int delay_ms;
} SpeedEntry;

static const SpeedEntry SPEEDS[] = {
    {"Lente",   120},
    {"Normale", 45},
    {"Rapide",  10},
};

#define SIZE_COUNT ((int)(sizeof(SIZES) / sizeof(SIZES[0])))
#define SPEED_COUNT ((int)(sizeof(SPEEDS) / sizeof(SPEEDS[0])))
#define MENU_ITEM_COUNT 4
#define MENU_MIN_COLS 50
#define MENU_MIN_ROWS 18

typedef struct {
    int selected_item;
    int algorithm_index;
    int size_index;
    int speed_index;
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

static void draw_action_button(int row, int cols, bool selected, const char *label)
{
    int width = 38;
    int x = (cols - width) / 2;
    int label_x = x + (width - (int)strlen(label)) / 2;

    if (x < 0)
        x = 0;
    if (label_x < 0)
        label_x = 0;

    if (selected) {
        attron(A_REVERSE);
        mvhline(row, x, ' ', width);
    }
    mvprintw(row, label_x, "%s", label);
    if (selected)
        attroff(A_REVERSE);
}

static int centered_x(int cols, const char *text)
{
    int x = (cols - (int)strlen(text)) / 2;
    return x > 0 ? x : 0;
}

static void draw_main_menu(const MenuState *state, int rows, int cols)
{
    char size_label[16];

    erase();
    if (rows < MENU_MIN_ROWS || cols < MENU_MIN_COLS) {
        const char *message = "Agrandissez le terminal.";
        mvprintw(rows / 2, centered_x(cols, message), "%s", message);
        refresh();
        return;
    }

    attron(COLOR_PAIR(COLOR_PAIR_BORDER) | A_BOLD);
    const char *title = "AlgoTri";
    mvprintw(2, centered_x(cols, title), "%s", title);
    attroff(COLOR_PAIR(COLOR_PAIR_BORDER) | A_BOLD);

    const char *subtitle = "Configurez votre tri";
    mvprintw(4, centered_x(cols, subtitle), "%s", subtitle);

    snprintf(size_label, sizeof(size_label), "%zu valeurs", SIZES[state->size_index]);
    draw_menu_item(7, cols, state->selected_item == 0, "Algorithme",
                   ALGOS[state->algorithm_index].label);
    draw_menu_item(9, cols, state->selected_item == 1, "Taille", size_label);
    draw_menu_item(11, cols, state->selected_item == 2, "Vitesse",
                   SPEEDS[state->speed_index].label);
    draw_action_button(14, cols, state->selected_item == 3, "Lancer le tri");

    const char *hint = "Flèches : choisir   Entrée : valider   Q : quitter";
    mvprintw(rows - 2, centered_x(cols, hint), "%s", hint);

    refresh();
}

static bool handle_sort_controls(SortContext *ctx)
{
    int ch = getch();

    if (ch == 'q' || ch == 'Q') {
        ctx->interrupted = true;
        ctx->paused = false;
        return false;
    }
    if (ch == ' ' || ch == 'p' || ch == 'P')
        ctx->paused = !ctx->paused;

    while (ctx->paused && !ctx->interrupted) {
        renderer_draw(ctx);
        ch = getch();
        if (ch == ' ' || ch == 'p' || ch == 'P')
            ctx->paused = false;
        else if (ch == 'q' || ch == 'Q') {
            ctx->interrupted = true;
            ctx->paused = false;
        }
        napms(30);
    }
    return !ctx->interrupted;
}

static void run_sort(SortContext *ctx, const MenuState *state)
{
    ctx->algo_name = ALGOS[state->algorithm_index].label;
    ctx->delay_ms = SPEEDS[state->speed_index].delay_ms;
    ctx->render_fn = renderer_draw;
    ctx->control_fn = handle_sort_controls;

    bool restart = true;
    while (restart) {
        restart = false;
        if (!context_resize(ctx, SIZES[state->size_index]))
            return;

        ALGOS[state->algorithm_index].fn(ctx);

        if (ctx->interrupted)
            return;

        ctx->active_index   = -1;
        ctx->compared_index = -1;
        renderer_wave(ctx);

        nodelay(stdscr, FALSE);
        const char *done_msg = "Tri terminé   R : encore   Entrée : menu";
        mvprintw(LINES - 1, centered_x(COLS, done_msg), "%s", done_msg);
        refresh();
        int ch = getch();
        restart = ch == 'r' || ch == 'R';
        nodelay(stdscr, TRUE);
    }
}

void menu_run(SortContext *ctx)
{
    renderer_init();

    MenuState state = {
        .selected_item = 0,
        .algorithm_index = 0,
        .size_index = 1,
        .speed_index = 1
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
                else if (state.selected_item == 2)
                    state.speed_index =
                        (state.speed_index - 1 + SPEED_COUNT) % SPEED_COUNT;
                break;
            case KEY_RIGHT:
                if (state.selected_item == 0)
                    state.algorithm_index = (state.algorithm_index + 1) % ALGO_COUNT;
                else if (state.selected_item == 1)
                    state.size_index = (state.size_index + 1) % SIZE_COUNT;
                else if (state.selected_item == 2)
                    state.speed_index = (state.speed_index + 1) % SPEED_COUNT;
                break;
            case '\n': case KEY_ENTER:
                if (state.selected_item == 3)
                    run_sort(ctx, &state);
                else
                    state.selected_item++;
                break;
            case 'q': case 'Q':
                running = false;
                break;
            default:
                break;
        }
        napms(16);
    }

    renderer_cleanup();
}
