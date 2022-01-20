#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "map.h"

#define JUMP_HEIGHT 4



typedef struct {
    int x, y, vx, vy, dir, counter, animation, width, height;
    char *aa;
    bool jump;
} ctr;

typedef struct
{
    int x, y, counter, direction;
    char *aa;
}obj;

int key;

//bool win_condition = FALSE;

void print_player(ctr *N)
{
    //mvprintw(N->y, N->x, N->aa);
    attron(COLOR_PAIR(1) | A_BOLD);
    if(N->dir == 1){
        if(N->animation == 0){
            mvprintw(N->y, N->x,   "| o");    
            mvprintw(N->y+1, N->x, "T/|\\");    
            mvprintw(N->y+2, N->x, " / \\");    
        }
        else if(N->animation == 1){
            mvprintw(N->y, N->x,   "| o");    
            mvprintw(N->y+1, N->x, "T/|\\");    
            mvprintw(N->y+2, N->x, "  >\\");    
        }
        else if(N->animation == 2){
            mvprintw(N->y, N->x,   "| o");    
            mvprintw(N->y+1, N->x, "T/|\\");    
            mvprintw(N->y+2, N->x, "  |\\");  
        }
        else if(N->animation == 3){
            mvprintw(N->y, N->x,   "| o");    
            mvprintw(N->y+1, N->x, "T/|\\");    
            mvprintw(N->y+2, N->x, "  |>");  
        }
    }
    else{
        if(N->animation == 0){
            mvprintw(N->y, N->x,   " o |");    
            mvprintw(N->y+1, N->x, "/|\\T");    
            mvprintw(N->y+2, N->x, "/ \\");    
        }
        else if(N->animation == 1){
            mvprintw(N->y, N->x,   " o |");    
            mvprintw(N->y+1, N->x, "/|\\T");    
            mvprintw(N->y+2, N->x, "/<");    
        }
        else if(N->animation == 2){
            mvprintw(N->y, N->x,   " o |");    
            mvprintw(N->y+1, N->x, "/|\\T");    
            mvprintw(N->y+2, N->x, "/|");  
        }
        else if(N->animation == 3){
            mvprintw(N->y, N->x,   " o |");    
            mvprintw(N->y+1, N->x, "/|\\T");    
            mvprintw(N->y+2, N->x, "<|");  
        }
    }
    attroff(COLOR_PAIR(1) | A_BOLD);
    return;
}

void print_obj(obj *N, int color)
{
    attron(COLOR_PAIR(color));
    mvprintw(N->y, N->x, N->aa);
    attroff(COLOR_PAIR(color));
    return;
}

void move_ctr(ctr *N)//fungsi ini bisa digabung sama input
{
    if(N->vx != 0)//ini kyknya gk penting
    {
        if(N->vx > 0)
        {
            N->x ++;
            N->vx --;
        }
        else
        {
            N->x --;
            N->vx ++;
        }
    }
    
    if(N->vy != 0)
    {
        if(N->vy > 0)
        {
            N->y ++;
            N->vy --;
        }
        else
        {
            N->y --;
            N->vy ++;
        }
    }
}

void jump(ctr *N)
{
    if(N->vy > 0)
    {
        N->y ++;
        N->vy --;
        if(N->vy == 0) N->jump = FALSE;
    }
    else
    {
        N->y --;
        N->vy ++;
        if(N->vy == 0) N->vy = JUMP_HEIGHT;
    }
        
    if(N->dir > 0) N->x = N->x + 2;
    else N->x = N->x - 2;
}

void input(int key, ctr *N)
{
    int dir_now = N->dir;
    bool move = FALSE;

    if(key == 'w' && N->y >= 16 ) {
        N->vy = -1;
        move = TRUE;
    }
    if(key == 'a') {
        N->vx = -1;
        N->dir = -1;
        move = TRUE;
    }
    if(key == 's' && N->y <= 21 - 2) {
        N->vy = 1;
        move = TRUE;
    }
    if(key == 'd') {
        N->vx = 1;
        N->dir = 1;
        move = TRUE;
    }
    if(key == ' ' && N->jump == FALSE) {
        N->vy = -JUMP_HEIGHT;
        N->jump = TRUE;
        move = TRUE;
        //N->animation = 0;
    }

    if(move == TRUE && dir_now == N->dir){
        if(N->animation != 3) N->animation ++;
        else N->animation = 0;
    }
    else if(move == TRUE && dir_now != N->dir) N->animation = 0;
}
 
