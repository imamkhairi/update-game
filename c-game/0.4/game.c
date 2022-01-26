#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "map.h"

#define JUMP_HEIGHT 4
#define ATTACK_PLAYER_DELAY 5
#define ENEMY_DELAY 20
#define ENEMY_X 100
#define ENEMY_Y 17
#define PLAYER_X 2
#define PLAYER_Y 17

typedef struct {
    int x, y, vx, vy, dir, counter, animation, width, height, health;
    char *aa;
    bool jump, attack_player, move;
} ctr;

typedef struct
{
    int x, y, counter, direction;
    char *aa;
    bool show;
}obj;

int key;
bool win_condition = FALSE;
bool game_end = FALSE;

int counter_map = 0;

bool collision(ctr *N, obj *M)
{
    bool col_x = FALSE;
    bool col_y = FALSE;

    if((M->x >= N->x && M->x <= N->x + N->width)||(M->x + strlen(M->aa) - 1 >= N->x && M->x + strlen(M->aa) - 1 <= N->x + N->width)) col_x = TRUE;
    if(M->y >= N->y && M->y <= N->y + N->height) col_y = TRUE;

    if(col_y == TRUE && col_x == TRUE) return TRUE;
    else return FALSE;
}

void print_player(ctr *N)
{
    //mvprintw(N->y, N->x, N->aa);
    attron(COLOR_PAIR(1) | A_BOLD);
    if(N->attack_player == FALSE){
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
        if(N->dir == 1){
            if(N->counter <= ATTACK_PLAYER_DELAY){
                mvprintw(N->y, N->x-2,   "    o");    
                mvprintw(N->y+1, N->x-2, "──Ͱ─|\\");    
                mvprintw(N->y+2, N->x-2, "   / \\");
            }
            else if(N->counter <= ATTACK_PLAYER_DELAY*2){
                mvprintw(N->y, N->x-2,   "──Ͱ\\o");    
                mvprintw(N->y+1, N->x-2, "    |\\");    
                mvprintw(N->y+2, N->x-2, "   / \\");
            }
            else if(N->counter <= ATTACK_PLAYER_DELAY*3){
                mvprintw(N->y, N->x-2,   "──Ͱ(o");    
                mvprintw(N->y+1, N->x-2, "    |\\");    
                mvprintw(N->y+2, N->x-2, "   / \\");
            }
            else if(N->counter <= ATTACK_PLAYER_DELAY*5){
                mvprintw(N->y-1, N->x-2,   "  _____");    
                mvprintw(N->y, N->x-2,     ".'  o  '.");    
                mvprintw(N->y+1, N->x-2,   "    \\⊣───");    
                mvprintw(N->y+2, N->x-2,   "   / \\");
            }
            else if(N->counter > ATTACK_PLAYER_DELAY*5)
            {
                N->attack_player = FALSE; //ini akan dipindah ke fungsi hitung damage
                N->counter = 0;
                N->animation = 0;
            }
        }
        else{
            if(N->counter <= ATTACK_PLAYER_DELAY){
                mvprintw(N->y, N->x,   " o    ");    
                mvprintw(N->y+1, N->x, "/|-⊣──");    
                mvprintw(N->y+2, N->x, "/ \\   ");
            }
            else if(N->counter <= ATTACK_PLAYER_DELAY*2){
                mvprintw(N->y, N->x,   " o/⊣──");    
                mvprintw(N->y+1, N->x, "/|    ");    
                mvprintw(N->y+2, N->x, "/ \\   ");
            }
            else if(N->counter <= ATTACK_PLAYER_DELAY*3){
                mvprintw(N->y, N->x,   " o)⊣──");    
                mvprintw(N->y+1, N->x, "/|    ");    
                mvprintw(N->y+2, N->x, "/ \\   ");
            }
            else if(N->counter <= ATTACK_PLAYER_DELAY*5){
                mvprintw(N->y-1, N->x-3,   "  _____  ");    
                mvprintw(N->y, N->x-3,     ".'  o  '.");    
                mvprintw(N->y+1, N->x-3,   "───Ͱ/    ");    
                mvprintw(N->y+2, N->x-3,   "   / \\   ");
            }
            else if(N->counter > ATTACK_PLAYER_DELAY*5)
            {
                N->attack_player = FALSE; //ini akan dipindah ke fungsi hitung damage
                N->counter = 0;
                N->animation = 0;
            }
        }
    }
    attroff(COLOR_PAIR(1) | A_BOLD);
    return;
}

