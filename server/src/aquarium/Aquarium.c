#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "logger.h"
#include "Aquarium.h"
#include "fd_el.h"

Aquarium *aq;
// Liste chainée Fish

int cmpFish(const void *a, const void *b)
{
    return strcmp(Aquarium_getFishName((const Fish *)a), Aquarium_getFishName((const Fish *)b));
}

void *copyFish(const void *a)
{
    Fish *b = malloc(sizeof(*b));
    strcpy(b->name, ((const Fish *)a)->name);
    b->x = ((const Fish *)a)->x;
    b->y = ((const Fish *)a)->y;
    b->vx = ((const Fish *)a)->vx;
    b->vy = ((const Fish *)a)->vy;
    b->height = ((const Fish *)a)->height;
    b->width = ((const Fish *)a)->width;
    b->move = ((const Fish *)a)->move;
    b->delay = ((const Fish *)a)->delay;
    b->started = ((const Fish *)a)->started;
    return b;
}

void delFish(void *a)
{
    free(a);
}

int cmpDisplay(const void *a, const void *b)
{
    return strcmp(Aquarium_getDisplayName((const Display *)a), Aquarium_getDisplayName((const Display *)b));
}

void *copyDisplay(const void *a)
{
    Display *b = malloc(sizeof(*b));
    *b = *(const Display *)a;
    return b;
}

void delDisplay(void *a)
{
    free(a);
}

int filterDisplayByName(void *d, void *args)
{
    return strcmp(Aquarium_getDisplayName((Display *)d), (char *)args) != 0;
}

void printDisplay(void *d, void *args)
{
    (void)args;
    printf("%s ", Aquarium_getDisplayName((Display *)d));
    printf("%dx", Aquarium_getDisplayX((Display *)d));
    printf("%d+", Aquarium_getDisplayY((Display *)d));
    printf("%d+", Aquarium_getDisplayWidth((Display *)d));
    printf("%d\n", Aquarium_getDisplayHeight((Display *)d));
}

void saveDisplay(void *d, void *args)
{
    char *logprint = (char *)args;
    char t[100];
    sprintf(t, "%s ", Aquarium_getDisplayName((Display *)d));
    strcat(logprint, t);
    sprintf(t, "%dx", Aquarium_getDisplayX((Display *)d));
    strcat(logprint, t);
    sprintf(t, "%d+", Aquarium_getDisplayY((Display *)d));
    strcat(logprint, t);
    sprintf(t, "%d+", Aquarium_getDisplayWidth((Display *)d));
    strcat(logprint, t);
    sprintf(t, "%d\n", Aquarium_getDisplayHeight((Display *)d));
    strcat(logprint, t);
}
void Aquarium_create(char *n, int w, int h, time_t timeout)
{
    if (aq != NULL)
    {
        Aquarium_del();
    }
    aq = malloc(sizeof(Aquarium));
    strcpy(aq->name, n);
    aq->width = w;
    aq->height = h;
    aq->displayList = set__empty(cmpDisplay, copyDisplay, delDisplay);
    aq->fishList = set__empty(cmpFish, copyFish, delFish);
    aq->timeout = timeout;
}

char *Aquarium_getName() { return aq->name; }

int Aquarium_getWidth() { return aq->width; }
int Aquarium_getHeight() { return aq->height; }

void Aquarium_printAquarium()
{
    printf("%dx%d\n", aq->width, aq->height);
    set__for_each(printDisplay, aq->displayList, NULL);
}

void Aquarium_saveAquarium(char *logprint)
{
    char t[100];
    sprintf(t, "%dx%d\n", aq->width, aq->height);
    strcat(logprint, t);
    set__for_each(saveDisplay, aq->displayList, logprint);
}

Aquarium *Aquarium_get()
{
    return aq;
}

int Aquarium_findDisplay(Display *d)
{
    return set__find(aq->displayList, d);
}

int displayCmpFd(void *d, void *fd)
{
    int is_expired = *(int *)fd == -1 ? (time(NULL) - ((Display *)d)->last_ping) > aq->timeout : 0;
    return ((Display *)d)->client_fd == *(int *)fd || is_expired;
}

int displayCmpName(void *d, void *name)
{
    return strcmp(((Display *)d)->name, (char *)name) == 0;
}

Display *Aquarium_findDisplayByFd(int fd)
{
    return set__search_by(aq->displayList, displayCmpFd, (void *)&fd);
}

Display *Aquarium_findDisplayByName(char *name)
{
    return set__search_by(aq->displayList, displayCmpName, (void *)name);
}

