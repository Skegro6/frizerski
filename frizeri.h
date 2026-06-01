#ifndef FRIZERI_H
#define FRIZERI_H

#include "tipovi.h"

int dodaj_frizera(Frizer **lista, int *broj, const Frizer *novi_frizer);
Frizer *pronadji_frizera_po_id(Frizer *lista, int broj, int id);
Frizer *pronadji_frizera_bsearch(Frizer *lista, int broj, int id);
Frizer *pronadji_frizera_po_prezimenu(Frizer *lista, int broj, const char *prezime);
int uredi_frizera(Frizer *lista, int broj, int id, const Frizer *novi);
int obrisi_frizera(Frizer **lista, int *broj, int id);

void ispisi_frizera(const Frizer *frizer);
void ispisi_sve_frizere(const Frizer *lista, int broj);
void ispisi_sortirane_frizere(Frizer *lista, int broj);

int usporedi_frizere_po_prezimenu(const void *a, const void *b);
int usporedi_frizere_po_id(const void *a, const void *b);
int prikupi_podatke_frizera(Frizer *frizer);
int pronadji_max_id_frizera(const Frizer *lista, int broj);

#endif
