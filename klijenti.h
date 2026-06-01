#ifndef KLIJENTI_H
#define KLIJENTI_H

#include "tipovi.h"

int dodaj_klijenta(Klijent **lista, int *broj, const Klijent *novi_klijent);
Klijent *pronadji_klijenta_po_id(Klijent *lista, int broj, int id);
Klijent *pronadji_klijenta_bsearch(Klijent *lista, int broj, int id);
Klijent *pronadji_klijenta_po_prezimenu(Klijent *lista, int broj, const char *prezime);
int uredi_klijenta(Klijent *lista, int broj, int id, const Klijent *novi);
int obrisi_klijenta(Klijent **lista, int *broj, int id);

void ispisi_klijenta(const Klijent *klijent);
void ispisi_sve_klijente(const Klijent *lista, int broj);
void ispisi_sortirane_klijente(Klijent *lista, int broj);

int usporedi_klijente_po_prezimenu(const void *a, const void *b);
int usporedi_klijente_po_id(const void *a, const void *b);
int prikupi_podatke_klijenta(Klijent *klijent);
int pronadji_max_id_klijenta(const Klijent *lista, int broj);

#endif
