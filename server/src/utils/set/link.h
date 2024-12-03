#ifndef __LINK_H__
#define __LINK_H__

#define UNUSED(x) (void)(x)

#define LNK_SUCCES 0
#define LNK_ERROR_BAD_LELEMENT (-1)
#define LNK_BAD__LELEMENT (NULL)
struct link {
    struct lelement* head;
};

struct lelement {
    void* data;
    struct lelement* next;
};

// Retourne un booléen attestant si `e` est la butée
int llm__is_end_mark(const struct lelement* e);

// Retourne le maillon suivant le maillon `e`
struct lelement* llm__next(const struct lelement* e);

// Crée une liste vide
// Post : le champ `head` pointe sur la butée
struct link lnk__empty(void);

// Retourne le 1er maillon de la liste `l`
struct lelement* lnk__first(const struct link* l);

// Ajoute le maillon `add` en tête de la liste `l`
// Pre : add->next == NULL,
//       add n'est pas la butée
int lnk__add_head(struct link* l, struct lelement* add);

// Enlève le premier maillon de la liste `l` et le retourne
// Si la liste est vide, renvoie NULL.
// Post : return->next == NULL || return == NULL
struct lelement* lnk__remove_head(struct link* l);

// Ajoute le maillon `add` à la suite du maillon `after` dans
// la liste `l`
// Rq : `l` peut ne pas être utilisé dans cette fonction.
// Pre : after appartient à la liste (non-vérifié)
//       after n'est pas la butée,
//       add n'est pas la butée,
//       after->next != NULL,
//       add->next == NULL
int lnk__add_after(struct link* l, struct lelement* after,
    struct lelement* add);

// Enlève le maillon se trouvant *après* `after` dans la liste `l` et
// le retourne. Renvoie NULL si le maillon après `after` est la butée.
// Pre  : after n'est pas la butée
//        after->next != NUL
//        after->next n'est pas la butée
// Post : return->next == NULL || return == NULL
struct lelement* lnk__remove_after(struct link* l, struct lelement* after);

#endif /* __LINK_H__ */