void move_obj(obj *N)
{
    if(N->y % 2 == 0) N->y ++;
    else N->y --;
}

void move_trap(obj *N)
{
    if(N->direction == 1) N->x++;
    else N->x--; 

    if(N->x <= 40) N->direction = 1;
    else if(N->x >= 62) N->direction = -1;
}

void move_trap_all(obj *N[10])
{
    for(int i = 1; i < 7; i++)
    {
        move_trap(N[i]);
    }
}

ctr *newctr(int x, int y)
{
    ctr *N;
    N = (ctr *)malloc(sizeof(ctr));
    if(N == NULL) return (NULL);
    N->x = x;
    N->y = y;
    N->vx = 0;
    N->vy = 0;
    N->jump = FALSE;
    N->dir = 1; // 1 = right
    N->counter = 0;
    N->animation = 0;
    N->width = 3;
    N->height = 2;

    return (N);
}

obj *newobj(char *ascii, int x, int y)
{
    obj *N;
    N = (obj *)malloc(sizeof(obj));
    N->x = x;
    N->y = y;
    N->counter = 0;
    N->aa = ascii;
    N->direction = (rand() % 2) + 1;
    return (N);
}

bool collision(ctr *N, obj *M)
{
    bool col_x = FALSE;
    bool col_y = FALSE;

    if((M->x >= N->x && M->x <= N->x + N->width)||(M->x + strlen(M->aa) - 1 >= N->x && M->x + strlen(M->aa) - 1 <= N->x + N->width)) col_x = TRUE;
    if(M->y >= N->y && M->y <= N->y + N->height) col_y = TRUE;

    if(col_y == TRUE && col_x == TRUE) return TRUE;
    else return FALSE;
}

void win(int w, int h){
    mvprintw(h/2-4, (w-strlen("You win"))/2, "You win");
}

int game()
{ 
    //int w, h; 

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);

    ctr *player;
    player = newctr(1, 17);

    obj *object[10];
    object[0] = newobj("{}=M", 88, 19); //"K" disini udh ga guna

    int pos[6];//trap
    for (int i = 1; i < 7; i ++){
        pos[i-1] = (rand()%22) + 40;
        object[i] = newobj("<()>", pos[i-1], 16+i);
    }

    timeout(0);
    
    //getmaxyx(stdscr, h, w);

    while (1)
    {
        erase();
        
        //makin ke bawah makin di atas
        map();
        for(int i = 0; i < 7; i ++){
            if(i == 0 ) print_obj(object[i], 2);
            else print_obj(object[i], 3);
        }
        print_player(player);

        key = getch();
        input(key, player);
        
        if(player->jump == FALSE) move_ctr(player);
        else {
            if(player->counter == 4){
                jump(player);
                player->counter = 0;
            }
            else player->counter ++;
        }

        if(object[0]->counter == 20) {
            move_obj(object[0]);
            object[0]->counter = 0;
        }
        else object[0]->counter++;

        if(object[1]->counter == 5){
            move_trap_all(object);
            object[1]->counter = 0;
        }
        else object[1]->counter++;

        if(collision(player, object[0]) == TRUE) {  
            break;
        }

        if(collision(player,object[1]) == TRUE ||
           collision(player,object[2]) == TRUE ||
           collision(player,object[3]) == TRUE ||
           collision(player,object[4]) == TRUE ||
           collision(player,object[5]) == TRUE ||
           collision(player,object[6]) == TRUE ){
            break; // belum jadi kalah trus harus dikasih syarat jump
        }
        if(key == 'q') break;

        refresh();

        usleep(10000); //10ms
    }

    return (key);
}
//masih banyak yang bisa dirapiin