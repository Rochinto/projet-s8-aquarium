#include "prompt.h"
#include "logger.h"
#include <stdio.h>

int add_view(char *name, char *size)
{
    struct line *arg = create_line(size, "+");
    if (arg->nbelement != 3)
    {
        printf("NOK : arguments d'entrée invalide (format: add view <name> <int>x<int>+<int>+<int>)\n");
        return 1;
    }
    struct line *end = arg->next;
    arg = create_line(arg->element, "x");
    if (arg->nbelement != 2)
    {
        printf("NOK : arguments d'entrée invalide (format: add view <name> <int>x<int>+<int>+<int>)\n");
        return 1;
    }
    struct line *tmp = arg->next;
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = end;
    tmp = arg;
    int x = atoi(tmp->element);
    tmp = tmp->next;
    int y = atoi(tmp->element);
    tmp = tmp->next;

    int width = atoi(tmp->element);
    tmp = tmp->next;
    if (tmp == NULL)
    {
        printf("NOK : pas assez d'arguments\n");
        return 1;
    }
    int height = atoi(tmp->element);
    tmp = tmp->next;
    if (tmp != NULL)
    {
        printf("NOK : trop d'arguments\n");
        return 1;
    }
    if(x == 0 || y == 0 || width == 0 || height == 0)
    {
        printf("NOK : arguments d'entrée invalide (format: add view <name> <int>x<int>+<int>+<int>)\n");
        return 1;
    }
    Aquarium_createDisplay(name, x, y, width, height);
    return 0;
}

int del_view(char *n)
{
    Aquarium_delDisplayByName((void *)n);
    return 0;
}

void save_aq(char *name)
{
    char logprint[2000];
    Aquarium_saveAquarium(logprint);
    strcat(name,".txt");
    FILE * fichier = fopen(name, "w");
    if (fichier != NULL)
    {
        
        fputs(&logprint[6], fichier);
        fclose(fichier);
    }
}

int load(struct line *l, int fd)
{
    (void)fd;
    if (l == NULL)
    {
        log_message(CRITIC, "NOK : commande introuvable\n");
        return 1;
    }
    if (l->next != NULL)
    {
        log_message(CRITIC, "NOK : trop d'argument\n");
        return 1;
    }
    if (parser(l->element, 0) != 0)
    {
        return 1;
    }
    printf("aquarium loaded (%d display view)!\n", (int)set__size(aq->displayList));
    return 0;
}

int show(struct line *l, int fd)
{
    (void)fd;
    if (l == NULL || strcmp(l->element, "aquarium") != 0)
    {
        printf("NOK : commande introuvable\n");
        return 1;
    }
    l = l->next;
    if (l != NULL)
    {
        printf("NOK : trop d'argument\n");
        return 1;
    }
    if (aq == NULL)
    {
        printf("NOK : aucun aquarium chargé\n");
        return 1;
    }
    Aquarium_printAquarium(aq);
    return 0;
}

int add(struct line *l, int fd)
{
    (void)fd;
    if (l == NULL || strcmp(l->element, "view") != 0)
    {
        printf("NOK : commande introuvable\n");
        return 1;
    }
    l = l->next;
    if (l == NULL)
    {
        printf("NOK : commande introuvable\n");
        return 1;
    }
    char *view = l->element;
    l = l->next;
    if (l == NULL)
    {
        printf("NOK : commande introuvable\n");
        return 1;
    }
    char *size = l->element;
    l = l->next;
    if (l != NULL)
    {
        printf("NOK : trop d'argument\n");
        return 1;
    }
    if (add_view(view, size) == 1)
    {
        return 1;
    }
    printf("view added\n");
    return 0;
}

int del(struct line *l, int fd)
{
    (void)fd;
    if (l == NULL || strcmp(l->element, "view") != 0)
    {
        printf("NOK : commande introuvable\n");
        return 1;
    }
    l = l->next;
    if (l == NULL)
    {
        printf("NOK : commande introuvable\n");
        return 1;
    }
    char *view = l->element;
    l = l->next;
    if (l != NULL)
    {
        printf("NOK : trop d'argument\n");
        return 1;
    }
    if (del_view(view) == 1)
    {
        return 1;
    }
    printf("view %s deleted.\n", view);
    return 0;
}

int save(struct line *l, int fd)
{
    (void)fd;
    if (l == NULL)
    {
        printf("NOK : commande introuvable\n");
        return 1;
    }
    char *n = l->element;
    l = l->next;
    if (l != NULL)
    {
        printf("NOK : trop d'argument\n");
        return 1;
    }
    save_aq(n);
    printf("Aquarium saved ! ( %d display view)\n", (int)set__size(aq->displayList));
    return 0;
}

int end(struct line *l, int fd)
{
    (void)l;
    (void)fd;
    if (aq != NULL)
    {
        Aquarium_del(aq);
    }
    log_message(INFO, "Exiting prompt...");
    return -1;
}

int lessive(struct line *l, int fd)
{
    (void)l;
    (void)fd;
    Aquarium_lessive();
  printf("Oh you did this !!??? Shame on you...\n");
 return 0;
}

struct command command_list_prompt[7] = {
    {"load", NULL, load},
    {"show", NULL, show},
    {"add", NULL, add},
    {"del", NULL, del},
    {"save", NULL, save},
    {"exit", NULL, end},
    {"lessive", NULL, lessive}};

int prompt()
{
    aq = Aquarium_get();
    char buffer[1024];
    int listen = 1;
    struct command *my_command;
    while (listen != -1)
    {
        fflush(stdout);
        printf("$ ");
        scanf("%[^\n]", buffer);
        fgetc(stdin);
        my_command = interpreter(buffer, " ", command_list_prompt, 7);
        if (my_command == NULL)
        {
            printf("NOK\n");
            listen = 1;
        }
        else
        {
            listen = my_command->func(my_command->args, STDOUT_FILENO);
            free_line(my_command->args);
        }
    }
    puts("Good Bye !");
    log_message(SUCCESS, "Prompt ended !");
    return 0;
}