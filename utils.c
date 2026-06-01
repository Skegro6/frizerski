#include "utils.h"
#include "config.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void ocisti_buffer(void) {
    int znak;
    while ((znak = getchar()) != '\n' && znak != EOF) {
    }
}

void pauziraj(void) {
    printf("\nPritisnite ENTER za nastavak...");
    (void)getchar();
}

void ocisti_ekran(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ispisi_liniju(int duzina) {
    if (duzina < 0) {
        return;
    }

    for (int i = 0; i < duzina; i++) {
        putchar('=');
    }
    putchar('\n');
}

void ispisi_zaglavlje(const char *naslov) {
    if (naslov == NULL) {
        return;
    }

    int duzina = (int)strlen(naslov) + 8;
    putchar('\n');
    ispisi_liniju(duzina);
    printf("=== %s ===\n", naslov);
    ispisi_liniju(duzina);
}

int ucitaj_string(const char *poruka, char *buffer, size_t velicina) {
    size_t duzina;
    int znak;

    if (poruka == NULL || buffer == NULL || velicina == 0U) {
        return ERR_NULL_PTR;
    }

    printf("%s", poruka);
    if (fgets(buffer, (int)velicina, stdin) == NULL) {
        return ERR_INVALID_INPUT;
    }

    duzina = strcspn(buffer, "\n");
    if (buffer[duzina] == '\n') {
        buffer[duzina] = '\0';
    } else {
        while ((znak = getchar()) != '\n' && znak != EOF) {
        }
    }

    return ERR_OK;
}

int ucitaj_int(const char *poruka, int *vrijednost) {
    char unos[MAX_UNOS];
    char *kraj = NULL;
    long rezultat;

    if (poruka == NULL || vrijednost == NULL) {
        return ERR_NULL_PTR;
    }

    if (ucitaj_string(poruka, unos, sizeof(unos)) != ERR_OK) {
        return ERR_INVALID_INPUT;
    }

    errno = 0;
    rezultat = strtol(unos, &kraj, 10);
    if (errno != 0 || kraj == unos || (*kraj != '\0' && *kraj != '\n') ||
        rezultat < INT_MIN || rezultat > INT_MAX) {
        return ERR_INVALID_INPUT;
    }

    *vrijednost = (int)rezultat;
    return ERR_OK;
}

int ucitaj_float(const char *poruka, float *vrijednost) {
    char unos[MAX_UNOS];
    char *kraj = NULL;
    float rezultat;

    if (poruka == NULL || vrijednost == NULL) {
        return ERR_NULL_PTR;
    }

    if (ucitaj_string(poruka, unos, sizeof(unos)) != ERR_OK) {
        return ERR_INVALID_INPUT;
    }

    errno = 0;
    rezultat = strtof(unos, &kraj);
    if (errno != 0 || kraj == unos || (*kraj != '\0' && *kraj != '\n')) {
        return ERR_INVALID_INPUT;
    }

    *vrijednost = rezultat;
    return ERR_OK;
}

const char *vrsta_usluge_u_string(VrstaUsluge vrsta) {
    switch (vrsta) {
        case SISANJE: return "Sisanje";
        case BOJANJE: return "Bojanje";
        case PRANJE: return "Pranje";
        case TRETMAN: return "Tretman";
        case BRIJANJE: return "Brijanje";
        case MODELIRANJE: return "Modeliranje";
        default: return "Nepoznata";
    }
}

const char *status_zaposlenika_u_string(StatusZaposlenika status) {
    switch (status) {
        case STATUS_AKTIVAN: return "Aktivan";
        case STATUS_NEAKTIVAN: return "Neaktivan";
        case STATUS_NA_GODISNJEM: return "Na godisnjem";
        default: return "Nepoznat";
    }
}

const char *status_termina_u_string(StatusTermina status) {
    switch (status) {
        case TERMIN_SLOBODAN: return "Slobodan";
        case TERMIN_ZAUZET: return "Zauzet";
        case TERMIN_OTKAZAN: return "Otkazan";
        case TERMIN_ODRADEN: return "Odraden";
        default: return "Nepoznat";
    }
}

int datum_u_string(Datum datum, char *buffer, size_t velicina) {
    if (buffer == NULL || velicina == 0U) {
        return ERR_NULL_PTR;
    }

    (void)snprintf(buffer, velicina, "%02d.%02d.%04d", datum.dan, datum.mjesec, datum.godina);
    return ERR_OK;
}

int vrijeme_u_string(Vrijeme vrijeme, char *buffer, size_t velicina) {
    if (buffer == NULL || velicina == 0U) {
        return ERR_NULL_PTR;
    }

    (void)snprintf(buffer, velicina, "%02d:%02d", vrijeme.sati, vrijeme.minute);
    return ERR_OK;
}

DatumVrijeme trenutno_datum_vrijeme(void) {
    DatumVrijeme rezultat = {{1, 1, 1970}, {0, 0}};
    time_t sada = time(NULL);
    struct tm *lokalno = localtime(&sada);

    if (lokalno != NULL) {
        rezultat.datum.dan = lokalno->tm_mday;
        rezultat.datum.mjesec = lokalno->tm_mon + 1;
        rezultat.datum.godina = lokalno->tm_year + 1900;
        rezultat.vrijeme.sati = lokalno->tm_hour;
        rezultat.vrijeme.minute = lokalno->tm_min;
    }

    return rezultat;
}
