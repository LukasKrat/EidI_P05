#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "person.h"
#include "personLL.h"

#define MAX_TITLE_LENGTH 20


/**
 * @brief appointment{time_t start, int duration, char title[20]}
 * 
 */
struct appointment
{
    long long int id;
    time_t start;
    int duration;
    char title[MAX_TITLE_LENGTH];
    LLNode *personenHead;
};

/**
 * @brief Get the User Input Main Menu object
 * 
 * @return int 
 */
int getUserInputMainMenu() {
    int input;
    printf("1. Termin anlegen\n"
           "2. Terminserie anlegen\n"
           "3. Termin loeschen\n"
           "4. Alle termine anzeigen\n"
           "5. Naechsten Termin anzeigen\n"
           "6. Kalenderausgabe\n"
           "7. Termin bearbeiten\n"
           "8. Person verwalten\n"
           "9. In Datei exportieren\n"
           "10. Programm beenden \n");
    
    scanf("%d", &input);
    #if WIN32 || WIN64
        fflush(stdin);
    #else
        getchar();
    #endif
    return input;

}

/**
 * @brief Get the User Input Appointment Type object
 * 
 * @return int 
 */
int getUserInputAppointmentType() {
    int input;
    printf("1. Täglicher Termin\n"
           "2. Wöchentlicher Termin\n"
           "3. Zwei-Wöchentlicher Termin\n");
    
    scanf("%d", &input);
    #if WIN32 || WIN64
        fflush(stdin);
    #else
        getchar();
    #endif
    return input;

}

long long int getUserInputEditAppointmentSelectAppointment() {
    long long int input;
    printf("Termin-ID eingeben:\n");

    scanf("%lld", &input);
#if WIN32 || WIN64
    fflush(stdin);
#else
    getchar();
#endif
    return input;
}

Person getUserInputPersonCreate() {
    char vorname[MAX_NAME_LENGTH] = "";
    char nachName[MAX_NAME_LENGTH] = "";

    puts("Bitte den Vornamen eingeben:\n");
    scanf("%20s", vorname);

#if WIN32 || WIN64
    fflush(stdin);
#else
    getchar();
#endif

    puts("Bitte den Nachnamen eingeben:\n");
    scanf("%20s", nachName);

#if WIN32 || WIN64
    fflush(stdin);
#else
    getchar();
#endif

    Person newPerson;
    strncpy(newPerson.vorname, vorname, 20);
    strncpy(newPerson.nachname, nachName, 20);

    return newPerson;
}

int getUserInputPersonMenu() {
    int input;
    printf("1. Person hinzufügen\n"
           "2. Person entfernen\n"
           "3. Personen anzeigen\n"
           "4. Abbrechen\n");

    scanf("%d", &input);
#if WIN32 || WIN64
    fflush(stdin);
#else
    getchar();
#endif
    return input;
}

/**
 * @brief Get the Week object
 * 
 * @param date 
 * @return int 
 */
int getWeek(const struct tm *date) {
    // Benutze strftime, um die Kalenderwoche zu extrahieren
    char weekBuffer[3];  // Der Puffer für die Kalenderwoche (bis zu zwei Stellen + Nullterminator)
    strftime(weekBuffer, sizeof(weekBuffer), "%V", date);

    // Konvertiere die Kalenderwoche von String zu Integer
    int calendarWeek = atoi(weekBuffer);

    return calendarWeek;
}

/**
 * @brief 
 * 
 * @param day 
 * @param month 
 * @param year 
 * @return int 
 */
int isValidDate(int day, int month, int year) {
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 0) {
        printf("Ungültiges Datum eingegeben. Tag sollte zwischen 1 und 31, Monat zwischen 1 und 12 und Jahr größer als 1900 sein.\n");
        return 0;
    }
    return 1;
}

/**
 * @brief 
 * 
 * @param appointments 
 * @param countAppointments 
 * @return struct appointment* 
 */
struct appointment *findNextAppointment(struct appointment *appointments, int countAppointments)
{
    time_t currentTime = time(NULL);

    struct appointment *nextAppointment = NULL;

