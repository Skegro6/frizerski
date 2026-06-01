#ifndef VALIDACIJA_H
#define VALIDACIJA_H

#include "config.h"
#include "tipovi.h"

int je_validan_email(const char *email);
int je_validan_telefon(const char *telefon);
int je_validan_string(const char *str, int min_duzina, int max_duzina);
int je_validan_id(int id);
int je_validan_datum(int dan, int mjesec, int godina);
int je_validna_cijena(float cijena);
int je_validno_vrijeme_termina(int sati, int minute);
int je_buduci_datum(Datum datum);

#endif
