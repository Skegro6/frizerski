#include "termini.h"
#include "config.h"
#include "globals.h"
#include "utils.h"
#include "validacija.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int smanji_memoriju_termina(Termin **lista, int broj) {
    Termin *temp;

    if (lista == NULL) {
        return ERR_NULL_PTR;
    }
    if (broj == 0) {
        SAFE_FREE(*lista);
        return ERR_OK;
    }

    temp = (Termin *)realloc(*lista, (size_t)broj * sizeof(Termin));
    if (temp != NULL) {
        *lista = temp;
    }
    return ERR_OK;
}

static int usporedi_datum_vrijeme(DatumVrijeme a, DatumVrijeme b) {
    if (a.datum.godina != b.datum.godina) {
        return (a.datum.godina > b.datum.godina) - (a.datum.godina < b.datum.godina);
    }
    if (a.datum.mjesec != b.datum.mjesec) {
        return (a.datum.mjesec > b.datum.mjesec) - (a.datum.mjesec < b.datum.mjesec);
    }
    if (a.datum.dan != b.datum.dan) {
        return (a.datum.dan > b.datum.dan) - (a.datum.dan < b.datum.dan);
    }
    if (a.vrijeme.sati != b.vrijeme.sati) {
        return (a.vrijeme.sati > b.vrijeme.sati) - (a.vrijeme.sati < b.vrijeme.sati);
    }
    return (a.vrijeme.minute > b.vrijeme.minute) - (a.vrijeme.minute < b.vrijeme.minute);
}

int dodaj_termin(Termin **lista, int *broj, const Termin *novi_termin) {
    Termin *temp;

    if (lista == NULL || broj == NULL || novi_termin == NULL) {
        return ERR_NULL_PTR;
    }
    if (*broj < 0 || *broj >= MAX_TERMINA) {
        return ERR_INVALID_RANGE;
    }
    if (*broj > 0 && *lista == NULL) {
        return ERR_NULL_PTR;
    }
    if (!je_validan_id(novi_termin->id_klijenta) ||
        !je_validan_id(novi_termin->id_frizera) ||
        !je_validan_id(novi_termin->id_usluge) ||
        !je_validna_cijena(novi_termin->uplacena_cijena)) {
        return ERR_INVALID_INPUT;
    }
    if (!je_validan_datum(novi_termin->datum_vrijeme.datum.dan,
                          novi_termin->datum_vrijeme.datum.mjesec,
                          novi_termin->datum_vrijeme.datum.godina) ||
        !je_validno_vrijeme_termina(novi_termin->datum_vrijeme.vrijeme.sati,
                                    novi_termin->datum_vrijeme.vrijeme.minute)) {
        return ERR_INVALID_INPUT;
    }
    if (!je_frizer_dostupan(*lista, *broj, novi_termin->id_frizera, novi_termin->datum_vrijeme, 0)) {
        return ERR_DUPLICATE;
    }

    temp = (Termin *)realloc(*lista, (size_t)(*broj + 1) * sizeof(Termin));
    if (temp == NULL) {
        return ERR_MEMORY;
    }

    *lista = temp;
    (*lista)[*broj] = *novi_termin;
    (*lista)[*broj].id = generiraj_novi_id(&g_last_termin_id);
    (*lista)[*broj].status = TERMIN_ZAUZET;
    (*broj)++;
    g_ukupno_termina = *broj;
    g_promjene_nisu_spremljene = 1;
    return ERR_OK;
}

Termin *pronadji_termin_po_id(Termin *lista, int broj, int id) {
    if (lista == NULL || broj <= 0 || id <= 0) {
        return NULL;
    }
    for (int i = 0; i < broj; i++) {
        if (lista[i].id == id) {
            return &lista[i];
        }
    }
    return NULL;
}

Termin *pronadji_termin_bsearch(Termin *lista, int broj, int id) {
    Termin kljuc;

    if (lista == NULL || broj <= 0 || id <= 0) {
        return NULL;
    }

    qsort(lista, (size_t)broj, sizeof(Termin), usporedi_termine_po_id);
    memset(&kljuc, 0, sizeof(Termin));
    kljuc.id = id;
    return (Termin *)bsearch(&kljuc, lista, (size_t)broj, sizeof(Termin), usporedi_termine_po_id);
}

