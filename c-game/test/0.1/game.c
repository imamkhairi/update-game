#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
//#include <time.h>

#define JUMP_HEIGHT 4

typedef struct {
    int x, y, vx, vy, dir, counter, animation, width, height;
    char *aa;
    bool jump;
} ctr;

typedef struct
{
    int x, y, counter;
    char *aa;
}obj;

int key;

//bool win_condition = FALSE;

void print_player(ctr *N)
{
    //mvprintw(N->y, N->x, N->aa);
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
            mvprintw(N->y, N->x,   "  o |");    
            mvprintw(N->y+1, N->x, " /|\\T");    
            mvprintw(N->y+2, N->x, " <|");  
        }
    }

    return;
}

void print_key(obj *N, int color)
{
    attron(COLOR_PAIR(color));
    mvprintw(N->y, N->x, N->aa);
    attroff(COLOR_PAIR(color));
    return;
}

void movectr(ctr *N)//fungsi ini bisa digabung sama input
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
        
    if(N->dir > 0) N->x++;
    else N->x--;
}

void input(int key, ctr *N)
{
    int dir_now = N->dir;
    bool move = FALSE;

    if(key == 'w') {
        N->vy = -1;
        move = TRUE;
    }
    if(key == 'a') {
        N->vx = -1;
        N->dir = -1;
        move = TRUE;
    }
    if(key == 's') {
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
 
void moveobj(obj *N)
{
    if(N->y % 2 == 0) N->y ++;
    else N->y --;
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
    player = newctr(10, 10);

    obj *object[10];
    object[0] = newobj("{}=M", 40, 3); //"K" disini udh ga guna

    timeout(0);
    
    //getmaxyx(stdscr, h, w);

    while (1)
    {
        erase();
        
        //makin ke bawah makin di atas
        print_key(object[0], 2); 
        print_player(player);

        key = getch();
        input(key, player);
        
        if(player->jump == FALSE) movectr(player);
        else {
            if(player->counter == 4){
                jump(player);
                player->counter = 0;
            }
            else player->counter ++;
        }

        if(object[0]->counter == 20) {
            moveobj(object[0]);
            object[0]->counter = 0;
        }
        else object[0]->counter++;

        if(collision(player, object[0]) == TRUE) {  
            break;
        }

        if(key == 'q') break;

        refresh();

        usleep(10000); //10ms
    }

    return (key);
}