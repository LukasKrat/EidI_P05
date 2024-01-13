#include <stdio.h>
#include <string.h>

int main() {
    // Original-Quellzeichenkette
    char source[] = "Hello, world!";

    // Unsicherer Kopiervorgang mit strcpy
    char unsafeDestination[15]; // Hier Größe verändern auf z.B. 5 um Fehler zu provozieren
    strcpy(unsafeDestination, source);
    printf("Source: %s\n", source);
    printf("Unsafe Copy - Destination: %s\n\n", unsafeDestination);

    // Sicheres Kopieren mit strncpy
    char safeDestination[5];
    strncpy(safeDestination, source, sizeof(safeDestination) - 1);
    safeDestination[sizeof(safeDestination) - 1] = '\0'; // Manuelle Hinzufügung des Null-Zeichens
    printf("Safe Copy - Destination: %s\n", safeDestination);

    return 0;
}
