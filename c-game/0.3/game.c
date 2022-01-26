#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "map.h"

#define JUMP_HEIGHT 4
#define ATTACK_DELAY 11


typedef struct {
    int x, y, vx, vy, dir, counter, animation, width, height;
    char *aa;
    bool jump, attack;
} ctr;

typedef struct
{
    int x, y, counter, direction;
    char *aa;
}obj;

int key;
bool win_condition = FALSE;
bool game_end = FALSE;

void print_player(ctr *N)
{
    //mvprintw(N->y, N->x, N->aa);
    attron(COLOR_PAIR(1) | A_BOLD);
    if(N->attack == FALSE){
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
    }
    else{
        if(N->counter <= ATTACK_DELAY){
            mvprintw(N->y, N->x-2,   "    o");    
            mvprintw(N->y+1, N->x-2, "──Ͱ─|\\");    
            mvprintw(N->y+2, N->x-2, "   / \\");
        }
        else if(N->counter <= ATTACK_DELAY*2){
            mvprintw(N->y, N->x-2,   "──Ͱ\\o");    
            mvprintw(N->y+1, N->x-2, "    |\\");    
            mvprintw(N->y+2, N->x-2, "   / \\");
        }
        else if(N->counter <= ATTACK_DELAY*3){
            mvprintw(N->y, N->x-2,   "──Ͱ(o");    
            mvprintw(N->y+1, N->x-2, "    |\\");    
            mvprintw(N->y+2, N->x-2, "   / \\");
        }
        else if(N->counter <= ATTACK_DELAY*5){
            mvprintw(N->y-1, N->x-2,   "  ____");    
            mvprintw(N->y, N->x-2,     ".'  o '.");    
            mvprintw(N->y+1, N->x-2,   "    \\⊣───");    
            mvprintw(N->y+2, N->x-2,   "   / \\");
        }
        else if(N->counter > ATTACK_DELAY*5)
        {
            N->attack = FALSE; //ini akan dipindah ke fungsi hitung damage
            N->counter = 0;
            N->animation = 0;
        }
    }
    attroff(COLOR_PAIR(1) | A_BOLD);
    return;
}

void print_player_att(ctr *N)
{
    if(N->counter == 6){
        mvprintw(N->y, N->x-3,   "    o");    
        mvprintw(N->y+1, N->x-3, "──Ͱ─|\\");    
        mvprintw(N->y+2, N->x-3, "   / \\");
    }
    else if(N->counter == 12){
        mvprintw(N->y, N->x-3,   "──Ͱ\\o");    
        mvprintw(N->y+1, N->x-3, "    |\\");    
        mvprintw(N->y+2, N->x-3, "   / \\");
    }
    else if(N->counter == 18){
        mvprintw(N->y, N->x-3,   "──Ͱ(o");    
        mvprintw(N->y+1, N->x-3, "    |\\");    
        mvprintw(N->y+2, N->x-3, "   / \\");
    }
    else if(N->counter == 24){
        mvprintw(N->y-1, N->x-3,   "  ____");    
        mvprintw(N->y, N->x-3,     ".'  o '.");    
        mvprintw(N->y+1, N->x-3,   "    \\⊣──");    
        mvprintw(N->y+2, N->x-3,   "   / \\");
    }
}

void print_obj(obj *N, int color)
{
    attron(COLOR_PAIR(color));
    mvprintw(N->y, N->x, N->aa);
    attroff(COLOR_PAIR(color));
    return;
}

void print_obj_all()
{
    
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

    N->counter = 0;
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
    if(key == 'j'){
        N->attack = TRUE;
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

    N->counter = 0;
}

void reset_ctr(ctr *N)
{
    N->x = 1;
    N->y = 17;
}

void move_trap_h(obj *N)
{
    if(N->direction == 1) N->x++;
    else N->x--; 

    if(N->x <= 40) N->direction = 1;
    else if(N->x >= 72) N->direction = -1;
}

void move_trap_v(obj *N)
{
    //direction 1 = up, 0 = down
    if(N->direction == 1) N->y--;
    else N->y++;

    if(N->y <= 17) N->direction = 0;
    else if(N->y >= 22) N->direction = 1;

    N->counter = 0;
}

void move_trap_all_h(obj *N[10])
{
    for(int i = 1; i < 7; i++)
    {
        move_trap_h(N[i]);
        N[i]->counter = 0;
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
    N->attack = FALSE;
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

bool collision_trap(ctr *N, obj *M)
{
    bool col_x = FALSE;
    bool col_y = FALSE;
    
    if(N->jump == FALSE){
        if((M->x >= N->x && M->x <= N->x + N->width)||(M->x + strlen(M->aa) - 1 >= N->x && M->x + strlen(M->aa) - 1 <= N->x + N->width)) col_x = TRUE;
        if(N->y+2 == M->y) col_y = TRUE;
        
        if(col_y == TRUE && col_x == TRUE) return TRUE;
        else return FALSE;
    }
    else return FALSE;
}

bool collision_trap_all(ctr *N, obj *M[10])
{
   if(collision_trap(N, M[1]) == TRUE ||
      collision_trap(N, M[2]) == TRUE ||
      collision_trap(N, M[3]) == TRUE ||
      collision_trap(N, M[4]) == TRUE ||
      collision_trap(N, M[5]) == TRUE ||
      collision_trap(N, M[6]) == TRUE ||
      collision_trap(N, M[7]) == TRUE ) return TRUE;
    else return FALSE;
}

int game()
{ 
    ctr *player;
    player = newctr(1, 17);

    obj *object[10];
    object[0] = newobj("{}=M", 88, 19);

    int pos[6];//inisialisasi trap, bisa pindah ke fungsi
    for (int i = 1; i < 7; i ++){
        pos[i-1] = (rand()%32) + 40;
        object[i] = newobj("<()>", pos[i-1], 16+i);
    }

    object[7] = newobj("<()>", object[0]->x - 6, 19);

    timeout(0);

    while (1)
    {
        erase();
        
        //makin ke bawah makin di atas
        map();

        for(int i = 0; i < 8; i ++){
            if(i == 0 ) print_obj(object[i], 2); //bisa di fungsi
            else print_obj(object[i], 3);
        }

        print_player(player);

        key = getch();

        input(key, player);
        
        if(player->attack == TRUE) player->counter++;

        if(player->jump == FALSE && player->attack == FALSE) move_ctr(player);
        else {
            if(player->counter == 4) jump(player);
            else player->counter ++;
        }

        if(object[0]->counter == 20) move_obj(object[0]);
        else object[0]->counter++;

        if(object[1]->counter == 6) move_trap_all_h(object);
        else object[1]->counter++;

        if(object[7]->counter == 15) move_trap_v(object[7]);
        else object[7]->counter++;

        if(collision(player, object[0]) == TRUE) {
            if(stage != 2){
                stage++;
                stage_print = FALSE;
                game_end = FALSE;//kenapa ini anjing
                reset_ctr(player);
                break;
            }
            else{
                win_condition = TRUE;
                game_end = TRUE;
                break;
            }
        }

        if(collision_trap_all(player, object) == TRUE){
            win_condition = FALSE;
            game_end = TRUE;
            break;
        }
        if(key == 'q') break;

        refresh();

        usleep(10000); //10ms
    }

    return (key);
}
//masih banyak yang bisa dirapiin