#include "izbornik.h"
#include "config.h"
#include "datoteke.h"
#include "frizeri.h"
#include "globals.h"
#include "klijenti.h"
#include "termini.h"
#include "usluge.h"
#include "utils.h"
#include "validacija.h"
#include <stdio.h>
#include <string.h>

static void ispisi_status_rezultat(int kod, const char *uspjeh) {
    if (kod == ERR_OK) {
        printf("%s\n", uspjeh);
    } else if (kod == ERR_NOT_FOUND) {
        printf("Zapis nije pronaden.\n");
    } else if (kod == ERR_DUPLICATE) {
        printf("Duplikat ili zauzet termin.\n");
    } else if (kod == ERR_INVALID_INPUT) {
        printf("Neispravan unos.\n");
    } else if (kod == ERR_MEMORY) {
        printf("Greska pri alokaciji memorije.\n");
    } else {
        printf("Greska, kod: %d\n", kod);
    }
}

static int provjeri_povezane_podatke_termina(const Termin *termin,
                                             Klijent *klijenti, int broj_klijenata,
                                             Frizer *frizeri, int broj_frizera,
                                             Usluga *usluge, int broj_usluga) {
    if (termin == NULL) {
        return ERR_NULL_PTR;
    }

    if (pronadji_klijenta_bsearch(klijenti, broj_klijenata, termin->id_klijenta) == NULL) {
        printf("Klijent s ID-em %d ne postoji.\n", termin->id_klijenta);
        return ERR_NOT_FOUND;
    }
    if (pronadji_frizera_bsearch(frizeri, broj_frizera, termin->id_frizera) == NULL) {
        printf("Frizer s ID-em %d ne postoji.\n", termin->id_frizera);
        return ERR_NOT_FOUND;
    }
    if (pronadji_uslugu_bsearch(usluge, broj_usluga, termin->id_usluge) == NULL) {
        printf("Usluga s ID-em %d ne postoji.\n", termin->id_usluge);
        return ERR_NOT_FOUND;
    }

    return ERR_OK;
}

void prikazi_glavni_izbornik(void) {
    printf("\n=== SALON FRIZURA ===\n");
    printf("1. Upravljanje klijentima\n");
    printf("2. Upravljanje frizerima\n");
    printf("3. Upravljanje uslugama\n");
    printf("4. Upravljanje terminima\n");
    printf("5. Izvjestaji\n");
    printf("6. Postavke\n");
    printf("0. Spremi i izadi\n");
}

void prikazi_izbornik_klijenti(Klijent **klijenti, int *broj_klijenata) {
    int opcija;

    if (klijenti == NULL || broj_klijenata == NULL) {
        return;
    }

    do {
        printf("\n--- KLIJENTI ---\n");
        printf("1. Dodaj klijenta\n2. Pregled\n3. Uredi\n4. Obrisi\n5. Trazi po ID-u\n6. Sortiraj po prezimenu\n0. Natrag\n");
        if (ucitaj_int("Odabir: ", &opcija) != ERR_OK) {
            printf("Neispravan unos.\n");
            continue;
        }

        switch ((OpcijaCrud)opcija) {
            case CRUD_DODAJ: {
                Klijent novi;
                int kod = prikupi_podatke_klijenta(&novi);
                if (kod == ERR_OK) {
                    kod = dodaj_klijenta(klijenti, broj_klijenata, &novi);
                }
                ispisi_status_rezultat(kod, "Klijent dodan.");
                break;
            }
            case CRUD_PREGLED:
                ispisi_sve_klijente(*klijenti, *broj_klijenata);
                break;
            case CRUD_UREDI: {
                int id;
                Klijent novi;
                int kod;
                if (ucitaj_int("ID klijenta: ", &id) != ERR_OK) {
                    printf("Neispravan ID.\n");
                    break;
                }
                kod = prikupi_podatke_klijenta(&novi);
                if (kod == ERR_OK) {
                    kod = uredi_klijenta(*klijenti, *broj_klijenata, id, &novi);
                }
                ispisi_status_rezultat(kod, "Klijent ureden.");
                break;
            }
            case CRUD_OBRISI: {
                int id;
                int kod;
                if (ucitaj_int("ID klijenta: ", &id) != ERR_OK) {
                    printf("Neispravan ID.\n");
                    break;
                }
                kod = obrisi_klijenta(klijenti, broj_klijenata, id);
                ispisi_status_rezultat(kod, "Klijent obrisan.");
                break;
            }
            case CRUD_TRAZI: {
                KljucPretrage kljuc;
                Klijent *pronadjen;
                if (ucitaj_int("ID klijenta: ", &kljuc.id) != ERR_OK) {
                    printf("Neispravan ID.\n");
                    break;
                }
                pronadjen = pronadji_klijenta_bsearch(*klijenti, *broj_klijenata, kljuc.id);
                if (pronadjen != NULL) {
                    ispisi_klijenta(pronadjen);
                } else {
                    printf("Klijent nije pronaden.\n");
                }
                break;
            }
            case CRUD_SORTIRAJ:
                ispisi_sortirane_klijente(*klijenti, *broj_klijenata);
                break;
            case CRUD_NATRAG:
                break;
            default:
                printf("Nepostojeca opcija.\n");
                break;
        }
    } while (opcija != CRUD_NATRAG);
}

