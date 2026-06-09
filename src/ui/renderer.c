#include <ncurses.h>
#include <string.h>

#include "ui/renderer.h"

void renderer_init(void)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_pair(COLOR_PAIR_BORDER,   COLOR_CYAN,   COLOR_BLACK);
        init_pair(COLOR_PAIR_NORMAL,   COLOR_WHITE,  COLOR_BLACK);
        init_pair(COLOR_PAIR_COMPARED, COLOR_YELLOW, COLOR_BLACK);
        init_pair(COLOR_PAIR_SWAPPED,  COLOR_RED,    COLOR_BLACK);
        init_pair(COLOR_PAIR_SORTED,   COLOR_GREEN,  COLOR_BLACK);
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
        attron(COLOR_PAIR(color));

        int x = i * bar_w;
        for (int r = bar_top; r <= bar_bot; r++) {
            int dist_from_bottom = bar_bot - r + 1;
            for (int w = 0; w < bar_w && x + w < cols; w++) {
                if (dist_from_bottom <= filled)
                    mvaddch(r, x + w, ACS_BLOCK);
                else
                    mvaddch(r, x + w, ' ');
            }
        }
        attroff(COLOR_PAIR(color));
    }
}

static void draw_stats(const SortContext *ctx, int row, int cols)
{
    attron(COLOR_PAIR(COLOR_PAIR_BORDER));
    mvhline(row, 0, ACS_HLINE, cols);
    attroff(COLOR_PAIR(COLOR_PAIR_BORDER));

    mvprintw(row + 1, 2, "Comparaisons : %-6zu   Echanges : %-6zu",
             ctx->comparisons, ctx->swaps);
}

static void draw_controls(int row, int cols)
{
    const char *hint = "Q : quitter";
    int x = (cols - (int)strlen(hint)) / 2;
    if (x < 0) x = 0;

    attron(A_DIM);
    mvprintw(row, x, "%s", hint);
    attroff(A_DIM);
}

void renderer_draw(const SortContext *ctx)
{
    if (!ctx)
        return;

    int rows = LINES;
    int cols = COLS;

    if (rows < RENDERER_MIN_ROWS || cols < RENDERER_MIN_COLS) {
        clear();
        const char *msg = "Agrandissez le terminal.";
        mvprintw(rows / 2, (cols - (int)strlen(msg)) / 2, "%s", msg);
        refresh();
        return;
    }

    clear();

    draw_title(ctx, cols);

    int bar_top  = 2;
    int bar_bot  = rows - 5;
    draw_bars(ctx, bar_top, bar_bot, cols);

    draw_stats(ctx, rows - 4, cols);
    draw_controls(rows - 1, cols);

    refresh();
}
