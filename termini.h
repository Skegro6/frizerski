#ifndef TERMINI_H
#define TERMINI_H

#include "tipovi.h"

int dodaj_termin(Termin **lista, int *broj, const Termin *novi_termin);
Termin *pronadji_termin_po_id(Termin *lista, int broj, int id);
Termin *pronadji_termin_bsearch(Termin *lista, int broj, int id);
int uredi_termin(Termin *lista, int broj, int id, const Termin *novi);
int obrisi_termin(Termin **lista, int *broj, int id);
int je_frizer_dostupan(const Termin *lista, int broj, int id_frizera, DatumVrijeme kada, int zanemari_id_termina);

void ispisi_termin(const Termin *termin);
void ispisi_sve_termine(const Termin *lista, int broj);
void ispisi_termine_dana(const Termin *lista, int broj, Datum dan);
void ispisi_sortirane_termine(Termin *lista, int broj);

int usporedi_termine_po_datumu(const void *a, const void *b);
int usporedi_termine_po_id(const void *a, const void *b);
int prikupi_podatke_termina(Termin *termin);
int pronadji_max_id_termina(const Termin *lista, int broj);

#endif