Display *Aquarium_setFdOnDisplay(int old_fd, int new_fd)
{
    Display *d = set__search_by(aq->displayList, displayCmpFd, (void *)&old_fd);
    if (d != NULL)
    {
        d->client_fd = new_fd;
        d->last_ping = time(NULL);
        return d;
    }
    return NULL;
}

int Aquarium_displayIsTimeout(const Display *d)
{
    return (time(NULL) - d->last_ping) > aq->timeout;
}

void Aquarium_updatePing(Display *d)
{
    d->last_ping = time(NULL);
}

void Aquarium_createDisplay(char *n, int xpos, int ypos, int w, int h)
{
    Display display;
    Display_init(&display, n, xpos, ypos, w, h);
    if (!Aquarium_findDisplay(&display))
    {
        set__add(aq->displayList, &display);
    }
}

const char *Aquarium_getDisplayName(const Display *d) { return Display_getName(d); }
int Aquarium_getDisplayX(const Display *d) { return Display_getX(d); }
int Aquarium_getDisplayY(const Display *d) { return Display_getY(d); }
void Aquarium_setDisplayX(Display *d, int xpos) { Display_setX(d, xpos); }
void Aquarium_setDisplayY(Display *d, int ypos) { Display_setX(d, ypos); }
void Aquarium_setDisplayPosition(Display *d, int xpos, int ypos) { Display_setPosition(d, xpos, ypos); }
int Aquarium_getDisplayWidth(const Display *d) { return Display_getWidth(d); }
int Aquarium_getDisplayHeight(const Display *d) { return Display_getHeight(d); }

void Aquarium_delDisplay(Display *d)
{
    set__remove(aq->displayList, d);
}

void Aquarium_delDisplayByName(char *displayName)
{
    struct set *new_displayList = set__filter(filterDisplayByName, aq->displayList, (void *)displayName);
    set__free(aq->displayList);
    aq->displayList = new_displayList;
}

int Aquarium_isFishInDisplay(Fish *f, Display *d)
{
    int min_x = Display_getX(d);
    int min_y = Display_getY(d);
    int max_x = min_x + Display_getWidth(d);
    int max_y = min_y + Display_getHeight(d);

    int pos_x = Aquarium_getFishX(f);
    int pos_y = Aquarium_getFishY(f);

    if (pos_x < min_x - ((Fish *)f)->width || max_x < pos_x)
    {
        return 0;
    }
    if (pos_y < min_y - ((Fish *)f)->height || max_y < pos_y)
    {
        return 0;
    }
    return 1;
}

void printFish(void *f, void *args)
{
    struct fd_el *fd_buf = (struct fd_el *)args;

    Display *d = Aquarium_findDisplayByFd(fd_buf->fd);
    if (d == NULL || Aquarium_isFishInDisplay((Fish *)f, d) != 1)
    {
        return;
    }

    int rel_x = ((((Fish *)f)->x - d->x) * 100) / d->width;
    int rel_y = ((((Fish *)f)->y - d->y) * 100) / d->height;
    int rel_w = ((((Fish *)f)->width) * 100 / d->width);
    int rel_h = ((((Fish *)f)->height) * 100 / d->height);
    // [PoissonRouge at 30x30,10x4,5]
    sprintf(fd_buf->buffer, "%s [%s at %dx%d,%dx%d,%d]", fd_buf->buffer, ((Fish *)f)->name,
            rel_x, rel_y, rel_w, rel_h, ((Fish *)f)->delay);
}

void Aquarium_printFishes(int fd)
{
    size_t fish_list_size = set__size(aq->fishList);
    if (fish_list_size <= 0)
    {
        return;
    }
    struct fd_el *fd_buf = malloc(sizeof(struct fd_el));
    fd_buf->fd = fd;
    fd_buf->buffer = malloc(sizeof(char) * (512 * (fish_list_size + 1)));
    sprintf(fd_buf->buffer, "list");
    set__for_each(printFish, aq->fishList, (void *)fd_buf);
    sprintf(fd_buf->buffer, "%s\n", fd_buf->buffer);
    write(fd_buf->fd, fd_buf->buffer, strlen(fd_buf->buffer));
    free(fd_buf->buffer);
    free(fd_buf);
}

void killFishes(void *f, void *args){
    Fish * p = (Fish *)f;
    p->move = DeadPathWay;
}

int Aquarium_lessive(){
    set__for_each(killFishes, aq->fishList, NULL);
    return 0;
}

