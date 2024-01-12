#ifndef EIDI_P05_PERSON_H
#define EIDI_P05_PERSON_H
#define MAX_NAME_LENGTH 20

typedef struct person {
    char vorname[MAX_NAME_LENGTH];
    char nachname[MAX_NAME_LENGTH];
} Person;

#endif //EIDI_P05_PERSON_H
