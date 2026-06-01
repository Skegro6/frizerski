#ifndef IZBORNIK_H
#define IZBORNIK_H

#include "config.h"
#include "tipovi.h"

void prikazi_glavni_izbornik(void);
void prikazi_izbornik_klijenti(Klijent **klijenti, int *broj_klijenata);
void prikazi_izbornik_frizeri(Frizer **frizeri, int *broj_frizera);
void prikazi_izbornik_usluge(Usluga **usluge, int *broj_usluga);
void prikazi_izbornik_termini(Termin **termini, int *broj_termina,
                              Klijent *klijenti, int broj_klijenata,
                              Frizer *frizeri, int broj_frizera,
                              Usluga *usluge, int broj_usluga);
void prikazi_izbornik_izvjestaji(const Klijent *klijenti, int broj_klijenata,
                                 const Frizer *frizeri, int broj_frizera,
                                 const Usluga *usluge, int broj_usluga,
                                 const Termin *termini, int broj_termina);
void prikazi_izbornik_postavke(void);

#endif
