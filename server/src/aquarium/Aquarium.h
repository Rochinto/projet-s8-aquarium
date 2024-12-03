#ifndef AQUARIUM_H
#define AQUARIUM_H

#include <time.h>

#include "Fish.h"
#include "Display.h"
#include "set.h"
typedef struct
{
  char name[256];
  int width;
  int height;
  struct set *fishList;
  struct set *displayList;
  time_t timeout;
} Aquarium;

extern Aquarium *aq;

void Aquarium_create(char *n, int w, int h, time_t timeout);
char *Aquarium_getName();
int Aquarium_getWidth();
int Aquarium_getHeight();
void Aquarium_printAquarium();
void Aquarium_saveAquarium(char *log);
void saveDisplay(void *d, void *args);
Aquarium *Aquarium_get();

int Aquarium_findDisplay(Display *d);
Display *Aquarium_findDisplayByFd(int fd);
Display *Aquarium_findDisplayByName(char *name);
Display *Aquarium_setFdOnDisplay(int old_fd, int new_fd);
int Aquarium_displayIsTimeout(const Display *d);
void Aquarium_updatePing(Display *d);
void Aquarium_createDisplay(char *n, int xpos, int ypos, int w, int h);
const char *Aquarium_getDisplayName(const Display *d);
int Aquarium_getDisplayX(const Display *d);
int Aquarium_getDisplayY(const Display *d);
void Aquarium_setDisplayX(Display *d, int xpos);
void Aquarium_setDisplayY(Display *d, int ypos);
void Aquarium_setDisplayPosition(Display *d, int xpos, int ypos);
int Aquarium_getDisplayWidth(const Display *d);
int Aquarium_getDisplayHeight(const Display *d);
void Aquarium_delDisplay(Display *d);
void Aquarium_delDisplayByName(char *displayName);

int Aquarium_findFish(Fish *f);
int Aquarium_addFish(Fish *f);
int Aquarium_deleteFish(Fish *fish);

const char *Aquarium_getFishName(const Fish *f);
Fish *Aquarium_findFishByName(char *name);
int Aquarium_getFishX(const Fish *f);
int Aquarium_getFishY(const Fish *f);
void Aquarium_setFishX(Fish *f, int xpos);
void Aquarium_setFishY(Fish *f, int ypos);
void Aquarium_setFishPosition(Fish *f, int xpos, int ypos);
int Aquarium_getFishVx(const Fish *f);
int Aquarium_getFishVy(const Fish *f);
void Aquarium_setFishVx(Fish *f, int Vx);
void Aquarium_setFishVy(Fish *f, int Vy);
void Aquarium_setFishSpeed(Fish *f, int Vx, int Vy);
int Aquarium_getFishWidth(const Fish *f);
int Aquarium_getFishHeight(const Fish *f);
int Aquarium_isFishStarted(const Fish *f);
void Aquarium_setFishStarted(Fish *f, int value);
void Aquarium_delFish(Fish *f);
void Aquarium_printFishes(int fd);

void Aquarium_del();

void Aquarium_maj();

int Aquarium_isFishInDisplay(Fish *f, Display *d);
int Aquarium_lessive();
#endif