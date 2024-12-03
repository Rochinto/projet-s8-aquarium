#include "link.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define SET_LINK__NOT_PROCESS -1
#define SET_LINK__ERROR -2
#define SET_LINK__FOUND 1
#define SET_LINK__NOT_FOUND 0
#define SET_LINK__SEARCH_FAILED NULL

struct set
{
    struct link *l;
    int (*cmp)(const void *, const void *);
    void *(*copy)(const void *);
    void (*del)(void *);
};

struct set *set__empty(int (*cmp)(const void *, const void *), void *(*copy)(const void *), void (*del)(void *))
{
    struct set *new_set = malloc(sizeof(*new_set));
    new_set->l = malloc(sizeof(struct link));
    *(new_set->l) = lnk__empty();
    new_set->cmp = cmp;
    new_set->copy = copy;
    new_set->del = del;
    return new_set;
}

int set__is_empty(const struct set *se)
{
    return llm__is_end_mark(lnk__first(se->l));
}

int set__find(const struct set *se, void *c)
{
    struct lelement *el = lnk__first(se->l);
    while (!llm__is_end_mark(el) && se->cmp(el->data, c) <= 0)
    {
        if (se->cmp(el->data, c) == 0)
        {
            return SET_LINK__FOUND;
        }
        el = el->next;
    }
    return SET_LINK__NOT_FOUND;
}

void *set__search_by(const struct set *se, int(cond)(void *, void *), void *args)
{
    struct lelement *el = lnk__first(se->l);
    while (!llm__is_end_mark(el))
    {
        if (cond(el->data, args))
        {
            return el->data;
        }
        el = el->next;
    }
    return SET_LINK__SEARCH_FAILED;
}

int set__add(struct set *se, void *c)
{
    struct lelement *el = lnk__first(se->l);
    if (llm__is_end_mark(el) || se->cmp(c, el->data) < 0)
    {
        struct lelement *add = malloc(sizeof(struct lelement));
        add->data = se->copy(c);
        add->next = NULL;
        lnk__add_head(se->l, add);
        return 0;
    }
    while (!llm__is_end_mark(el))
    {
        if (se->cmp(el->data, c) == 0)
        {
            return SET_LINK__NOT_PROCESS;
        }
        else if (se->cmp(el->data, c) < 0 && (llm__is_end_mark(el->next) || se->cmp(c, el->next->data) < 0))
        {
            struct lelement *add = malloc(sizeof(struct lelement));
            add->data = se->copy(c);
            add->next = NULL;
            lnk__add_after(se->l, el, add);
            return 0;
        }
        el = el->next;
    }
    return SET_LINK__ERROR;
}

int set__remove(struct set *se, void *c)
{
    struct lelement *el = lnk__first(se->l);
    if (!llm__is_end_mark(el) && se->cmp(el->data, c) == 0)
    {
        struct lelement *el_to_rm = lnk__remove_head(se->l);
        se->del(el_to_rm->data);
        free(el_to_rm);
        return 0;
    }
    while (!llm__is_end_mark(el->next) && se->cmp(el->next->data, c) <= 0)
    {
        if (se->cmp(el->next->data, c) == 0)
        {
            struct lelement *el_to_rm = lnk__remove_after(se->l, el);
            se->del(el_to_rm->data);
            free(el_to_rm);
            return 0;
        }
        el = el->next;
    }
    return SET_LINK__NOT_PROCESS;
}

size_t set__size(const struct set *se)
{
    size_t size = 0;
    struct lelement *el = lnk__first(se->l);
    while (!llm__is_end_mark(el))
    {
        ++size;
        el = el->next;
    }
    return size;
}

struct set *set__filter(int (*filter)(void *, void *), const struct set *se, void *args)
{
    struct set *s_filtered = set__empty(se->cmp, se->copy, se->del);
    struct lelement *el = lnk__first(se->l);
    while (!llm__is_end_mark(el))
    {
        if (filter(el->data, args))
        {
            set__add(s_filtered, el->data);
        }
        el = el->next;
    }
    return s_filtered;
}

void set__for_each(void (*function)(void *, void *), struct set *se, void *args)
{
    struct lelement *el = lnk__first(se->l);
    while (!llm__is_end_mark(el))
    {
        function(el->data, args);
        el = el->next;
    }
}

void set__free(struct set *se)
{
    while (!set__is_empty(se))
    {
        struct lelement *el_to_free = lnk__remove_head(se->l);
        se->del(el_to_free->data);
        free(el_to_free);
    }
    free(se->l);
    free(se);
}