void prikazi_izbornik_frizeri(Frizer **frizeri, int *broj_frizera) {
    int opcija;

    if (frizeri == NULL || broj_frizera == NULL) {
        return;
    }

    do {
        printf("\n--- FRIZERI ---\n");
        printf("1. Dodaj frizera\n2. Pregled\n3. Uredi\n4. Obrisi\n5. Trazi po ID-u\n6. Sortiraj po prezimenu\n0. Natrag\n");
        if (ucitaj_int("Odabir: ", &opcija) != ERR_OK) {
            printf("Neispravan unos.\n");
            continue;
        }

        switch ((OpcijaCrud)opcija) {
            case CRUD_DODAJ: {
                Frizer novi;
                int kod = prikupi_podatke_frizera(&novi);
                if (kod == ERR_OK) {
                    kod = dodaj_frizera(frizeri, broj_frizera, &novi);
                }
                ispisi_status_rezultat(kod, "Frizer dodan.");
                break;
            }
            case CRUD_PREGLED:
                ispisi_sve_frizere(*frizeri, *broj_frizera);
                break;
            case CRUD_UREDI: {
                int id;
                Frizer novi;
                int kod;
                if (ucitaj_int("ID frizera: ", &id) != ERR_OK) {
                    printf("Neispravan ID.\n");
                    break;
                }
                kod = prikupi_podatke_frizera(&novi);
                if (kod == ERR_OK) {
                    kod = uredi_frizera(*frizeri, *broj_frizera, id, &novi);
                }
                ispisi_status_rezultat(kod, "Frizer ureden.");
                break;
            }
            case CRUD_OBRISI: {
                int id;
                int kod;
                if (ucitaj_int("ID frizera: ", &id) != ERR_OK) {
                    printf("Neispravan ID.\n");
                    break;
                }
                kod = obrisi_frizera(frizeri, broj_frizera, id);
                ispisi_status_rezultat(kod, "Frizer obrisan.");
                break;
            }
            case CRUD_TRAZI: {
                int id;
                Frizer *pronadjen;
                if (ucitaj_int("ID frizera: ", &id) != ERR_OK) {
                    printf("Neispravan ID.\n");
                    break;
                }
                pronadjen = pronadji_frizera_bsearch(*frizeri, *broj_frizera, id);
                if (pronadjen != NULL) {
                    ispisi_frizera(pronadjen);
                } else {
                    printf("Frizer nije pronaden.\n");
                }
                break;
            }
            case CRUD_SORTIRAJ:
                ispisi_sortirane_frizere(*frizeri, *broj_frizera);
                break;
            case CRUD_NATRAG:
                break;
            default:
                printf("Nepostojeca opcija.\n");
                break;
        }
    } while (opcija != CRUD_NATRAG);
}

