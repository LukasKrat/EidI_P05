#include "person.h"

struct llNode {
    void *data;
    struct llNode *next;
    struct llNode *prev;
};