#ifndef USLUGE_H
#define USLUGE_H

#include "tipovi.h"

int dodaj_uslugu(Usluga **lista, int *broj, const Usluga *nova_usluga);
Usluga *pronadji_uslugu_po_id(Usluga *lista, int broj, int id);
Usluga *pronadji_uslugu_bsearch(Usluga *lista, int broj, int id);
Usluga *pronadji_uslugu_po_nazivu(Usluga *lista, int broj, const char *naziv);
int uredi_uslugu(Usluga *lista, int broj, int id, const Usluga *nova);
int obrisi_uslugu(Usluga **lista, int *broj, int id);

void ispisi_uslugu(const Usluga *usluga);
void ispisi_sve_usluge(const Usluga *lista, int broj);
void ispisi_sortirane_usluge(Usluga *lista, int broj);

int usporedi_usluge_po_cijeni(const void *a, const void *b);
int usporedi_usluge_po_id(const void *a, const void *b);
int prikupi_podatke_usluge(Usluga *usluga);
int pronadji_max_id_usluge(const Usluga *lista, int broj);

#endif
