#ifndef FISH_H
#define FISH_H
typedef struct
{
  char name[256];
  int x;
  int y;
  int vx;
  int vy;
  int width;
  int height;
  int started;
  int delay;
  void (*move)(void *);
} Fish;

void StayPathWay(void *f);
void DeadPathWay(void *f);
void VerticalPathWay(void *f);
void RandomPathWay(void *f);
void HorizontalPathWay(void *f);

void Fish_init(Fish *fish, char *n, int xpos, int ypos, int Vx, int Vy, int w, int h);
const char *Fish_getName(const Fish *fish);

int Fish_getX(const Fish *fish);
int Fish_getY(const Fish *fish);
void Fish_setX(Fish *fish, int xpos);
void Fish_setY(Fish *fish, int ypos);
void Fish_setPosition(Fish *fish, int xpos, int ypos);

int Fish_getVx(const Fish *fish);
int Fish_getVy(const Fish *fish);
void Fish_setVx(Fish *fish, int Vx);
void Fish_setVy(Fish *fish, int Vy);
void Fish_setSpeed(Fish *fish, int Vx, int Vy);

int Fish_getWidth(const Fish *fish);
int Fish_getHeight(const Fish *fish);

int Fish_isStarted(const Fish *fish);
void Fish_setStarted(Fish *fish, int value);

int Fish_getDelay(Fish *fish);
void Fish_setDelay(Fish *fish, int Delay);

int Aquarium_getFishDelay(Fish *f);
void Aquarium_setFishDelay(Fish *f, int Vx);

#endif /* FISH_H */
