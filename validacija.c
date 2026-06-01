#include "validacija.h"
#include "config.h"
#include <ctype.h>
#include <string.h>
#include <time.h>

int je_validan_email(const char *email) {
    const char *at;
    const char *dot;

    if (email == NULL || strlen(email) < 5U) {
        return 0;
    }

    at = strchr(email, '@');
    if (at == NULL || at == email) {
        return 0;
    }

    dot = strchr(at + 1, '.');
    if (dot == NULL || dot == at + 1 || dot[1] == '\0') {
        return 0;
    }

    return 1;
}

int je_validan_telefon(const char *telefon) {
    size_t duzina;

    if (telefon == NULL) {
        return 0;
    }

    duzina = strlen(telefon);
    if (duzina < 6U || duzina >= MAX_TELEFON) {
        return 0;
    }

    for (size_t i = 0; i < duzina; i++) {
        unsigned char znak = (unsigned char)telefon[i];
        if (!isdigit(znak) && telefon[i] != '+' && telefon[i] != '-' && telefon[i] != ' ') {
            return 0;
        }
    }

    return 1;
}

int je_validan_string(const char *str, int min_duzina, int max_duzina) {
    size_t duzina;

    if (str == NULL || min_duzina < 0 || max_duzina < min_duzina) {
        return 0;
    }

    duzina = strlen(str);
    return duzina >= (size_t)min_duzina && duzina <= (size_t)max_duzina;
}

int je_validan_id(int id) {
    return id > 0;
}

int je_validan_datum(int dan, int mjesec, int godina) {
    int dani_u_mjesecu[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (godina < 1900 || godina > 2100) {
        return 0;
    }

    if (mjesec < 1 || mjesec > 12) {
        return 0;
    }

    if ((godina % 4 == 0 && godina % 100 != 0) || (godina % 400 == 0)) {
        dani_u_mjesecu[1] = 29;
    }

    return dan >= 1 && dan <= dani_u_mjesecu[mjesec - 1];
}

int je_validna_cijena(float cijena) {
    return cijena >= 0.0f && cijena < 1000000.0f;
}

int je_validno_vrijeme_termina(int sati, int minute) {
    return sati >= RADNI_SATI_OD && sati < RADNI_SATI_DO && minute >= 0 && minute < 60;
}

int je_buduci_datum(Datum datum) {
    time_t sada = time(NULL);
    struct tm *lokalno = localtime(&sada);

    if (lokalno == NULL) {
        return 0;
    }

    if (datum.godina > lokalno->tm_year + 1900) {
        return 1;
    }
    if (datum.godina < lokalno->tm_year + 1900) {
        return 0;
    }
    if (datum.mjesec > lokalno->tm_mon + 1) {
        return 1;
    }
    if (datum.mjesec < lokalno->tm_mon + 1) {
        return 0;
    }

    return datum.dan >= lokalno->tm_mday;
}
