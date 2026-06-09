#include <locale.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>

#include "ui/renderer.h"

void renderer_init(void)
{
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        use_default_colors();
        init_pair(COLOR_PAIR_BORDER,   COLOR_CYAN,   COLOR_BLACK);
        init_pair(COLOR_PAIR_NORMAL,   COLOR_BLACK,  COLOR_WHITE);
        init_pair(COLOR_PAIR_COMPARED, COLOR_BLACK,  COLOR_YELLOW);
        init_pair(COLOR_PAIR_SWAPPED,  COLOR_BLACK,  COLOR_RED);
        init_pair(COLOR_PAIR_SORTED,   COLOR_BLACK,  COLOR_GREEN);
    }
}

void renderer_cleanup(void)
{
    curs_set(1);
    endwin();
}

static int bar_color(const SortContext *ctx, int i)
{
    if (ctx->active_index == -1 && ctx->compared_index == -1)
        return COLOR_PAIR_SORTED;
    if (i == ctx->active_index)
        return COLOR_PAIR_SWAPPED;
    if (i == ctx->compared_index)
        return COLOR_PAIR_COMPARED;
    return COLOR_PAIR_NORMAL;
}

static void draw_title(const SortContext *ctx, int cols)
{
    const char *name = ctx->algo_name ? ctx->algo_name : "AlgoTri";
    int title_len = (int)strlen(name);
    int x = (cols - title_len) / 2;
    if (x < 0) x = 0;

    attron(COLOR_PAIR(COLOR_PAIR_BORDER) | A_BOLD);
    mvhline(0, 0, ACS_HLINE, cols);
    mvprintw(0, x, " %s ", name);
    attroff(COLOR_PAIR(COLOR_PAIR_BORDER) | A_BOLD);
}

static void draw_bars(const SortContext *ctx, int bar_top, int bar_bot, int cols)
{
    if (ctx->length == 0 || bar_bot <= bar_top)
        return;

    int bar_h = bar_bot - bar_top + 1;
    int bar_w = cols / (int)ctx->length;
    if (bar_w < 1) bar_w = 1;

    for (int i = 0; i < (int)ctx->length; i++) {
        int val    = ctx->values[i];
        int filled = val * bar_h / (int)ctx->length;
        if (filled == 0 && val > 0)
            filled = 1;

        int color = bar_color(ctx, i);
        int x = i * bar_w;

        for (int r = bar_top; r <= bar_bot; r++) {
            int dist_from_bottom = bar_bot - r + 1;
            int pair = (dist_from_bottom <= filled) ? COLOR_PAIR(color) : 0;
            for (int w = 0; w < bar_w && x + w < cols; w++)
                mvaddch(r, x + w, ' ' | pair);
        }
    }
}

static void draw_stats(const SortContext *ctx, int row, int cols)
{
    attron(COLOR_PAIR(COLOR_PAIR_BORDER));
    mvhline(row, 0, ACS_HLINE, cols);
    attroff(COLOR_PAIR(COLOR_PAIR_BORDER));

    mvprintw(row + 1, 2, "Comparaisons : %-6zu   Échanges : %-6zu",
             ctx->comparisons, ctx->swaps);
}

static void draw_explanation(const SortContext *ctx, int row, int cols)
{
    char message[96];

    if (!ctx->paused || ctx->active_index < 0 || ctx->compared_index < 0)
        return;

    int left = ctx->values[ctx->active_index];
    int right = ctx->values[ctx->compared_index];
    if (ctx->operation == OPERATION_COMPARE) {
        if (left == right)
            snprintf(message, sizeof(message), "Comparaison : %d et %d sont égales.",
                     left, right);
        else
            snprintf(message, sizeof(message), "Comparaison : %d est %s que %d.",
                     left, left < right ? "plus petite" : "plus grande", right);
    } else if (ctx->operation == OPERATION_SWAP) {
        snprintf(message, sizeof(message), "Échange : %d et %d changent de place.",
                 left, right);
    } else {
        return;
    }

    int x = (cols - (int)strlen(message)) / 2;
    mvprintw(row, x > 0 ? x : 0, "%s", message);
}

static void draw_controls(const SortContext *ctx, int row, int cols)
{
    const char *hint = ctx->paused
        ? "Pause   N / → : étape   Espace : reprendre   Q : menu"
        : "Espace : pause   Q : menu";
    int x = (cols - (int)strlen(hint)) / 2;
    if (x < 0) x = 0;

    attron(A_DIM);
    mvprintw(row, x, "%s", hint);
    attroff(A_DIM);
}

static void draw_one_bar_green(int index, int val, int length, int bar_top,
                               int bar_bot, int bar_w, int cols)
{
    int bar_h  = bar_bot - bar_top + 1;
    int filled = val * bar_h / length;
    if (filled == 0 && val > 0)
        filled = 1;

    int x = index * bar_w;
    for (int r = bar_top; r <= bar_bot; r++) {
        int dist_from_bottom = bar_bot - r + 1;
        int pair = (dist_from_bottom <= filled) ? COLOR_PAIR(COLOR_PAIR_SORTED) : 0;
        for (int w = 0; w < bar_w && x + w < cols; w++)
            mvaddch(r, x + w, ' ' | pair);
    }
}

void renderer_wave(const SortContext *ctx)
{
    if (!ctx || ctx->length == 0)
        return;

    int rows = LINES;
    int cols = COLS;

    if (rows < RENDERER_MIN_ROWS || cols < RENDERER_MIN_COLS)
        return;

    int bar_top = 2;
    int bar_bot = rows - 6;
    int bar_w   = cols / (int)ctx->length;
    if (bar_w < 1) bar_w = 1;

    long delay_ns = 600000000L / (long)ctx->length;
    struct timespec ts = { .tv_sec = 0, .tv_nsec = delay_ns };

    for (size_t k = 0; k < ctx->length; k++) {
        draw_one_bar_green((int)k, ctx->values[k], (int)ctx->length,
                           bar_top, bar_bot, bar_w, cols);
        refresh();
        nanosleep(&ts, NULL);
    }
}

void renderer_draw(const SortContext *ctx)
{
    if (!ctx)
        return;

    int rows = LINES;
    int cols = COLS;

    if (rows < RENDERER_MIN_ROWS || cols < RENDERER_MIN_COLS) {
        erase();
        const char *msg = "Agrandissez le terminal.";
        int x = (cols - (int)strlen(msg)) / 2;
        mvprintw(rows / 2, x > 0 ? x : 0, "%s", msg);
        refresh();
        return;
    }

    erase();

    draw_title(ctx, cols);

    int bar_top = 2;
    int bar_bot = rows - 6;
    draw_bars(ctx, bar_top, bar_bot, cols);

    draw_explanation(ctx, rows - 5, cols);
    draw_stats(ctx, rows - 4, cols);
    draw_controls(ctx, rows - 1, cols);

    refresh();
}
