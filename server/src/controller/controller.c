#include <string.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "controller.h"
#include "Aquarium.h"
#include "link.h"
#include "interpreter.h"
#include "logger.h"
#include "fd_el.h"
#include "set.h"
#include "Aquarium.h"

#define NB_COMMANDS 8
#define CONTROLLER_BUF_SIZE 256
#define NB_FISH_MOVES 5

typedef struct
{
    char *name;
    void (*move)(void *);
} Fish_move;

Fish_move Fish_moves[NB_FISH_MOVES] = {
    {"StayPathway", StayPathWay},
    {"RandomPathway", RandomPathWay},
    {"DeadPathway", DeadPathWay},
    {"VerticalPathway", VerticalPathWay},
    {"HorizontalPathway", HorizontalPathWay}};

struct set *loop_fd = NULL;
static int e = EMPTY_FD;
int is_on = 0;
pthread_t thread_getFishesContinuously;
pthread_t thread_updateFishes;

int controller_getFishes(struct line *l, int fd)
{
    Aquarium_printFishes(fd);
    return 0;
}

int controller_ls(struct line *l, int fd)
{
    return 0;
}

void func_getFishContinuously(void *fd_el, void *args)
{
    (void)args;
    Display *d = Aquarium_findDisplayByFd(((struct fd_el *)fd_el)->fd);
    if (d == NULL)
    {
        return;
    }
    Aquarium_updatePing(d);
    Aquarium_printFishes(((struct fd_el *)fd_el)->fd);
}

static void *loop_getFishContinuously(void *args)
{
    (void)args;
    while (is_on != 0)
    {
        set__for_each(func_getFishContinuously, loop_fd, NULL);
        sleep(1);
    }
    pthread_exit(NULL);
}

int controller_getFishesContinuously(struct line *l, int fd)
{
    // TODO :  generate while on fish list
    (void)l;
    (void)fd;
    struct fd_el new_fd;
    new_fd.fd = fd;
    new_fd.buffer = "";
    if (set__find(loop_fd, &new_fd))
    {
        return 0;
    }
    else
    {
        set__add(loop_fd, &new_fd);
    }
    return 0;
}

int controller_logout(struct line *l, int fd)
{
    char const *message = "bye";
    Display *d = NULL;
    if (l != NULL && strcmp(l->element, "out") == 0)
    {
        d = Aquarium_setFdOnDisplay(fd, e);
        if (d == NULL)
        {
            log_message(WARN, "Display not found for fd %d", fd);
            return 1;
        }
        write(fd, message, strlen(message));
        return 0;
    }
    else
    {
        log_message(WARN, "Log out interpretation failed");
        return 1;
    }
}

int controller_ping(struct line *l, int fd)
{
    (void)(l);
    char message[CONTROLLER_BUF_SIZE] = "pong ";
    if (l != NULL)
    {
        if (strlen(l->element) > CONTROLLER_BUF_SIZE - 7)
        {
            return 1;
        }
        strcat(message, l->element);
        strcat(message, "\n");
        write(fd, message, strlen(message));
        return 0;
    }
    return 1;
}

int controller_addFish(struct line *l, int fd)
{
    Display *d = Aquarium_findDisplayByFd(fd);

    if (l == NULL || l->nbelement != 4)
    {
        log_message(WARN, "Add fish failed : Wrong arguments number");
        return 1;
    }
    Fish fish;
    strcpy(fish.name, l->element);
    l = l->next;
    if (l == NULL || strcmp(l->element, "at") != 0)
    {
        log_message(WARN, "Add fish failed : Bad command format for fish %s", fish.name);
        return 1;
    }

    /* Get fish parameters : XxY, WxH, pathway*/
    l = l->next;
    if (l == NULL)
    {
        log_message(WARN, "Add fish failed : Wrong arguments number");
        return 1;
    }
    struct line *fish_params = create_line(l->element, ",");
    if (fish_params->nbelement != 2)
    {
        free_line(fish_params);
        log_message(WARN, "Add fish failed : Invalid argument number %s", fish.name);
        return 1;
    }

    /* Get fish position : XxY*/
    struct line *fish_position = create_line(fish_params->element, "x");
    if (fish_position->nbelement != 2)
    {
        free_line(fish_position);
        free_line(fish_params);
        log_message(WARN, "Add fish failed : Bad position format for fish %s", fish.name);
        return 1;
    }
    fish.x = (d->width * atoi(fish_position->element) / 100) + d->x;
    fish_position = pop_arg(fish_position);
    fish.y = (d->height * atoi(fish_position->element) / 100) + d->y;
    free_line(fish_position);

    /* Get fish size : XxY*/
    fish_params = pop_arg(fish_params);
    struct line *fish_size = create_line(fish_params->element, "x");
    if (fish_size->nbelement != 2)
    {
        free_line(fish_size);
        free_line(fish_params);
        log_message(WARN, "Add fish failed : Bad size format for fish %s", fish.name);
        return 1;
    }
    fish.width = atoi(fish_size->element) * (d->width / 100);
    fish_size = pop_arg(fish_size);
    fish.height = atoi(fish_size->element) * (d->height / 100);
    free_line(fish_size);

    free_line(fish_params);
    l = l->next;

    /* Get fish delay*/
    fish.move = NULL;
    for (int i = 0; i < NB_FISH_MOVES; i++)
    {
        if (strcmp(l->element, Fish_moves[i].name) == 0)
        {
            fish.move = Fish_moves[i].move;
            break;
        }
    }
    if (fish.move == NULL)
    {
        log_message(WARN, "Given fish move %s does not exist", l->element);
        return 1;
    }
    fish.vx = 0;
    fish.vy = 0;
    fish.delay = 1;
    fish.started = 1;

    if (Aquarium_addFish(&fish) != 0)
    {
        log_message(WARN, "Fish %s already exist", fish.name);
        return 1;
    }
    char const *message = "OK\n";
    write(fd, message, strlen(message));
    return 0;
}

