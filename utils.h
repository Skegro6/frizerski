#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdlib.h>
#include "tipovi.h"

#define SAFE_FREE(ptr) do { \
    free(ptr);             \
    (ptr) = NULL;          \
} while (0)

static inline int je_prazan_string(const char *tekst) {
    return tekst == NULL || tekst[0] == '\0';
}

void ocisti_buffer(void);
void pauziraj(void);
void ocisti_ekran(void);
void ispisi_zaglavlje(const char *naslov);
void ispisi_liniju(int duzina);

int ucitaj_int(const char *poruka, int *vrijednost);
int ucitaj_float(const char *poruka, float *vrijednost);
int ucitaj_string(const char *poruka, char *buffer, size_t velicina);

const char *vrsta_usluge_u_string(VrstaUsluge vrsta);
const char *status_zaposlenika_u_string(StatusZaposlenika status);
const char *status_termina_u_string(StatusTermina status);

int datum_u_string(Datum datum, char *buffer, size_t velicina);
int vrijeme_u_string(Vrijeme vrijeme, char *buffer, size_t velicina);
DatumVrijeme trenutno_datum_vrijeme(void);

#endif
