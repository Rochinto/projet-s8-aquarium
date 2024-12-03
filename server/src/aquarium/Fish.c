#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "Aquarium.h"
#include "Fish.h"

void Fish_init(Fish *fish, char *n, int xpos, int ypos, int Vx, int Vy, int w, int h)
{
  strcpy(fish->name, n);
  Fish_setPosition(fish, xpos, ypos);
  Fish_setSpeed(fish, Vx, Vy);
  fish->width = w;
  fish->height = h;
  fish->started = 0;
}

const char *Fish_getName(const Fish *fish) { return fish->name; }

int Fish_getX(const Fish *fish) { return fish->x; }
int Fish_getY(const Fish *fish) { return fish->y; }
void Fish_setX(Fish *fish, int xpos) { fish->x = xpos; }
void Fish_setY(Fish *fish, int ypos) { fish->y = ypos; }
void Fish_setPosition(Fish *fish, int xpos, int ypos)
{
  Fish_setX(fish, xpos);
  Fish_setY(fish, ypos);
}

int Fish_getVx(const Fish *fish) { return fish->vx; }
int Fish_getVy(const Fish *fish) { return fish->vy; }
void Fish_setVx(Fish *fish, int Vx) { fish->vx = Vx; }
void Fish_setVy(Fish *fish, int Vy) { fish->vy = Vy; }
void Fish_setSpeed(Fish *fish, int Vx, int Vy)
{
  fish->vx = Vx;
  fish->vy = Vy;
}

int Fish_getWidth(const Fish *fish) { return fish->width; }
int Fish_getHeight(const Fish *fish) { return fish->height; }

int Fish_isStarted(const Fish *fish) { return fish->started; }
void Fish_setStarted(Fish *fish, int value) { fish->started = value; }

int Fish_getDelay(Fish *fish) { return fish->delay; }
void Fish_setDelay(Fish *fish, int Delay) { fish->delay = Delay; }

void StayPathWay(void *f)
{
  ((Fish *)f)->vx = 0;
  ((Fish *)f)->vy = 0;
}

void DeadPathWay(void *f)
{
  ((Fish *)f)->vx = 0;
  if (((Fish *)f)->height < 15 && ((Fish *)f)->width < 15)
  {
    if (((Fish *)f)->y > 0)
    {
      ((Fish *)f)->vy = -60;
    }
    else
    {
      ((Fish *)f)->vy = 0;
    }
  }
  else
  {
    if (((Fish *)f)->y < aq->height - ((Fish *)f)->height)
    {
      ((Fish *)f)->vy = 60;
    }
    else
    {
      ((Fish *)f)->vy = 0;
    }
  }
}

void RandomPathWay(void *f)
{
  ((Fish *)f)->vx = (2 * (rand() % 2) - 1) * (rand() % 60);
  ((Fish *)f)->vy = (2 * (rand() % 2) - 1) * (rand() % 60);
}

void HorizontalPathWay(void *f)
{
  ((Fish *)f)->vx = /*(2*(rand()%2) - 1)*/ (rand() % 60);
  ((Fish *)f)->vy = 0;
}

void VerticalPathWay(void *f)
{

  ((Fish *)f)->vx = 0;
  ((Fish *)f)->vy = (2 * (rand() % 2) - 1) * (rand() % 60);
}