int controller_delFish(struct line *l, int fd)
{
    if (l == NULL || l->nbelement != 1)
    {
        log_message(WARN, "Delete fish failed : Wrong arguments number");
        return 1;
    }

    Fish *f = Aquarium_findFishByName(l->element);
    Display *d = Aquarium_findDisplayByFd(fd);
    if (!Aquarium_isFishInDisplay(f, d))
    {
        return EXIT_FAILURE;
    }

    if (Aquarium_deleteFish(f) != 0)
    {
        log_message(WARN, "Delete fish failed : Fish %s does not exist", f->name);
        return 1;
    }
    char const *message = "OK\n";
    write(fd, message, strlen(message));
    return 0;
}

void startFish(void *f, void *args)
{
    if (f > (void *)0x10)
    {
        void **a = args;
        char *target_name = a[0];
        const char *name = Aquarium_getFishName(f);
        if (strcmp(name, target_name) == 0)
        {
            Aquarium_setFishStarted(f, 1);
        }
    }
    return;
}

int controller_startFish(struct line *l, int fd)
{
    void *args[] = {l->element};
    set__for_each(startFish, aq->fishList, args);
    char const *message = "OK\n";
    write(fd, message, strlen(message));
    return 0;
}

struct command command_list[NB_COMMANDS] = {
    {"getFishes", NULL, controller_getFishes},
    {"ls", NULL, controller_ls},
    {"getFishesContinuously", NULL, controller_getFishesContinuously},
    {"log", NULL, controller_logout},
    {"ping", NULL, controller_ping},
    {"addFish", NULL, controller_addFish},
    {"delFish", NULL, controller_delFish},
    {"startFish", NULL, controller_startFish},
};

void *loop_updateFishes(void *args)
{
    (void)(args);
    while (is_on != 0)
    {
        Aquarium_maj();
        sleep(1);
    }
    pthread_exit(NULL);
}

void init_controller()
{
    log_message(INFO, "Controller starting...");
    loop_fd = set__empty(fd_el__cmp, fd_el__copy, fd_el__del);
    is_on = 1;
    pthread_create(&thread_getFishesContinuously, NULL, loop_getFishContinuously, NULL);
    pthread_create(&thread_updateFishes, NULL, loop_updateFishes, NULL);
    srand(time(NULL));
    log_message(SUCCESS, "Controller started ! Ready to handle active connections.");
}

int answer(int fd)
{
    struct fd_el new_fd;
    new_fd.fd = fd;
    set__remove(loop_fd, &new_fd);

    char buffer[CONTROLLER_BUF_SIZE];
    struct command *my_command;
    exit_if(read(fd, buffer, CONTROLLER_BUF_SIZE) < 0, "Client reading failed");
    my_command = interpreter(buffer, " ", command_list, NB_COMMANDS);
    if (my_command == NULL || my_command->func(my_command->args, fd) != 0)
    {
        write(fd, "NOK", strlen("NOK"));
        if (my_command != NULL && my_command->args != NULL)
        {
            free_line(my_command->args);
        }
        return 1;
    }
    free_line(my_command->args);

    return 0;
}

void end_controller()
{
    is_on = 0;
    log_message(INFO, "Controller stopping...");
    pthread_join(thread_getFishesContinuously, NULL);
    pthread_join(thread_updateFishes, NULL);
    set__free(loop_fd);
    log_message(SUCCESS, "Controller ended !");
}