#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "map.h"

int life;

int title(int *stage)
{
    int w, h;

    erase();
    getmaxyx(stdscr, h, w);

    mvprintw(h/2, (w-strlen("Main Menu"))/2, "Main Menu");
    mvprintw(h/2 + 2, (w-strlen("Press any key to play"))/2, "Press any key to play");
    mvprintw(h/2 + 3, (w-strlen("Press [Q] to quit"))/2, "Press [Q] to quit");
    
    refresh();

    *stage = 1;

    timeout(-1);
    return (getch());
}

int stage_change(bool *stage_print)
{
    int w, h;

    erase();
    getmaxyx(stdscr, h, w);

    mvprintw(h/2, (w-strlen("Stage Clear"))/2, "Stage Clear");
    mvprintw(h/2 + 2, (w-strlen("Press any key to continue"))/2, "Press any key to continue");
    mvprintw(h/2 + 3, (w-strlen("Press [Q] to quit"))/2, "Press [Q] to quit");
    
    refresh();

    *stage_print = TRUE;

    timeout(-1);
    return (getch());
}

int end_win(int *stage)
{
    int w, h;
 
    erase();
    getmaxyx(stdscr, h, w);

    mvprintw(h/2, (w-strlen("You Win"))/2, "You Win");
    mvprintw(h/2 + 2, (w-strlen("Press any key to Main Menu"))/2, "Press any key to Main Menu");
    mvprintw(h/2 + 3, (w-strlen("Press [Q] to quit"))/2, "Press [Q] to quit");

    refresh();

    *stage = 0;

    timeout(-1);
    return (getch());
}

int end_lose(int *stage)
{
    int w, h;
 
    erase();
    getmaxyx(stdscr, h, w);

    mvprintw(h/2, (w-strlen("You Lose"))/2, "You Lose");
    mvprintw(h/2 + 2, (w-strlen("Press any key to Main Menu"))/2, "Press any key to Main Menu");
    mvprintw(h/2 + 3, (w-strlen("Press [Q] to quit"))/2, "Press [Q] to quit");

    refresh();

    *stage = 0;

    timeout(-1);
    return (getch());
}

int main(void)
{
   
    setlocale(LC_ALL,"");
    initscr();
    curs_set(0);
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));
    
    srand(time(0));

    while (1)
    {
        if(stage == 0) if(title(&stage) == 'q') break;
        if(game() == 'q') break;
        
        //stage transition
        if(stage_print == FALSE){ 
            if(stage_change(&stage_print) == 'q') break;
        }
        
        if(win_condition == TRUE && game_end == TRUE) {
            if(end_win(&stage) == 'q') break;
        }
        
        if(win_condition == FALSE && game_end == TRUE) {
            if(end_lose(&stage) == 'q') break;
        }
    }

    endwin();

    return (0);
}