int uredi_termin(Termin *lista, int broj, int id, const Termin *novi) {
    Termin *termin;

    if (lista == NULL || broj <= 0 || id <= 0 || novi == NULL) {
        return ERR_NULL_PTR;
    }

    termin = pronadji_termin_po_id(lista, broj, id);
    if (termin == NULL) {
        return ERR_NOT_FOUND;
    }
    if (!je_validan_id(novi->id_klijenta) ||
        !je_validan_id(novi->id_frizera) ||
        !je_validan_id(novi->id_usluge) ||
        !je_validna_cijena(novi->uplacena_cijena) ||
        !je_validan_datum(novi->datum_vrijeme.datum.dan,
                          novi->datum_vrijeme.datum.mjesec,
                          novi->datum_vrijeme.datum.godina) ||
        !je_validno_vrijeme_termina(novi->datum_vrijeme.vrijeme.sati,
                                    novi->datum_vrijeme.vrijeme.minute)) {
        return ERR_INVALID_INPUT;
    }
    if (!je_frizer_dostupan(lista, broj, novi->id_frizera, novi->datum_vrijeme, id)) {
        return ERR_DUPLICATE;
    }

    termin->id_klijenta = novi->id_klijenta;
    termin->id_frizera = novi->id_frizera;
    termin->id_usluge = novi->id_usluge;
    termin->datum_vrijeme = novi->datum_vrijeme;
    termin->uplacena_cijena = novi->uplacena_cijena;
    termin->status = novi->status;
    (void)snprintf(termin->napomena, sizeof(termin->napomena), "%s", novi->napomena);
    g_promjene_nisu_spremljene = 1;
    return ERR_OK;
}

int obrisi_termin(Termin **lista, int *broj, int id) {
    int indeks = -1;

    if (lista == NULL || broj == NULL || id <= 0) {
        return ERR_NULL_PTR;
    }
    if (*broj <= 0 || *lista == NULL) {
        return ERR_NOT_FOUND;
    }

    for (int i = 0; i < *broj; i++) {
        if ((*lista)[i].id == id) {
            indeks = i;
            break;
        }
    }

    if (indeks == -1) {
        return ERR_NOT_FOUND;
    }

    for (int i = indeks; i < *broj - 1; i++) {
        (*lista)[i] = (*lista)[i + 1];
    }

    (*broj)--;
    (void)smanji_memoriju_termina(lista, *broj);
    g_ukupno_termina = *broj;
    g_promjene_nisu_spremljene = 1;
    return ERR_OK;
}

int je_frizer_dostupan(const Termin *lista, int broj, int id_frizera, DatumVrijeme kada, int zanemari_id_termina) {
    if (id_frizera <= 0) {
        return 0;
    }
    if (lista == NULL || broj <= 0) {
        return 1;
    }

    for (int i = 0; i < broj; i++) {
        if (lista[i].id == zanemari_id_termina) {
            continue;
        }
        if (lista[i].id_frizera == id_frizera &&
            lista[i].status == TERMIN_ZAUZET &&
            usporedi_datum_vrijeme(lista[i].datum_vrijeme, kada) == 0) {
            return 0;
        }
    }

    return 1;
}

void ispisi_termin(const Termin *termin) {
    if (termin == NULL) {
        printf("Nema podataka.\n");
        return;
    }

    printf("ID: %d\n", termin->id);
    printf("Klijent ID: %d | Frizer ID: %d | Usluga ID: %d\n", termin->id_klijenta,
           termin->id_frizera, termin->id_usluge);
    printf("Datum: %02d.%02d.%04d.\n", termin->datum_vrijeme.datum.dan,
           termin->datum_vrijeme.datum.mjesec, termin->datum_vrijeme.datum.godina);
    printf("Vrijeme: %02d:%02d\n", termin->datum_vrijeme.vrijeme.sati, termin->datum_vrijeme.vrijeme.minute);
    printf("Cijena: %.2f EUR\n", termin->uplacena_cijena);
    printf("Status: %s\n", status_termina_u_string(termin->status));
    printf("Napomena: %s\n", termin->napomena);
}

void ispisi_sve_termine(const Termin *lista, int broj) {
    if (lista == NULL || broj <= 0) {
        printf("Nema termina.\n");
        return;
    }

    printf("\n%-5s %-8s %-8s %-8s %-12s %-8s %-12s\n", "ID", "Klijent", "Frizer", "Usluga", "Datum", "Vrijeme", "Status");
    for (int i = 0; i < broj; i++) {
        printf("%-5d %-8d %-8d %-8d %02d.%02d.%04d %-2d:%02d    %-12s\n",
               lista[i].id, lista[i].id_klijenta, lista[i].id_frizera, lista[i].id_usluge,
               lista[i].datum_vrijeme.datum.dan, lista[i].datum_vrijeme.datum.mjesec,
               lista[i].datum_vrijeme.datum.godina, lista[i].datum_vrijeme.vrijeme.sati,
               lista[i].datum_vrijeme.vrijeme.minute, status_termina_u_string(lista[i].status));
    }
}