void prikazi_izbornik_usluge(Usluga **usluge, int *broj_usluga) {
    int opcija;

    if (usluge == NULL || broj_usluga == NULL) {
        return;
    }

    do {
        printf("\n--- USLUGE ---\n");
        printf("1. Dodaj uslugu\n2. Pregled\n3. Uredi\n4. Obrisi\n5. Trazi po ID-u\n6. Sortiraj po cijeni\n0. Natrag\n");
        if (ucitaj_int("Odabir: ", &opcija) != ERR_OK) {
            printf("Neispravan unos.\n");
            continue;
        }

        switch ((OpcijaCrud)opcija) {
            case CRUD_DODAJ: {
                Usluga nova;
                int kod = prikupi_podatke_usluge(&nova);
                if (kod == ERR_OK) {
                    kod = dodaj_uslugu(usluge, broj_usluga, &nova);
                }
                ispisi_status_rezultat(kod, "Usluga dodana.");
                break;
            }
            case CRUD_PREGLED:
                ispisi_sve_usluge(*usluge, *broj_usluga);
                break;
            case CRUD_UREDI: {
                int id;
                Usluga nova;
                int kod;
                if (ucitaj_int("ID usluge: ", &id) != ERR_OK) {
                    printf("Neispravan ID.\n");
                    break;
                }
                kod = prikupi_podatke_usluge(&nova);
                if (kod == ERR_OK) {
                    kod = uredi_uslugu(*usluge, *broj_usluga, id, &nova);
                }
                ispisi_status_rezultat(kod, "Usluga uredena.");
                break;
            }
            case CRUD_OBRISI: {
                int id;
                int kod;
                if (ucitaj_int("ID usluge: ", &id) != ERR_OK) {
                    printf("Neispravan ID.\n");
                    break;
                }
                kod = obrisi_uslugu(usluge, broj_usluga, id);
                ispisi_status_rezultat(kod, "Usluga obrisana.");
                break;
            }
            case CRUD_TRAZI: {
                int id;
                Usluga *pronadjena;
                if (ucitaj_int("ID usluge: ", &id) != ERR_OK) {
                    printf("Neispravan ID.\n");
                    break;
                }
                pronadjena = pronadji_uslugu_bsearch(*usluge, *broj_usluga, id);
                if (pronadjena != NULL) {
                    ispisi_uslugu(pronadjena);
                } else {
                    printf("Usluga nije pronadena.\n");
                }
                break;
            }
            case CRUD_SORTIRAJ:
                ispisi_sortirane_usluge(*usluge, *broj_usluga);
                break;
            case CRUD_NATRAG:
                break;
            default:
                printf("Nepostojeca opcija.\n");
                break;
        }
    } while (opcija != CRUD_NATRAG);
}