int Aquarium_findFish(Fish *f)
{
    return set__find(aq->fishList, f);
}

int fishCmpName(void *d, void *name)
{
    return strcmp(((Fish *)d)->name, (char *)name) == 0;
}

Fish *Aquarium_findFishByName(char *name)
{
    return set__search_by(aq->fishList, fishCmpName, (void *)name);
}

int Aquarium_addFish(Fish *fish)
{
    return set__add(aq->fishList, fish);
}

int Aquarium_deleteFish(Fish *fish)
{
    return set__remove(aq->fishList, fish);
}

const char *Aquarium_getFishName(const Fish *f) { return Fish_getName(f); }
int Aquarium_getFishX(const Fish *f) { return Fish_getX(f); }
int Aquarium_getFishY(const Fish *f) { return Fish_getY(f); }
void Aquarium_setFishX(Fish *f, int xpos) { Fish_setX(f, xpos); }
void Aquarium_setFishY(Fish *f, int ypos) { Fish_setY(f, ypos); }
void Aquarium_setFishPosition(Fish *f, int xpos, int ypos) { Fish_setPosition(f, xpos, ypos); }

int Aquarium_getFishVx(const Fish *f) { return Fish_getVx(f); }
int Aquarium_getFishVy(const Fish *f) { return Fish_getVy(f); }
void Aquarium_setFishVx(Fish *f, int Vx) { Fish_setVx(f, Vx); }
void Aquarium_setFishVy(Fish *f, int Vy) { Fish_setVy(f, Vy); }
void Aquarium_setFishSpeed(Fish *f, int Vx, int Vy) { Fish_setSpeed(f, Vx, Vy); }

void Aquarium_delFish(Fish *f)
{
    set__remove(aq->fishList, f);
}
int Aquarium_getFishWidth(const Fish *f) { return Fish_getWidth(f); }
int Aquarium_getFishHeight(const Fish *f) { return Fish_getHeight(f); }

int Aquarium_isFishStarted(const Fish *f) { return Fish_isStarted(f); }
void Aquarium_setFishStarted(Fish *f, int value) { Fish_setStarted(f, value); }

void Aquarium_del()
{
    set__free(aq->displayList);
    set__free(aq->fishList);
    free(aq);
}

void Aquarium_fishMaj(void *f, void *args)
{
    (void)args;
    if (Aquarium_isFishStarted((Fish *)f))
    {
        ((Fish *)f)->move(f);
        if (Aquarium_getFishX((Fish *)f) + Aquarium_getFishVx((Fish *)f) < 0)
        {
            Aquarium_setFishPosition((Fish *)f,
                                     0,
                                     Aquarium_getFishY((Fish *)f) + Aquarium_getFishVy((Fish *)f));
        }
        else if (Aquarium_getFishX((Fish *)f) + Aquarium_getFishVx((Fish *)f) > Aquarium_getWidth() - Aquarium_getFishWidth((Fish *)f))
        {
            Aquarium_setFishPosition((Fish *)f,
                                     Aquarium_getWidth() - Aquarium_getFishWidth((Fish *)f),
                                     Aquarium_getFishY((Fish *)f) + Aquarium_getFishVy((Fish *)f));
        }
        else if (Aquarium_getFishY((Fish *)f) + Aquarium_getFishVy((Fish *)f) < 0)
        {
            Aquarium_setFishPosition((Fish *)f,
                                     Aquarium_getFishX((Fish *)f) + Aquarium_getFishVx((Fish *)f),
                                     0);
        }
        else if (Aquarium_getFishY((Fish *)f) + Aquarium_getFishVy((Fish *)f) > Aquarium_getHeight() - Aquarium_getFishHeight((Fish *)f))
        {
            Aquarium_setFishPosition((Fish *)f,
                                     Aquarium_getFishX((Fish *)f) + Aquarium_getFishVx((Fish *)f),
                                     Aquarium_getHeight() - Aquarium_getFishHeight((Fish *)f));
        }
        else
        {
            Aquarium_setFishPosition((Fish *)f,
                                     Aquarium_getFishX((Fish *)f) + Aquarium_getFishVx((Fish *)f),
                                     Aquarium_getFishY((Fish *)f) + Aquarium_getFishVy((Fish *)f));
        }
    }
}
void Aquarium_maj()
{
    set__for_each(Aquarium_fishMaj, aq->fishList, NULL);
}

int Aquarium_getFishDelay(Fish *f) { return Fish_getDelay(f); }
void Aquarium_setFishDelay(Fish *f, int Vx) { Fish_setDelay(f, Vx); }