    for (int i = 0; i < countAppointments; i++)
    {
        if (appointments[i].start > currentTime && (nextAppointment == NULL || appointments[i].start < nextAppointment->start))
        {
            nextAppointment = &appointments[i];
        }
    }

    return nextAppointment;
}

struct appointment *findAppointmentById(int appointmentsCount, struct appointment *pAppointment, long long int id) {
    for (int i = 0; i < appointmentsCount; i++) {
        if (pAppointment[i].id == id) return &pAppointment[i];
    }

    return NULL;
}

/**
 * @brief Create a Single Appointment object
 * 
 * @param appointments 
 * @param countAppointments 
 * @param day 
 * @param month 
 * @param year 
 * @param hour 
 * @param minute 
 * @param durationHours 
 * @param durationMinutes 
 * @param title 
 */
void createSingleAppointment(struct appointment **appointments, int *countAppointments, int day, int month, int year, int hour, int minute, int durationHours, int durationMinutes, const char *title)
{   
    

    // Allocate memory for the new appointment
    *appointments = (struct appointment *)realloc(*appointments, (*countAppointments + 1) * sizeof(struct appointment));

    if (*appointments == NULL)
    {
        fprintf(stderr, "Speicherplatz konnte nicht reserviert werden.\n");
        exit(1);
    }

    struct tm appointmentTime = {0};
    appointmentTime.tm_year = year - 1900;
    appointmentTime.tm_mon = month - 1;
    appointmentTime.tm_mday = day;
    appointmentTime.tm_hour = hour;
    appointmentTime.tm_min = minute;

    for (int i = 0; i < *countAppointments; ++i)
    {
        time_t existingStart = (*appointments)[i].start;
        int existingDuration = (*appointments)[i].duration;

        if (
            ((existingStart <= mktime(&appointmentTime) && existingStart + existingDuration >= mktime(&appointmentTime)) ||
             (existingStart >= mktime(&appointmentTime) && existingStart <= mktime(&appointmentTime) + durationHours * 3600 + durationMinutes * 60)) ||
            ((existingStart >= mktime(&appointmentTime) && existingStart < mktime(&appointmentTime) + 3600) ||
             (existingStart + existingDuration >= mktime(&appointmentTime) && existingStart + existingDuration < mktime(&appointmentTime) + 3600)))
        {
            printf("Warnung! Der Termin am %02d.%02d.%04d um %02d:%02d mit dem Titel \"%s\" hat eine Überschneidung und wird im Kalender nicht angezeigt!\n",day, month, year, hour, minute, title);
        } 
    }

    // Set the start time using mktime
    (*appointments)[*countAppointments].start = mktime(&appointmentTime);

    // Set the id
    (*appointments)[*countAppointments].id = ((long long int) (&(*appointments)[*countAppointments])) + *countAppointments;

    // Set personenHead to NULL
    (*appointments)[*countAppointments].personenHead = NULL;

    // Set the duration
    (*appointments)[*countAppointments].duration = durationHours * 3600 + durationMinutes * 60;

    // Copy the title manually without strncpy
    int i;
    for (i = 0; i < MAX_TITLE_LENGTH - 1 && title[i] != '\0'; ++i)
    {
        (*appointments)[*countAppointments].title[i] = title[i];
    }
    (*appointments)[*countAppointments].title[i] = '\0'; // Nullterminator hinzufügen

    // Increment the count of appointments
    (*countAppointments)++;
}

/**
 * @brief Get the Weekday Name object
 * 
 * @param dayIndex 
 * @return const char* 
 */
const char *getWeekdayName(int dayIndex) {
    switch (dayIndex) {
        case 0: return "Montag";
        case 1: return "Dienstag";
        case 2: return "Mittwoch";
        case 3: return "Donnerstag";
        case 4: return "Freitag";
        case 5: return "Samstag";
        case 6: return "Sonntag";
        default: return "Ungültig";
    }
}

int compareAppointments(const void *a, const void *b) {
    const struct appointment *appointmentA = (const struct appointment *)a;
    const struct appointment *appointmentB = (const struct appointment *)b;

    if (appointmentA->start < appointmentB->start) return -1;
    if (appointmentA->start > appointmentB->start) return 1;

    return 0; // Appointments are equal
}

