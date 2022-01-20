#ifndef MAP_H
#define MAP_H

extern void map();
//extern bool win_condition;

typedef struct {
  	int w, h;
    char wall[256][256];
}Map;

#endif