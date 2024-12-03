#include <assert.h>
#include <limits.h>
#include <stdlib.h>

#include "link.h"

static const struct lelement sentinel = { (void*)0xdeadbeef,
    (struct lelement*)&sentinel };
int llm__is_end_mark(const struct lelement* e);

#define SENTINEL ((struct lelement*)&sentinel)

int llm__is_end_mark(const struct lelement* e)
{
    return e == SENTINEL;
}

struct lelement* llm__next(const struct lelement* e)
{
    return e->next;
}

struct link lnk__empty(void)
{
    struct link res;
    res.head = SENTINEL;
    return res;
}

struct lelement* lnk__first(const struct link* l)
{
    return l->head;
}

int lnk__add_head(struct link* l, struct lelement* add)
{
    if (llm__next(add) != NULL || llm__is_end_mark(add)) {
        return LNK_ERROR_BAD_LELEMENT;
    }
    add->next = l->head;
    l->head = add;
    return LNK_SUCCES;
}

struct lelement* lnk__remove_head(struct link* l)
{
    if (llm__is_end_mark(l->head)) {
        return LNK_BAD__LELEMENT;
    }
    struct lelement* res = l->head;
    l->head = res->next;
    res->next = NULL;
    return res;
}

int lnk__add_after(struct link* l, struct lelement* after, struct lelement* add)
{
    UNUSED(l);
    if (llm__next(add) != NULL || llm__is_end_mark(add) || llm__next(after) == NULL || llm__is_end_mark(after)) {
        return LNK_ERROR_BAD_LELEMENT;
    }
    add->next = after->next;
    after->next = add;
    return LNK_SUCCES;
}

struct lelement* lnk__remove_after(struct link* l, struct lelement* after)
{
    UNUSED(l);
    if (llm__next(after) == NULL || llm__is_end_mark(after) || llm__is_end_mark(after->next)) {
        return LNK_BAD__LELEMENT;
    }
    struct lelement* res = after->next;
    after->next = res->next;
    res->next = NULL;
    return res;
}