int checkPersonExistsInAppointment(struct appointment *appointment, Person person) {
    LLNode* temp = appointment->personenHead;

    do {
        if (strcmp(temp->data->vorname, person.vorname) == 0 && strcmp(temp->data->nachname, person.nachname) == 0) { return 1; }
        temp = temp->next;
    }
    while (temp != NULL);

    return 0;
}

// #### Start Person-management
/**
 * @brief Append a person to an appointment.
 * @param appointment
 * @param person
 */
void appendPersonToAppointment(struct appointment *appointment, Person person) {
    // allocate memory for node
    LLNode* newNode = (LLNode*)malloc(sizeof(LLNode));

    // assign data to newNode
    newNode->data = malloc(sizeof (Person));
    memcpy(newNode->data, &person, sizeof (Person));

    // assign NULL to next of newNode
    newNode->next = NULL;

    appointment->personenHead->data;

    // store the head node temporarily (for later use)
    LLNode* temp = appointment->personenHead;

    // if the linked list is empty, make the newNode as head node
    if (appointment->personenHead == NULL) {
        newNode->prev = NULL;
        appointment->personenHead = newNode;
        return;
    }

    int isNewPersonAlreadyInAppointment = checkPersonExistsInAppointment(appointment, person);

    if (isNewPersonAlreadyInAppointment == 1) return;

    // if the linked list is not empty, traverse to the end of the linked list
    while (temp->next != NULL)
        temp = temp->next;

    // now, the last node of the linked list is temp

    // point the next of the last node (temp) to newNode.
    temp->next = newNode;

    // assign prev of newNode to temp
    newNode->prev = temp;
}

/**
 *
 * @param strlenA
 * @param a
 * @param strlenB
 * @param b
 * @return 0 in case the order is identical; 1 in case b comes after a; -1 in case a comes after b.
 */
int compareAlphabeticRank(int strlenA, char a[strlenA], int strlenB, char b[strlenB]) {
    /*
    char base[strlenA < strlenB ? strlenA : strlenB];
    int baseLength = strlenA < strlenB ? strlenA : strlenB;

    char cmp[strlenA < strlenB ? strlenB : strlenA];
    int cmpLength = strlenA < strlenB ? strlenB : strlenA;

    if (strlenA < strlenB) {
        strncpy(base, a, strlenA);
        strncpy(cmp, b, strlenB);
    }
    else {
        strncpy(base, b, strlenB);
        strncpy(cmp, a, strlenA);
    }
     */

    for (int i = 0; i < (strlenA > strlenB ? strlenB : strlenA); i++) {
        if (a[i] == b[i]) continue;
        else if (a[i] < b[i]) return 1;
        else if (a[i] > b[i]) return -1;
    }

    return 0;
}

void insertPersonToAppointmentAlphabetically(struct appointment *appointment, Person person) {
    // allocate memory for node
    LLNode* newNode = (LLNode*)malloc(sizeof(LLNode));

    // assign data to newNode
    newNode->data = malloc(sizeof (Person));
    memcpy(newNode->data, &person, sizeof (Person));

    // assign NULL to next of newNode
    newNode->next = NULL;

    appointment->personenHead->data;

    // store the head node temporarily (for later use)
    LLNode* currentNodeCursor = appointment->personenHead;

    // if the linked list is empty, make the newNode as head node
    if (appointment->personenHead == NULL) {
        newNode->prev = NULL;
        appointment->personenHead = newNode;
        return;
    }

    int isNewPersonAlreadyInAppointment = checkPersonExistsInAppointment(appointment, person);

    if (isNewPersonAlreadyInAppointment == 1) return;

    // if the linked list is not empty, traverse to the end of the linked list
    /*
    while (temp->next != NULL)
        temp = temp->next;
    */

    while (currentNodeCursor->next != NULL && compareAlphabeticRank(MAX_NAME_LENGTH, currentNodeCursor->data->nachname, MAX_NAME_LENGTH, person.nachname) >= 0) {
        currentNodeCursor = currentNodeCursor->next;
    }

    if (currentNodeCursor->prev != NULL)
    {
        currentNodeCursor = currentNodeCursor->prev;

        // currentNodeCursor now contains the value which should be BEFORE person.

        // set next of newNode to next of prev node
        newNode->next = currentNodeCursor->next;

        // set next of prev node to newNode
        currentNodeCursor->next = newNode;

        // set prev of newNode to the previous node
        newNode->prev = currentNodeCursor;

        // set prev of newNode's next to newNode
        if (newNode->next != NULL)
            newNode->next->prev = newNode;
    }
    else {
        currentNodeCursor->prev = newNode;
        newNode->prev = NULL;
        newNode->next = currentNodeCursor;

        appointment->personenHead = newNode;
    }
}

