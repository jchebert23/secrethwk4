//IMPORTANT: I got this code for a stack from Professor Aspnes from the URL: http://www.cs.yale.edu/homes/aspnes/classes/223/notes.html#stacks

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>



struct elt {
    struct elt *next;
    int value;
};

typedef struct elt *Stack;

#define STACK_EMPTY (0)

/* push a new value onto top of stack */
void
stackPush(Stack *s, int value)
{
    struct elt *e;

    e = malloc(sizeof(struct elt));
    assert(e);

    e->value = value;
    e->next = *s;
    *s = e;
}

int
stackEmpty(const Stack *s)
{
    return (*s == 0);
}

int
stackPop(Stack *s)
{
    int ret;
    struct elt *e;

    assert(!stackEmpty(s));

    ret = (*s)->value;

    /* patch out first element */
    e = *s;
    *s = e->next;

    free(e);

    return ret;
}

