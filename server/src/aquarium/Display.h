#ifndef DISPLAY_H
#define DISPLAY_H

#include <time.h>
typedef struct
{
  char name[256];
  int x;
  int y;
  int width;
  int height;
  int client_fd;
  time_t last_ping;
} Display;

#define EMPTY_FD -1;

void Display_init(Display *display, char *n, int xpos, int ypos, int w, int h);
const char *Display_getName(const Display *display);
int Display_getX(const Display *display);
int Display_getY(const Display *display);
void Display_setX(Display *display, int xpos);
void Display_setY(Display *display, int ypos);
void Display_setPosition(Display *display, int xpos, int ypos);
int Display_getWidth(const Display *display);
int Display_getHeight(const Display *display);

#endif /* DISPLAY_H */
