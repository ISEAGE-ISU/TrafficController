#include <menu.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define MENU_WIDTH 50
#define MENU_HEIGHT 18

char *menu_choices[] = {
                        "Reboot Device",
                        "Upload Traffic Light Programming",
                        "Choice 3",
                        "Choice 4",
                        "Choice 5",
                        "Choice 6",
                        "Choice 7",
                        "Exit                                   ",
                  };
void init() {
    initscr();
    start_color();
    cbreak();
    curs_set(0);
    noecho();
    keypad(stdscr, TRUE);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
}

void build_menu(ITEM ***items, char **menu_choices, int menu_len, MENU **menu) {
    *items = (ITEM **)malloc(menu_len * sizeof(ITEM *));
    bzero(*items, menu_len * sizeof(ITEM*));

    for(int i = 0; i < menu_len; ++i)
          (*items)[i] = new_item(menu_choices[i], "");
    (*items)[menu_len + 1] = (ITEM *)NULL;

    *menu = new_menu((ITEM **)*items);

    set_menu_fore(*menu, COLOR_PAIR(1) | A_REVERSE);
    set_menu_back(*menu, COLOR_PAIR(2));
    set_menu_grey(*menu, COLOR_PAIR(3));
}

int main()
{
    ITEM **items;
    int c;
    MENU *menu;
    int max_row, max_col;
    WINDOW *main_win;

    int menu_len = ARRAY_SIZE(menu_choices);
    init();

    build_menu(&items, menu_choices, menu_len, &menu);

    getmaxyx(stdscr, max_row, max_col);

    main_win = newwin(MENU_HEIGHT, MENU_WIDTH, ((max_row - MENU_HEIGHT)/2) - 1, (max_col - MENU_WIDTH)/ 2);
    keypad(main_win, TRUE);
    set_menu_win(menu, main_win);
    set_menu_sub(menu, derwin(main_win, MENU_HEIGHT - 5, MENU_WIDTH -2, 5, 2));
    set_menu_mark(menu, " > ");
    box(main_win, 0, 0);

    mvprintw(max_row - 1, 0, "Press <ENTER> to select");

    wattron(main_win, COLOR_PAIR(1));
    mvwprintw(main_win, 1, (MENU_WIDTH - strlen("City of Ames Traffic"))/2, "%s", "City of Ames Traffic");
    wattroff(main_win, COLOR_PAIR(1));

    mvwaddch(main_win, 2, 0, ACS_LTEE);
    mvwhline(main_win, 2, 1, ACS_HLINE, MENU_WIDTH - 1);
    mvwaddch(main_win, 2, MENU_WIDTH - 1, ACS_RTEE);

    refresh();
    post_menu(menu);
    wrefresh(main_win);

    while((c = wgetch(main_win)) != KEY_F(1)) {
        switch(c) {
              case 'q':
                endwin();
                return 0;
                break;
              case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
              case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
              case 10: /* Enter */
                move(LINES - 2, 0);
                clrtoeol();
                mvprintw(LINES - 2, 0, "Item selected is : %s", item_name(current_item(menu)));
                break;
      }
      refresh();
      wrefresh(main_win);
    }

    unpost_menu(menu);
    for(int i = 0; i < ARRAY_SIZE(menu_choices); ++i)
        free_item(items[i]);
    free_menu(menu);
    endwin();
}
