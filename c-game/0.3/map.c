#include <ncurses.h>
#include <string.h>
#include "map.h"
#define BUFLEN 256

int stage = 0;
bool stage_print = FALSE;

void drawmap(FILE *mapfile, bool *stage_print)
{
    char buf[BUFLEN];
    char *a;

    for (int i = 0; i < 23; i++){
        fgets(buf, BUFLEN, mapfile);
        a = strtok(buf, "\n");
        mvprintw(i, 0, "%s", a);
    }
    *stage_print = TRUE;
}

void map()
{
    FILE *map;
    if(stage == 1) map = fopen("map1.txt", "r");
    if(stage == 2) map = fopen("map2.txt", "r");

    attron(COLOR_PAIR(1) | A_DIM);
    //stage1();
    drawmap(map, &stage_print);
    attroff(COLOR_PAIR(1) | A_DIM);

    fclose(map);
}