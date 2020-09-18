#ifndef GUI_H
#define GUI_H

#include <stdio.h>

typedef struct {
    char* text;
    void (*func)();
    MenuItem* next;
    MenuItem* prev;

    MenuItem* parent;
    MenuItem* children;

} MenuItem;

void display();

#endif