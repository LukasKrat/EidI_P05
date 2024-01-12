#ifndef EIDI_P05_PERSONLL_H
#define EIDI_P05_PERSONLL_H

typedef struct llNode {
    Person *data;
    struct llNode *next;
    struct llNode *prev;
} LLNode;

#endif //EIDI_P05_PERSONLL_H