void print_enemy(ctr *N)
{
    if(N->health != 0){
        attron(COLOR_PAIR(1) | A_BOLD);
        if(N->dir == 1){
            if(N->animation == 0){
                mvprintw(N->y, N->x+3,     "_");    
                mvprintw(N->y+1, N->x+2,  "/(}");    
                mvprintw(N->y+2, N->x+1, ";  /^");    
                mvprintw(N->y+3, N->x,  "/_ |");
            }
            else if(N->animation == 1){
                mvprintw(N->y, N->x+3,     "_");    
                mvprintw(N->y+1, N->x+2,  "/(}");    
                mvprintw(N->y+2, N->x+1, ";  /~");    
                mvprintw(N->y+3, N->x,  "/ _|");
            }    
        }
        else{
            if(N->animation == 0){
                mvprintw(N->y, N->x+2,    "_"   );    
                mvprintw(N->y+1, N->x+1, "{)\\" );    
                mvprintw(N->y+2, N->x,  "^\\  ;");    
                mvprintw(N->y+3, N->x+2, "| _\\");
            }
            else if(N->animation == 1){
                mvprintw(N->y, N->x+2,    "_"   );    
                mvprintw(N->y+1, N->x+1, "{)\\");    
                mvprintw(N->y+2, N->x,  "~\\  ;");    
                mvprintw(N->y+3, N->x+2, "|_ \\");
            }
        }
        attroff(COLOR_PAIR(1) | A_BOLD);
    }
}

void print_obj(obj *N, int color)
{
    if(N->show == TRUE){
        attron(COLOR_PAIR(color));
        mvprintw(N->y, N->x, N->aa);
        attroff(COLOR_PAIR(color));
    }
    return;
}

void print_obj_all(obj *N[10])//nanti aja
{
    for(int i = 0; i < 9; i ++){
        if(N[i]->show == FALSE) continue;
        if(i == 0) print_obj(N[i], 2);
        else print_obj(N[i], 3);
    }
}

bool activate_enemy (ctr *N, ctr *M)
{
    if(M->move == FALSE){
        if(N->x+3 >= ENEMY_X - 20 && N->x <= ENEMY_X + 26){
            M->move = TRUE;
        }
    }
    else{
        if(N->x+3 < ENEMY_X - 25 || N->x > ENEMY_X + 31){
            M->move = FALSE;
        }
    }
    return M->move;
}

void reset_ctr(ctr *N)
{
    N->x = PLAYER_X;
    N->y = PLAYER_Y;
    N->animation = 0;
    N->dir = 1;
}

void move_enemy(ctr *N, ctr *M)
{
    if(M->health != 0){
        if(activate_enemy(N, M) == TRUE){
            if(M->counter == ENEMY_DELAY){
                if(M->animation != 1) M->animation ++;
                else M->animation = 0;
                
                if(N->x+3 < M->x){
                    M->dir = -1;
                    M->x --;
                    if(N->x+3 == M->x-1 && M->health != 0){
                        N->health --;
                        reset_ctr(N);    
                    }
                }
                else if(N->x > M->x+5){
                    M->dir = 1;
                    M->x ++;
                }
                
                if(N->y-1 < M->y && N->jump == FALSE) M->y--;
                else if(N->y-1 > M->y && N->jump == FALSE) M->y++;

                M->counter = 0;
            }
            else M->counter++;
        }
        else{        
            if(M->counter == ENEMY_DELAY){
                
                    
                    
                if(M->x < ENEMY_X) {
                    if(M->animation != 1) M->animation ++;
                    else M->animation = 0;            
                    M->dir = 1;
                    M->x++;
                }
                if(M->x > ENEMY_X) {
                    if(M->animation != 1) M->animation ++;
                    else M->animation = 0;
                    M->x--;
                    M->dir = -1;
                }

                if(M->y < ENEMY_Y){
                    if(M->animation != 1) M->animation ++;
                    else M->animation = 0;
                    M->y++;
                }
                if(M->y > ENEMY_Y){
                    if(M->animation != 1) M->animation ++;
                    else M->animation = 0;  
                    M->y--;
                } 

                M->counter = 0;
            }
            else M->counter++;
        }
    }
}

