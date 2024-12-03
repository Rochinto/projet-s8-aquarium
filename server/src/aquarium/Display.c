#include <stdio.h>
#include <string.h>

#include "Display.h"

void Display_init(Display *display, char *n, int xpos, int ypos, int w, int h)
{
    strcpy(display->name, n);
    Display_setPosition(display, xpos, ypos);
    display->width = w;
    display->height = h;
    display->client_fd = EMPTY_FD;
    display->last_ping = time(NULL);
}

const char *Display_getName(const Display *display) { return display->name; }
int Display_getX(const Display *display) { return display->x; }
int Display_getY(const Display *display) { return display->y; }
void Display_setX(Display *display, int xpos) { display->x = xpos; }
void Display_setY(Display *display, int ypos) { display->y = ypos; }

void Display_setPosition(Display *display, int xpos, int ypos)
{
    Display_setX(display, xpos);
    Display_setY(display, ypos);
}

int Display_getWidth(const Display *display) { return display->width; }
int Display_getHeight(const Display *display) { return display->height; }
