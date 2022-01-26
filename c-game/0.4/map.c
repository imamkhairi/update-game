#include <ncurses.h>
#include <string.h>
#include "map.h"
#define BUFLEN 256

int stage = 0;
bool stage_print = FALSE;
int step = 0;

void drawmap(FILE *mapfile, bool *stage_print)
{
    char buf[BUFLEN];
    char *a;

    for (int i = 0; i < 23; i++){
        fgets(buf, BUFLEN, mapfile);
        a = strtok(buf, "\n");
        attron(COLOR_PAIR(1) | A_DIM);
        mvprintw(i, 0, "%s", a);
        attroff(COLOR_PAIR(1) | A_DIM);
    }
    *stage_print = TRUE;
}

void tutorial(int a)
{
    int w, h;
    getmaxyx(stdscr, h, w);
    attron(COLOR_PAIR(1) | A_BOLD);
    if(a>0 && a%2 == 0) mvprintw(h/2 - 3, (w-strlen("Good job!"))/2, "Good job!");
    else{
        if(a == 1) mvprintw(h/2 - 3, (w-strlen("Press W to move up"))/2, "Press W to move up");
        if(a == 3) mvprintw(h/2 - 3, (w-strlen("Press S to move down"))/2, "Press S to move down");
        if(a == 5) mvprintw(h/2 - 3, (w-strlen("Press A to move left"))/2, "Press A to move left");
        if(a == 7) mvprintw(h/2 - 3, (w-strlen("Press D to move right"))/2, "Press D to move right");
        if(a == 9) mvprintw(h/2 - 3, (w-strlen("Press J to attack"))/2, "Press J to attack");
        if(a == 11) mvprintw(h/2 - 3, (w-strlen("Press SPACEBAR to move jump"))/2, "Press SPACEBAR to move jump");
        if(a == 13) mvprintw(h/2 - 3, (w-strlen("Go to the key"))/2, "Go to the key");
        if(a == 15) mvprintw(h/2 - 3, (w-strlen("Press E to pickup the key"))/2, "Press E to pickup the key");
        if(a == 17) mvprintw(h/2 - 3, (w-strlen("Go to the trap"))/2, "Go to the trap");
        if(a == 19){
            mvprintw(h/2 - 3, (w-strlen("Good, you stil follow the instruction."))/2, "Good, you stil follow the instruction.");
            mvprintw(h/2 - 2, (w-strlen("Even though you know that the trap will kill you."))/2, "Even though you know that the trap will kill you.");
            mvprintw(h/2 - 1, (w-strlen("So, we will give your live back."))/2, "So, we will give your live back.");
        }
        if(a == 21) mvprintw(h/2 - 3, (w-strlen("Now, try to kill the enemy"))/2, "Now, try to kill the enemy");
        if(a == 23){
            mvprintw(h/2 - 3, (w-strlen("You have finished the tutorial"))/2, "You have finished the tutorial.");
            mvprintw(h/2 - 2, (w-strlen("Now you can proceed to the next stage"))/2, "Now you can proceed to the next stage");
        }    
    }
    
    attroff(COLOR_PAIR(1) | A_BOLD);
}

void map()
{
    FILE *map;
    if(stage == 1) map = fopen("map1.txt", "r");
    if(stage == 2) map = fopen("map2.txt", "r");
    if(stage == 3) map = fopen("map2.txt", "r");


    //stage1();
    drawmap(map, &stage_print);
    if(stage == 1) tutorial(step);


    fclose(map);
}