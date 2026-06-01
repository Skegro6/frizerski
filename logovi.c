#include "logovi.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dodaj_log(LogCvor **glava, const char *poruka) {
    LogCvor *novi;

    if (glava == NULL || poruka == NULL) {
        return ERR_NULL_PTR;
    }

    novi = (LogCvor *)calloc(1U, sizeof(LogCvor));
    if (novi == NULL) {
        return ERR_MEMORY;
    }

    (void)snprintf(novi->poruka, sizeof(novi->poruka), "%s", poruka);
    novi->sljedeci = *glava;
    *glava = novi;
    return ERR_OK;
}

void ispisi_logove(const LogCvor *glava) {
    const LogCvor *trenutni = glava;
    int redni = 1;

    if (glava == NULL) {
        printf("Nema logova u ovoj sesiji.\n");
        return;
    }

    while (trenutni != NULL) {
        printf("%d. %s\n", redni, trenutni->poruka);
        trenutni = trenutni->sljedeci;
        redni++;
    }
}

void oslobodi_logove(LogCvor **glava) {
    LogCvor *trenutni;

    if (glava == NULL) {
        return;
    }

    while (*glava != NULL) {
        trenutni = *glava;
        *glava = (*glava)->sljedeci;
        free(trenutni);
    }
}
