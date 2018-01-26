#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

/* our cat */
const char nyan_cat[5][13] = {
    ",------,   \n",
    "|   /\\_/\\\n",
    "|__( ^ .^) \n",
    "\"\"  \"\" \n"};

/*
 *  draw_cat_step - draw one frame of the cat crossing the screen,
 *  based on x and y coordinates
 */
void draw_cat_step(int x, int y, char *tail) {
    int i = 0, w=0;

    /* flesh out our tail */
    for (i; i<x; i++) {
        *(tail+i) = '=';
    }

    /* draw the cat and tail lines */
    for(w=0; w<4; w++) {
        attron(COLOR_PAIR(w+1));
        mvprintw(y+w, 0, tail);
        attron(COLOR_PAIR(5));
        mvprintw(y+w, x, nyan_cat[w]);
    }
}

int main(int argc, char **argv) {
    /* start curses */
    initscr();

    /* initialize the colors we want */
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);

    // get the height/width of the window.
    int height, width = 0;
    getmaxyx(stdscr, height, width);

    /*  while we have room, we will animate our cat crossing... */
    int c =0;
    char *tail = (char*)malloc(width-8);
    while (c < width-8 ) {
        /*  print what we have. */
        refresh();
        c++;
        /* draw our cat! */
        /* allocate some space for the growing tail */
        draw_cat_step(c, height/2+2, tail);
        /* sleep for some */
        usleep(50000);
    }
    /*  end ncurses */
    endwin();
    free(tail);
    return(0);
}