void jump(ctr *N)
{
    if(N->counter == 4){
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
    else N->counter ++;
}

void move_player(ctr *N)//fungsi ini bisa digabung sama input
{   
    if(N->jump == FALSE){
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
    else jump(N);
}

void attack_player(ctr *N, ctr *M)
{
    if(N->attack_player == TRUE) N->counter ++;
    if(N->counter >= ATTACK_PLAYER_DELAY*3){
        if(N->dir == 1) {
            if(M->x >= N->x+3 && M->x <= N->x+6 && M->y == N->y-1) M->health = 0;
        }
        else{ 
            if(M->x+5 <= N->x && M->x+5 >= N->x-3 && M->y == N->y-1) M->health = 0;
        }
    }
}

void pick_player(ctr *N, obj *M){
    if(collision(N, M) == TRUE && M->show == TRUE) M->show = FALSE;
}

void input(int key, ctr *N, obj *M)
{
    int dir_now = N->dir;
    bool move = FALSE;

    if(key == 'w' && N->y >= 16 ) {
        N->vy = -1;
        move = TRUE;
    }
    if(key == 'a' && N->x > 0) {
        N->vx = -1;
        N->dir = -1;
        move = TRUE;
    }
    if(key == 's' && N->y <= 21 - 2) {
        N->vy = 1;
        move = TRUE;
    }
    if(key == 'd' && N->x+3 < 133) {
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
        N->attack_player = TRUE;
    }
    if(key == 'e'){
        pick_player(N, M);
    }

    if(move == TRUE && dir_now == N->dir){
        if(N->animation != 3) N->animation ++;
        else N->animation = 0;
    }
    else if(move == TRUE && dir_now != N->dir) N->animation = 0;
}
 
void move_obj(obj *N)
{
    if(N->show == TRUE){
        if(N->counter == 20){
            if(N->y % 2 == 0) N->y ++;
            else N->y --;
            N->counter = 0;
        }
        else N->counter ++;
    }
    else return;
}

void move_trap_h(obj *N)
{
    if(N->counter == 6)
    {
        if(N->direction == 1) N->x++;
        else N->x--; 

        if(N->x <= 30) N->direction = 1;
        else if(N->x >= 62) N->direction = -1;

        N->counter = 0;
    }
    else N->counter ++;
}

void move_trap_v(obj *N)
{
    //direction 1 = up, 0 = down
    if(N->counter == 15){
        if(N->direction == 1) N->y--;
        else N->y++;

        if(N->y <= 17) N->direction = 0;
        else if(N->y >= 22) N->direction = 1;

        N->counter = 0;
    }
    else N->counter ++;
}

// void move_trap_all_h(obj *N[10], int a)
// {
//     if(N[1]->counter == 15)
//     {
//         for(int i = 1; i <= a; i++)
//         {
//             move_trap_h(N[i]);
//             N[i]->counter = 0;
//         }
//     }    
//     else N[1]->counter ++;
// }

bool nextstage(ctr *N){
    if(N->x+4 > 132){
        if(stage != 3){
            stage++;
            stage_print = FALSE;
            game_end = FALSE;
            reset_ctr(N);
            return TRUE;
        }
        else{
            win_condition = TRUE;
            game_end = TRUE;
            return TRUE;
        }
        step = 0;
    }
    else return FALSE;
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
    N->attack_player = FALSE;
    N->move = FALSE;
    N->dir = 1; // 1 = right
    N->counter = 0;
    N->animation = 0;
    N->width = 3;
    N->height = 2;
    N->health = 1;

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
    N->show = TRUE;
   
    return (N);
}
 
bool collision_trap(ctr *N, obj *M)
{
    bool col_x = FALSE;
    bool col_y = FALSE;
    
    if(M->show == TRUE){
        if(N->jump == FALSE){
            if((M->x >= N->x && M->x <= N->x + N->width)||(M->x + strlen(M->aa) - 1 >= N->x && M->x + strlen(M->aa) - 1 <= N->x + N->width)) col_x = TRUE;
            if(N->y+2 == M->y) col_y = TRUE;
            
            if(col_y == TRUE && col_x == TRUE) {
                N->health --;
                reset_ctr(N);
                return TRUE;
            }
            else return FALSE;
        }
        else return FALSE;
    }
    else return FALSE;
}

void live_player(ctr *N)
{
    if(N->health == 2) mvprintw(1, 1, "Lives : 3");
    if(N->health == 1) mvprintw(1, 1, "Lives : 2");
    if(N->health == 0) mvprintw(1, 1, "Lives : 1");

}

void stage1_init(ctr *N[5], obj *M[10]){
    N[0] = newctr(PLAYER_X, PLAYER_Y);
    N[0]->health = 2;

    N[1] = newctr(ENEMY_X, ENEMY_Y);
    N[1]->health = 0;

    M[0] = newobj("{}=M", 125, 19);
    M[0]->show = FALSE;

    int pos = (rand()%32) + 30;
    M[1] = newobj("<()>", pos, 19);
    M[1]->show = FALSE;


}

int  stage1(ctr *N[5], obj *M[10], int k)
{
    if(counter_map == 100){
        if(step%2 == 0)step++;
        counter_map = 0;
    }
    else counter_map++;

    if(step == 1 && k == 'w') step++;
    if(step == 3 && k == 's') step++;
    if(step == 5 && k == 'a') step++;
    if(step == 7 && k == 'd') step++;
    if(step == 9 && k == 'j') step++;
    if(step == 11 && k == ' ') step++;
    if(step == 13){
        M[0]->show = TRUE;
        move_obj(M[0]);
        if(collision(N[0], M[0]) == TRUE) step+=2;
    }
    if(step == 15){
        move_obj(M[0]);
        if(M[0]->show == FALSE) step++;
    }
    if(step == 17){
        move_trap_h(M[1]);
        M[1]->show = TRUE;
        if(collision_trap(N[0], M[1]) == TRUE) {
            step+=2;
            M[1]->show = FALSE;
        }
    }
    if(step == 19){
        if(counter_map == 400) {
            N[0]->health ++;
            step = 21;
            counter_map = 0;
        }
        else counter_map ++;
    }
    if(step == 21){
        move_enemy(N[0], N[1]);
        if(counter_map == 0) {
            N[1]->health = 1;
            counter_map++;
        }
        if(N[1]->health == 0) {
            step ++;
            counter_map = 0;
        }
    }
    if(step == 23)
    {
        if(nextstage(N[0]) == TRUE) {
            step = 0;
            return 1;
        }
    }
    //if(nextstage(N[0]) == TRUE) return 1;
    print_obj(M[0], 2);
    print_obj(M[1], 3);
    print_enemy(N[1]);
    print_player(N[0]);
    
    attack_player(N[0], N[1]);
    move_player(N[0]);
    return 0;
}

void stage2_init(ctr *N[5], obj *M[10]){
    N[0] = newctr(PLAYER_X, PLAYER_Y);
    N[0]->health = 2;

    N[1] = newctr(ENEMY_X, ENEMY_Y);
    N[1]->health = 0;

    int pos[6];
    for (int i = 1; i < 7; i ++){
        pos[i-1] = (rand()%32) + 30;
        M[i] = newobj("<()>", pos[i-1], 16+i);
    }
    M[7] = newobj("<()>", 100, 19);
    M[8] = newobj("<()>", 110, 19);

    for(int i = 1; i <= 7; i++){
        M[i]->show = TRUE;
    }
}

int  stage2(ctr *N[5], obj *M[10], int k)
{
    print_obj_all(M);
    print_player(N[0]);

    move_trap_h(M[1]);
    move_trap_h(M[2]);
    move_trap_h(M[3]);
    move_trap_h(M[4]);
    move_trap_h(M[5]);
    move_trap_h(M[6]);
    move_trap_v(M[7]);
    move_trap_v(M[8]);
    
    collision_trap(N[0], M[2]);
    collision_trap(N[0], M[1]);
    collision_trap(N[0], M[3]);
    collision_trap(N[0], M[4]);
    collision_trap(N[0], M[5]);
    collision_trap(N[0], M[6]);
    collision_trap(N[0], M[7]);
    collision_trap(N[0], M[8]);
    
    attack_player(N[0], N[1]);
    move_player(N[0]);
    
    if(nextstage(N[0]) == TRUE) return 1;
    return 0;
}

void stage3_init(ctr *N[5], obj *M[10]){
    N[0] = newctr(PLAYER_X, PLAYER_Y);
    N[0]->health = 2;

    N[1] = newctr(ENEMY_X, ENEMY_Y);
    N[1]->health = 1;

    M[0] = newobj("{}=M", 125, 19);

    int pos[6];
    for (int i = 1; i < 7; i ++){
        pos[i-1] = (rand()%32) + 30;
        M[i] = newobj("<()>", pos[i-1], 16+i);
    }
    M[7] = newobj("<()>", M[0]->x - 10, 19);

    for(int i = 0; i <= 7; i++){
        M[i]->show = TRUE;
    }
}

int  stage3(ctr *N[5], obj *M[10], int k)
{
    print_obj(M[0], 2);
    print_obj(M[1], 3);
    print_obj(M[2], 3);
    print_obj(M[3], 3);
    print_obj(M[4], 3);
    print_obj(M[5], 3);
    print_obj(M[6], 3);
    print_obj(M[7], 3);

    print_enemy(N[1]);

    print_player(N[0]);
    
    move_obj(M[0]);
    move_trap_h(M[1]);
    move_trap_h(M[2]);
    move_trap_h(M[3]);
    move_trap_h(M[4]);
    move_trap_h(M[5]);
    move_trap_h(M[6]);
    move_trap_v(M[7]);
    move_enemy(N[0], N[1]);

    collision_trap(N[0], M[1]);
    collision_trap(N[0], M[2]);
    collision_trap(N[0], M[3]);
    collision_trap(N[0], M[4]);
    collision_trap(N[0], M[5]);
    collision_trap(N[0], M[6]);
    collision_trap(N[0], M[7]);
    
    attack_player(N[0], N[1]);
    move_player(N[0]);
    
    if(M[0]->show == FALSE) return 1;
    return 0;
}

int game()
{ 
    ctr *character[5];
    obj *object[10];

    
    if(stage == 1) stage1_init(character, object);
    if(stage == 2) stage2_init(character, object);
    if(stage == 3) stage3_init(character, object);

    timeout(0);

    while (1)
    {
        erase();
        
        //makin ke bawah makin di atas
        map();

        if(stage == 1) {
            if(stage1(character, object, key) == 1) break;
        }
        if(stage == 2){
            if(stage2(character, object, key) == 1) break;
        }
        if(stage == 3){
            if(stage3(character, object, key) == 1) {
                game_end = TRUE;
                win_condition = TRUE;
                break;
            }
        }
        //print_enemy(character[1]);
        live_player(character[0]);

        key = getch();
        input(key, character[0], object[0]);

        if(character[0]->health < 0){
            game_end = TRUE;
            win_condition = FALSE;
            break;
        }
        if(key == 'q') break;

        refresh();

        usleep(10000); //10ms
    }

    return (key);
}