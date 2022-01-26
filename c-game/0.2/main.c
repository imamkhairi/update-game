#include <locale.h>
#include <ncurses.h>
#include <string.h>
#include "game.h"

int title(void)
{
    int w, h;

    erase();
    getmaxyx(stdscr, h, w);

    mvprintw(h/2, (w-strlen("Main Menu"))/2, "Main Menu");
    mvprintw(h/2 + 2, (w-strlen("Press any key to play"))/2, "Press any key to play");
    mvprintw(h/2 + 3, (w-strlen("Press [Q] to quit"))/2, "Press [Q] to quit");
    
    refresh();

    timeout(-1);
    return (getch());
}

int end()
{
    int w, h;
 
    erase();
    getmaxyx(stdscr, h, w);

    mvprintw(h/2, (w-strlen("You Win"))/2, "You Win");
    mvprintw(h/2 + 2, (w-strlen("Press any key to Main Menu"))/2, "Press any key to Main Menu");
    mvprintw(h/2 + 3, (w-strlen("Press [Q] to quit"))/2, "Press [Q] to quit");

    refresh();

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
    init_pair(3, COLOR_BLACK, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));
    
    while (1)
    {
        if(title() == 'q') break;
        if(game() == 'q') break;
        if(end() == 'q') break;
    }
    endwin();

    return (0);
}