void removePersonFromAppointment(struct appointment *appointment, Person person) {
    appointment->personenHead->data;

    // store the head node temporarily (for later use)
    LLNode* currentNodeCursor = appointment->personenHead;

    // if the linked list is empty, make the newNode as head node
    if (appointment->personenHead == NULL) {
        return;
    }

    int isNewPersonAlreadyInAppointment = checkPersonExistsInAppointment(appointment, person);

    if (isNewPersonAlreadyInAppointment == 0) return;

    // if the linked list is not empty, traverse to the end of the linked list
    /*
    while (temp->next != NULL)
        temp = temp->next;
    */

    while (currentNodeCursor->next != NULL && (strcmp(currentNodeCursor->data->nachname, person.nachname) != 0 && strcmp(currentNodeCursor->data->vorname, person.vorname) != 0)) {
        currentNodeCursor = currentNodeCursor->next;
    }

    if (appointment->personenHead == currentNodeCursor) {
        appointment->personenHead = currentNodeCursor->next;
    }
    else if (currentNodeCursor->next != NULL) {
        currentNodeCursor->next->prev = currentNodeCursor->prev;
    }
    else if (currentNodeCursor->prev != NULL)
    {
        currentNodeCursor->prev->next = currentNodeCursor->next;
    }

    free(currentNodeCursor);
}

// #### End Person-management