void prikazi_izbornik_termini(Termin **termini, int *broj_termina,
                              Klijent *klijenti, int broj_klijenata,
                              Frizer *frizeri, int broj_frizera,
                              Usluga *usluge, int broj_usluga) {
    int opcija;

    if (termini == NULL || broj_termina == NULL) {
        return;
    }

    do {
        printf("\n--- TERMINI ---\n");
        printf("1. Dodaj termin\n2. Pregled\n3. Uredi\n4. Obrisi\n5. Trazi po ID-u\n6. Sortiraj po datumu\n0. Natrag\n");
        if (ucitaj_int("Odabir: ", &opcija) != ERR_OK) {
            printf("Neispravan unos.\n");
            continue;
        }

        switch ((OpcijaCrud)opcija) {
            case CRUD_DODAJ: {
                Termin novi;
                int kod = prikupi_podatke_termina(&novi);
                if (kod == ERR_OK) {
                    kod = provjeri_povezane_podatke_termina(&novi, klijenti, broj_klijenata,
                                                            frizeri, broj_frizera,
                                                            usluge, broj_usluga);
                }
                if (kod == ERR_OK) {
                    kod = dodaj_termin(termini, broj_termina, &novi);
                }
                ispisi_status_rezultat(kod, "Termin dodan.");
                break;
            }
            case CRUD_PREGLED:
                ispisi_sve_termine(*termini, *broj_termina);
                break;
            case CRUD_UREDI: {
                int id;
                Termin novi;
                int kod;
                if (ucitaj_int("ID termina: ", &id) != ERR_OK) {
                    printf("Neispravan ID.\n");
                    break;
                }
                kod = prikupi_podatke_termina(&novi);
                if (kod == ERR_OK) {
                    kod = provjeri_povezane_podatke_termina(&novi, klijenti, broj_klijenata,
                                                            frizeri, broj_frizera,
                                                            usluge, broj_usluga);
                }
                if (kod == ERR_OK) {
                    kod = uredi_termin(*termini, *broj_termina, id, &novi);
                }
                ispisi_status_rezultat(kod, "Termin ureden.");
                break;
            }
            case CRUD_OBRISI: {
                int id;
                int kod;
                if (ucitaj_int("ID termina: ", &id) != ERR_OK) {
                    printf("Neispravan ID.\n");
                    break;
                }
                kod = obrisi_termin(termini, broj_termina, id);
                ispisi_status_rezultat(kod, "Termin obrisan.");
                break;
            }
            case CRUD_TRAZI: {
                int id;
                Termin *pronadjen;
                if (ucitaj_int("ID termina: ", &id) != ERR_OK) {
                    printf("Neispravan ID.\n");
                    break;
                }
                pronadjen = pronadji_termin_bsearch(*termini, *broj_termina, id);
                if (pronadjen != NULL) {
                    ispisi_termin(pronadjen);
                } else {
                    printf("Termin nije pronaden.\n");
                }
                break;
            }
            case CRUD_SORTIRAJ:
                ispisi_sortirane_termine(*termini, *broj_termina);
                break;
            case CRUD_NATRAG:
                break;
            default:
                printf("Nepostojeca opcija.\n");
                break;
        }
    } while (opcija != CRUD_NATRAG);
}

void prikazi_izbornik_izvjestaji(const Klijent *klijenti, int broj_klijenata,
                                 const Frizer *frizeri, int broj_frizera,
                                 const Usluga *usluge, int broj_usluga,
                                 const Termin *termini, int broj_termina) {
    float prihod = 0.0f;
    int odradeni = 0;

    (void)klijenti;
    (void)frizeri;
    (void)usluge;

    if (termini != NULL) {
        for (int i = 0; i < broj_termina; i++) {
            if (termini[i].status == TERMIN_ODRADEN) {
                prihod += termini[i].uplacena_cijena;
                odradeni++;
            }
        }
    }

    g_ukupan_prihod = prihod;
    g_ukupno_odradenih_termina = odradeni;

    printf("\n--- IZVJESTAJI ---\n");
    printf("Broj klijenata: %d\n", broj_klijenata);
    printf("Broj frizera: %d\n", broj_frizera);
    printf("Broj usluga: %d\n", broj_usluga);
    printf("Broj termina: %d\n", broj_termina);
    printf("Odradeni termini: %d\n", odradeni);
    printf("Ukupan prihod: %.2f EUR\n", prihod);
}

void prikazi_izbornik_postavke(void) {
    ispisi_globalne_informacije();
    printf("Velicina datoteke klijenata: %ld bajtova\n", velicina_datoteke(PATH_KLIJENTI));
    printf("Velicina datoteke frizera: %ld bajtova\n", velicina_datoteke(PATH_FRIZERI));
    printf("Velicina datoteke usluga: %ld bajtova\n", velicina_datoteke(PATH_USLUGE));
    printf("Velicina datoteke termina: %ld bajtova\n", velicina_datoteke(PATH_TERMINI));
}