void ispisi_termine_dana(const Termin *lista, int broj, Datum dan) {
    int pronadeno = 0;

    if (lista == NULL || broj <= 0) {
        printf("Nema termina.\n");
        return;
    }

    for (int i = 0; i < broj; i++) {
        if (lista[i].datum_vrijeme.datum.dan == dan.dan &&
            lista[i].datum_vrijeme.datum.mjesec == dan.mjesec &&
            lista[i].datum_vrijeme.datum.godina == dan.godina) {
            ispisi_termin(&lista[i]);
            pronadeno = 1;
        }
    }

    if (!pronadeno) {
        printf("Nema termina za zadani datum.\n");
    }
}

void ispisi_sortirane_termine(Termin *lista, int broj) {
    Termin *kopija;

    if (lista == NULL || broj <= 0) {
        printf("Nema termina.\n");
        return;
    }

    kopija = (Termin *)malloc((size_t)broj * sizeof(Termin));
    if (kopija == NULL) {
        printf("Greska pri alokaciji memorije.\n");
        return;
    }

    memcpy(kopija, lista, (size_t)broj * sizeof(Termin));
    qsort(kopija, (size_t)broj, sizeof(Termin), usporedi_termine_po_datumu);
    ispisi_sve_termine(kopija, broj);
    free(kopija);
}

int usporedi_termine_po_datumu(const void *a, const void *b) {
    const Termin *ta = (const Termin *)a;
    const Termin *tb = (const Termin *)b;
    return usporedi_datum_vrijeme(ta->datum_vrijeme, tb->datum_vrijeme);
}

int usporedi_termine_po_id(const void *a, const void *b) {
    const Termin *ta = (const Termin *)a;
    const Termin *tb = (const Termin *)b;
    return (ta->id > tb->id) - (ta->id < tb->id);
}

int prikupi_podatke_termina(Termin *termin) {
    int status;

    if (termin == NULL) {
        return ERR_NULL_PTR;
    }

    memset(termin, 0, sizeof(Termin));

    if (ucitaj_int("ID klijenta: ", &termin->id_klijenta) != ERR_OK || termin->id_klijenta <= 0) {
        return ERR_INVALID_INPUT;
    }
    if (ucitaj_int("ID frizera: ", &termin->id_frizera) != ERR_OK || termin->id_frizera <= 0) {
        return ERR_INVALID_INPUT;
    }
    if (ucitaj_int("ID usluge: ", &termin->id_usluge) != ERR_OK || termin->id_usluge <= 0) {
        return ERR_INVALID_INPUT;
    }
    if (ucitaj_int("Dan: ", &termin->datum_vrijeme.datum.dan) != ERR_OK ||
        ucitaj_int("Mjesec: ", &termin->datum_vrijeme.datum.mjesec) != ERR_OK ||
        ucitaj_int("Godina: ", &termin->datum_vrijeme.datum.godina) != ERR_OK ||
        !je_validan_datum(termin->datum_vrijeme.datum.dan, termin->datum_vrijeme.datum.mjesec,
                          termin->datum_vrijeme.datum.godina)) {
        return ERR_INVALID_INPUT;
    }
    if (ucitaj_int("Sati: ", &termin->datum_vrijeme.vrijeme.sati) != ERR_OK ||
        ucitaj_int("Minute: ", &termin->datum_vrijeme.vrijeme.minute) != ERR_OK ||
        !je_validno_vrijeme_termina(termin->datum_vrijeme.vrijeme.sati, termin->datum_vrijeme.vrijeme.minute)) {
        return ERR_INVALID_INPUT;
    }
    if (ucitaj_float("Cijena (EUR): ", &termin->uplacena_cijena) != ERR_OK ||
        !je_validna_cijena(termin->uplacena_cijena)) {
        return ERR_INVALID_INPUT;
    }
    if (ucitaj_int("Status (1 zauzet, 2 otkazan, 3 odraden): ", &status) != ERR_OK ||
        (status != TERMIN_ZAUZET && status != TERMIN_OTKAZAN && status != TERMIN_ODRADEN)) {
        return ERR_INVALID_INPUT;
    }
    termin->status = (StatusTermina)status;

    if (ucitaj_string("Napomena: ", termin->napomena, sizeof(termin->napomena)) != ERR_OK) {
        return ERR_INVALID_INPUT;
    }

    return ERR_OK;
}

int pronadji_max_id_termina(const Termin *lista, int broj) {
    int max_id = 0;

    if (lista == NULL || broj <= 0) {
        return 0;
    }
    for (int i = 0; i < broj; i++) {
        if (lista[i].id > max_id) {
            max_id = lista[i].id;
        }
    }
    return max_id;
}