int main(void) {

    struct appointment *appointments = NULL; // Dynamisches Array für Termine
    int countAppointments = 0, userInputMainMenu, userInputAppointmentType;

    while (1)
    {
        userInputMainMenu = getUserInputMainMenu();

        switch (userInputMainMenu)
        {
        case 1:
            {
                int day, month, year, hour, minute, durationHours, durationMinutes;
                char title[MAX_TITLE_LENGTH];

                printf("Startzeit: (Tag Monat Jahr): ");
                scanf("%d %d %d", &day, &month, &year);

                if (!isValidDate(day, month, year)) {
                    break;
                }

                printf("Uhrzeit (Stunde Minute): ");
                scanf("%d %d", &hour, &minute);

                if(hour < 0 || hour > 23 || minute < 0 || minute > 60) {
                    printf("Uhrzeit muss zwischen 0:00 Uhr und 23:59 liegen!\n");
                    break;
                }

                printf("Dauer (Stunden Minuten): ");
                scanf("%d %d", &durationHours, &durationMinutes);

                if (durationHours > 8){
                    printf("Dauer darf maximal 8 Studen betragen!\n");
                    break;
                }

                printf("Titel (max %d Zeichen): ", MAX_TITLE_LENGTH);
                scanf("%s", title);

                createSingleAppointment(&appointments, &countAppointments, day, month, year, hour, minute, durationHours, durationMinutes, title);
                break;

            }
        case 2: {
            userInputAppointmentType = getUserInputAppointmentType();
            switch (userInputAppointmentType) {
                case 1:
                    /* Täglicher Termin */
                {

                    int appointmentSeriesAmount = 0;
                    printf("Anzahl der Termine in der Serie: ");
                    scanf("%d", &appointmentSeriesAmount);

                    if (appointmentSeriesAmount > 1000) {
                        printf("Zu viele Termine bitte nutze eine Anzahl unter 1000");
                        break;
                    }


                    int day, month, year, hour, minute;
                    printf("Erster Termin der Serie: (Tag Monat Jahr): ");
                    scanf("%d %d %d", &day, &month, &year);

                    if (!isValidDate(day, month, year)) {
                        break;
                    }

                    printf("Uhrzeit (Stunde Minute): ");
                    scanf("%d %d", &hour, &minute);

                    if (hour < 0 || hour > 23 || minute < 0 || minute > 60) {
                        printf("Uhrzeit muss zwischen 0:00 Uhr und 23:59 liegen!\n");
                        break;
                    }

                    printf("Dauer (Stunden Minuten): ");
                    int durationHours, durationMinutes;
                    scanf("%d %d", &durationHours, &durationMinutes);

                    if (durationMinutes > 60) {
                        printf("Minuten Dauer darf maximal 60 betragen!\n");
                        break;
                    }

                    if (durationHours > 8) {
                        printf("Dauer darf maximal 8 Studen betragen!\n");
                        break;
                    }


                    printf("Titel (max %d Zeichen): ", MAX_TITLE_LENGTH);
                    char title[MAX_TITLE_LENGTH];
                    scanf("%s", title);


                    for (int i = 0; i < appointmentSeriesAmount; i++) {

                        createSingleAppointment(&appointments, &countAppointments, day + i, month, year, hour,
                                                minute, durationHours, durationMinutes, title);

                    }
                }

                    break;

                case 2:
                    /* Wöchentlicher Termin */
                {
                    int appointmentSeriesAmount = 0;
                    printf("Anzahl der Termine in der Serie: ");
                    scanf("%d", &appointmentSeriesAmount);

                    if (appointmentSeriesAmount > 1000) {
                        printf("Zu viele Termine bitte nutze eine Anzahl unter 1000");
                        break;
                    }


                    int day, month, year, hour, minute;
                    printf("Erster Termin der Serie: (Tag Monat Jahr): ");
                    scanf("%d %d %d", &day, &month, &year);

                    if (!isValidDate(day, month, year)) {
                        break;
                    }

                    printf("Uhrzeit (Stunde Minute): ");
                    scanf("%d %d", &hour, &minute);

                    if (hour < 0 || hour > 23 || minute < 0 || minute > 60) {
                        printf("Uhrzeit muss zwischen 0:00 Uhr und 23:59 liegen!\n");
                        break;
                    }

                    printf("Dauer (Stunden Minuten): ");
                    int durationHours, durationMinutes;
                    scanf("%d %d", &durationHours, &durationMinutes);

                    if (durationMinutes > 60) {
                        printf("Minuten Dauer darf maximal 60 betragen!\n");
                        break;
                    }

                    if (durationHours > 8) {
                        printf("Dauer darf maximal 8 Studen betragen!\n");
                        break;
                    }

                    printf("Titel (max %d Zeichen): ", MAX_TITLE_LENGTH);
                    char title[MAX_TITLE_LENGTH];
                    scanf("%s", title);


                    for (int i = 0; i < appointmentSeriesAmount; i++) {

                        createSingleAppointment(&appointments, &countAppointments, day + (i * 7), month, year, hour,
                                                minute, durationHours, durationMinutes, title);

                    }
                }
                    break;

                case 3:
                    /* 2-Wöchentlicher Termin */

                {
                    int appointmentSeriesAmount = 0;
                    printf("Anzahl der Termine in der Serie: ");
                    scanf("%d", &appointmentSeriesAmount);

                    if (appointmentSeriesAmount > 1000) {
                        printf("Zu viele Termine bitte nutze eine Anzahl unter 1000");
                        break;
                    }


                    int day, month, year, hour, minute;
                    printf("Erster Termin der Serie: (Tag Monat Jahr): ");
                    scanf("%d %d %d", &day, &month, &year);

                    if (!isValidDate(day, month, year)) {
                        break;
                    }

                    printf("Uhrzeit (Stunde Minute): ");
                    scanf("%d %d", &hour, &minute);

                    if (hour < 0 || hour > 23 || minute < 0 || minute > 60) {
                        printf("Uhrzeit muss zwischen 0:00 Uhr und 23:59 liegen!\n");
                        break;
                    }

                    printf("Dauer (Stunden Minuten): ");
                    int durationHours, durationMinutes;
                    scanf("%d %d", &durationHours, &durationMinutes);
                    if (durationMinutes > 60) {
                        printf("Minuten Dauer darf maximal 60 betragen!\n");
                        break;
                    }


                    if (durationHours > 8) {
                        printf("Dauer darf maximal 8 Studen betragen!\n");
                        break;
                    }

                    printf("Titel (max %d Zeichen): ", MAX_TITLE_LENGTH);
                    char title[MAX_TITLE_LENGTH];
                    scanf("%s", title);


                    for (int i = 0; i < appointmentSeriesAmount; i++) {

                        createSingleAppointment(&appointments, &countAppointments, day + (i * 14), month, year,
                                                hour, minute, durationHours, durationMinutes, title);

                    }
                }
                    break;
                default:
                    puts("Ungueltige Eingabe!");
                    break;
            }
        }
            break;
        case 3:
            {
                int day=0,month=0,year=0;
                printf("Bitte geben Sie das Datum des Termins ein (Tag Monat Jahr): ");
                scanf("%d %d %d", &day, &month, &year);

                 // Initialisiere ein struct tm-Objekt
                struct tm date = {0};
                date.tm_mday = day;
                date.tm_mon = month - 1; // Monate in der Struktur sind von 0 bis 11
                date.tm_year = year - 1900;
                

                int weekNumber = getWeek(&date);

                printf("Die Kalenderwoche für den %02d.%02d.%d ist: %d\n", day, month, year, weekNumber);

                
            }
            break;
        
        case 4:
            if (countAppointments == 0) {
                printf("Es sind keine Termine vorhanden.\n");
            } else {
                printf("Alle Termine:\n");
                for (int i = 0; i < countAppointments; i++) {
                    printf("Termin %d\n", i + 1);

                    printf("Id: %lld\n", appointments[i].id);

                    // Zeit in struct tm-Format umwandeln
                    struct tm *localTime = localtime(&appointments[i].start);
                    
                    // Ausgabe der Startzeit
                    printf("Startzeit: %d.%d.%d %02d:%02d\n",
                        localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900,
                        localTime->tm_hour, localTime->tm_min);
                    
                    // Dauer in Stunden und Minuten umrechnen und ausgeben
                    int durationHours = appointments[i].duration / 3600;
                    int durationMinutes = (appointments[i].duration % 3600) / 60;
                    printf("Dauer: %d Stunden %d Minuten\n", durationHours, durationMinutes);


                    printf("Titel: %s\n", appointments[i].title);
                    puts("Teilnehmer:innen:");

                    LLNode* temp = appointments[i].personenHead;

                    if (temp != NULL) {
                        // if the linked list is not empty, traverse to the end of the linked list
                        do
                        {
                            printf("%s, %s\n", temp->data->nachname, temp->data->vorname);
                            temp = temp->next;
                        }
                        while (temp != NULL);
                    }

                    printf("\n\n");
                }
            }
            break;

        case 5:
            if (countAppointments == 0)
            {
                printf("Es sind keine Termine vorhanden.\n");
            }
            else
            {
                struct appointment *nextAppointment = findNextAppointment(appointments, countAppointments);

                if (nextAppointment != NULL)
                {
                    struct tm *localTime = localtime(&nextAppointment->start);
                    printf("Nächster Termin:\n");
                    printf("Startzeit: %d.%d.%d %02d:%02d\n",
                           localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900,
                           localTime->tm_hour, localTime->tm_min);

                    int durationHours = nextAppointment->duration / 3600;
                    int durationMinutes = (nextAppointment->duration % 3600) / 60;
                    printf("Dauer: %d Stunden %d Minuten\n", durationHours, durationMinutes);

                    printf("Titel: %s\n", nextAppointment->title);
                }
                else
                {
                    printf("Es gibt keine zukünftigen Termine.\n");
                }
            }
            break;


        case 6:
        {
            /* Kalenderausgabe */
            printf("Kalenderausgabe\n");

            if (countAppointments == 0) {
                printf("Es sind keine Termine vorhanden.\n");
            } else {
                int year, week;
                printf("Bitte geben Sie das Jahr und die Kalenderwoche ein (im Format JJJJ WW): ");
                scanf("%d %d", &year, &week);
                getchar();

                struct tm timeinfo = {0};
                timeinfo.tm_year = year - 1900;
                timeinfo.tm_mon = 0; // Januar
                timeinfo.tm_mday = 1; // Erster Tag des Monats
                timeinfo.tm_isdst = -1; // Automatische Sommerzeit-Erkennung

                // Berechnung des ersten Wochentags im Jahr und der Kalenderwoche
                time_t startOfWeek = mktime(&timeinfo);
                startOfWeek += (week - 1) * 7 * 24 * 60 * 60;

                for (int j = 0; j < 7; j++) {
                    printf("|%-*s", MAX_TITLE_LENGTH, getWeekdayName(j)); // Hier sollte die Funktion getWeekdayName() implementiert sein
                }
                printf("\n");

                for (int i = 0; i < 24; i++) {
                    // Speichern der aktuellen Stunde
                    int hasAppointmentInHour = 0;

                    for (int j = 0; j < 7; j++) {
                        int currentDayIndex = (j + 1) % 7;

                        time_t currentDay = startOfWeek + currentDayIndex * 24 * 60 * 60 + i * 60 * 60;

                        // Überprüfung, ob ein Termin für den aktuellen Tag und die aktuelle Uhrzeit existiert
                        int hasAppointment = 0;
                        for (int k = 0; k < countAppointments; k++) {
                            struct tm *localTime = localtime(&appointments[k].start);
                            int appointmentWeek = getWeek(localTime);

                            // Überprüfe, ob der Wochentag, die Uhrzeit und die Kalenderwoche übereinstimmen
                            if (localTime->tm_wday == currentDayIndex && localTime->tm_hour == i && appointmentWeek == week) {
                                hasAppointment = 1;
                                hasAppointmentInHour = 1;
                                printf("|%-*s", MAX_TITLE_LENGTH, appointments[k].title);
                                break;
                            }
                        }

                        // Wenn kein Termin existiert, fülle mit einem Strich
                        if (!hasAppointment) {
                            printf("|%-*s", MAX_TITLE_LENGTH, "");
                        }
                    }

                    printf("\n");

                    // Print times for each day
                    for (int j = 0; j < 7; j++) {
                        int currentDayIndex = (j + 1) % 7;

                        // Überprüfe, ob in der Stunde an diesem Wochentag ein Termin existiert
                        int hasAppointmentInHourAndDay = 0;

                        // Find and print times for the current day and selected week
                        for (int k = 0; k < countAppointments; k++) {
                            struct tm *localTime = localtime(&appointments[k].start);
                            int appointmentWeek = getWeek(localTime);

                            // Check if the appointment is for the current day, selected week, and the same hour
                            if (localTime->tm_wday == currentDayIndex && appointmentWeek == week && localTime->tm_hour == i) {
                                hasAppointmentInHourAndDay = 1;

                                // Print Startzeit und Endzeit direkt unter den Termin
                                int timeWidth = MAX_TITLE_LENGTH;
                                printf("|(%02d:%02d-%02d:%02d)%*s", 
                                    localTime->tm_hour, localTime->tm_min,
                                    (localTime->tm_hour + appointments[k].duration / 3600),
                                    (localTime->tm_min + (appointments[k].duration % 3600) / 60),
                                    timeWidth - 13, "");  // 11 ist die Länge von "hh:mm-hh:mm"
                                break; 
                            }
                        }

                        // Wenn kein Termin existiert, fülle mit einem Strich
                        if (!hasAppointmentInHourAndDay) {
                            printf("|%-*s", MAX_TITLE_LENGTH, "");
                        }
                    }
                    printf("\n");
                }
            }
        }
        break;

        case 7: {
            long long int editAppointmentSelectionId = getUserInputEditAppointmentSelectAppointment();

            int personenManagementMenuSelection = getUserInputPersonMenu();

            switch (personenManagementMenuSelection) {
                case 1: {
                    Person newPerson = getUserInputPersonCreate();

                    struct appointment *selectedAppointment = findAppointmentById(countAppointments, appointments, editAppointmentSelectionId);

                    if (selectedAppointment == NULL) break;

                    insertPersonToAppointmentAlphabetically(selectedAppointment, newPerson);

                    break;
                }
                case 2: {
                    {
                        Person newPerson = getUserInputPersonCreate();

                        struct appointment *selectedAppointment = findAppointmentById(countAppointments, appointments, editAppointmentSelectionId);

                        if (selectedAppointment == NULL) break;

                        removePersonFromAppointment(selectedAppointment, newPerson);

                        break;
                    }
                }
                case 3: {
                    struct appointment *selectedAppointment = findAppointmentById(countAppointments, appointments, editAppointmentSelectionId);

                    if (selectedAppointment == NULL) break;

                    // store the head node temporarily (for later use)
                    LLNode* temp = selectedAppointment->personenHead;

                    // if the linked list is not empty, traverse to the end of the linked list
                    do
                    {
                        printf("%s, %s\n", temp->data->nachname, temp->data->vorname);
                        temp = temp->next;
                    }
                    while (temp != NULL);

                    break;
                }
                case 4: {
                    break;
                }
                default: {
                    break;
                }
            }

            break;
        }
        case 8: {
            Person personRequested = getUserInputPersonCreate();

            for (int i = 0; i < countAppointments; i++) {
                LLNode* temp = appointments[i].personenHead;

                if (temp == NULL || temp->data == NULL) continue;

                do {

                    if (strcmp(temp->data->vorname, personRequested.vorname) == 0 && strcmp(temp->data->nachname, personRequested.nachname) == 0) {
                        printf("Termin %d\n", i + 1);

                        printf("Id: %lld\n", appointments[i].id);

                        // Zeit in struct tm-Format umwandeln
                        struct tm *localTime = localtime(&appointments[i].start);

                        // Ausgabe der Startzeit
                        printf("Startzeit: %d.%d.%d %02d:%02d\n",
                               localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900,
                               localTime->tm_hour, localTime->tm_min);

                        // Dauer in Stunden und Minuten umrechnen und ausgeben
                        int durationHours = appointments[i].duration / 3600;
                        int durationMinutes = (appointments[i].duration % 3600) / 60;
                        printf("Dauer: %d Stunden %d Minuten\n", durationHours, durationMinutes);

                        printf("Titel: %s\n", appointments[i].title);
                        printf("\n");

                        temp = temp->next;
                    }
                    else {
                        temp = temp->next;
                    }
                }
                while (temp != NULL);
            }

            break;
        }
        case 9: {
            FILE *pFile;

            pFile=fopen("export.txt", "a");

            if(pFile==NULL) {
                perror("Error opening file.");
            }
            else {
                for (int i = 0; i < countAppointments; i++)
                {
                    // Zeit in struct tm-Format umwandeln
                    struct tm *localTime = localtime(&appointments[i].start);

                    // Dauer in Stunden und Minuten umrechnen und ausgeben
                    int durationHours = appointments[i].duration / 3600;
                    int durationMinutes = (appointments[i].duration % 3600) / 60;

                    fprintf(pFile,
                            "Termin %d\n"
                            "Datum: %d.%d.%d %02d:%02d\n"
                            "Dauer: %d Stunden %d Minuten\n"
                            "Titel: %s\n",
                            i+1,
                            localTime->tm_mday,
                            localTime->tm_mon + 1,
                            localTime->tm_year + 1900,
                            localTime->tm_hour,
                            localTime->tm_min,
                            durationHours,
                            durationMinutes,
                            appointments[i].title
                            );

                    fprintf(pFile, "Teilnehmer:innen:\n");

                    LLNode* temp = appointments[i].personenHead;

                    if (temp != NULL) {
                        // if the linked list is not empty, traverse to the end of the linked list
                        do
                        {
                            fprintf(pFile, "%s, %s\n", temp->data->nachname, temp->data->vorname);
                            temp = temp->next;
                        }
                        while (temp != NULL);
                    }

                    fprintf(pFile, "\n\n");

                }
            }
            fclose(pFile);

            break;
        }
        case 10:
            free(appointments);
            return 0;
        
        default:
            puts("Ungueltige Eingabe!");
            break;
        }

    }
    


    return 0;
}