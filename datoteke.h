#ifndef DATOTEKE_H
#define DATOTEKE_H

#include <stddef.h>
#include "tipovi.h"

int spremi_klijente(const char *putanja, const Klijent *lista, int broj);
int ucitaj_klijente(const char *putanja, Klijent **lista, int *broj);

int spremi_frizere(const char *putanja, const Frizer *lista, int broj);
int ucitaj_frizere(const char *putanja, Frizer **lista, int *broj);

int spremi_usluge(const char *putanja, const Usluga *lista, int broj);
int ucitaj_usluge(const char *putanja, Usluga **lista, int *broj);

int spremi_termine(const char *putanja, const Termin *lista, int broj);
int ucitaj_termine(const char *putanja, Termin **lista, int *broj);

int datoteka_postoji(const char *putanja);
long velicina_datoteke(const char *putanja);
int obrisi_datoteku(const char *putanja);
int kopiraj_datoteku(const char *izvor, const char *odrediste);
int sigurno_zamijeni_datoteku(const char *privremena, const char *originalna);

#endif
