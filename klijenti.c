#include "klijenti.h"
#include "config.h"
#include "globals.h"
#include "utils.h"
#include "validacija.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int smanji_memoriju_klijenata(Klijent **lista, int broj) {
    Klijent *temp;

    if (lista == NULL) {
        return ERR_NULL_PTR;
    }

    if (broj == 0) {
        SAFE_FREE(*lista);
        return ERR_OK;
    }

    temp = (Klijent *)realloc(*lista, (size_t)broj * sizeof(Klijent));
    if (temp != NULL) {
        *lista = temp;
    }

    return ERR_OK;
}

int dodaj_klijenta(Klijent **lista, int *broj, const Klijent *novi_klijent) {
    Klijent *temp;

    if (lista == NULL || broj == NULL || novi_klijent == NULL) {
        return ERR_NULL_PTR;
    }
    if (*broj < 0 || *broj >= MAX_KLIJENTI) {
        return ERR_INVALID_RANGE;
    }
    if (*broj > 0 && *lista == NULL) {
        return ERR_NULL_PTR;
    }

    for (int i = 0; i < *broj; i++) {
        if (strcmp((*lista)[i].telefon, novi_klijent->telefon) == 0) {
            return ERR_DUPLICATE;
        }
    }

    temp = (Klijent *)realloc(*lista, (size_t)(*broj + 1) * sizeof(Klijent));
    if (temp == NULL) {
        return ERR_MEMORY;
    }

    *lista = temp;
    (*lista)[*broj] = *novi_klijent;
    (*lista)[*broj].id = generiraj_novi_id(&g_last_klijent_id);
    (*broj)++;
    g_ukupno_klijenata = *broj;
    g_promjene_nisu_spremljene = 1;
    return ERR_OK;
}

Klijent *pronadji_klijenta_po_id(Klijent *lista, int broj, int id) {
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

Klijent *pronadji_klijenta_bsearch(Klijent *lista, int broj, int id) {
    Klijent kljuc;

    if (lista == NULL || broj <= 0 || id <= 0) {
        return NULL;
    }

    qsort(lista, (size_t)broj, sizeof(Klijent), usporedi_klijente_po_id);
    memset(&kljuc, 0, sizeof(Klijent));
    kljuc.id = id;

    return (Klijent *)bsearch(&kljuc, lista, (size_t)broj, sizeof(Klijent), usporedi_klijente_po_id);
}

Klijent *pronadji_klijenta_po_prezimenu(Klijent *lista, int broj, const char *prezime) {
    if (lista == NULL || broj <= 0 || prezime == NULL) {
        return NULL;
    }

    for (int i = 0; i < broj; i++) {
        if (strcmp(lista[i].prezime, prezime) == 0) {
            return &lista[i];
        }
    }

    return NULL;
}

int uredi_klijenta(Klijent *lista, int broj, int id, const Klijent *novi) {
    Klijent *klijent;

    if (lista == NULL || broj <= 0 || id <= 0 || novi == NULL) {
        return ERR_NULL_PTR;
    }

    klijent = pronadji_klijenta_po_id(lista, broj, id);
    if (klijent == NULL) {
        return ERR_NOT_FOUND;
    }

    (void)snprintf(klijent->ime, sizeof(klijent->ime), "%s", novi->ime);
    (void)snprintf(klijent->prezime, sizeof(klijent->prezime), "%s", novi->prezime);
    (void)snprintf(klijent->telefon, sizeof(klijent->telefon), "%s", novi->telefon);
    (void)snprintf(klijent->email, sizeof(klijent->email), "%s", novi->email);
    (void)snprintf(klijent->adresa, sizeof(klijent->adresa), "%s", novi->adresa);
    g_promjene_nisu_spremljene = 1;
    return ERR_OK;
}

int obrisi_klijenta(Klijent **lista, int *broj, int id) {
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
    (void)smanji_memoriju_klijenata(lista, *broj);
    g_ukupno_klijenata = *broj;
    g_promjene_nisu_spremljene = 1;
    return ERR_OK;
}

void ispisi_klijenta(const Klijent *klijent) {
    if (klijent == NULL) {
        printf("Nema podataka.\n");
        return;
    }

    printf("ID: %d\n", klijent->id);
    printf("Ime i prezime: %s %s\n", klijent->ime, klijent->prezime);
    printf("Telefon: %s\n", klijent->telefon);
    printf("Email: %s\n", klijent->email);
    printf("Adresa: %s\n", klijent->adresa);
}

void ispisi_sve_klijente(const Klijent *lista, int broj) {
    if (lista == NULL || broj <= 0) {
        printf("Nema klijenata.\n");
        return;
    }

    printf("\n%-5s %-15s %-15s %-20s %-25s\n", "ID", "Ime", "Prezime", "Telefon", "Email");
    for (int i = 0; i < broj; i++) {
        printf("%-5d %-15s %-15s %-20s %-25s\n", lista[i].id, lista[i].ime,
               lista[i].prezime, lista[i].telefon, lista[i].email);
    }
}

void ispisi_sortirane_klijente(Klijent *lista, int broj) {
    Klijent *kopija;

    if (lista == NULL || broj <= 0) {
        printf("Nema klijenata.\n");
        return;
    }

    kopija = (Klijent *)malloc((size_t)broj * sizeof(Klijent));
    if (kopija == NULL) {
        printf("Greska pri alokaciji memorije.\n");
        return;
    }

    memcpy(kopija, lista, (size_t)broj * sizeof(Klijent));
    qsort(kopija, (size_t)broj, sizeof(Klijent), usporedi_klijente_po_prezimenu);
    ispisi_sve_klijente(kopija, broj);
    free(kopija);
}

int usporedi_klijente_po_prezimenu(const void *a, const void *b) {
    const Klijent *ka = (const Klijent *)a;
    const Klijent *kb = (const Klijent *)b;
    int rezultat = strcmp(ka->prezime, kb->prezime);

    if (rezultat == 0) {
        rezultat = strcmp(ka->ime, kb->ime);
    }
    return rezultat;
}

int usporedi_klijente_po_id(const void *a, const void *b) {
    const Klijent *ka = (const Klijent *)a;
    const Klijent *kb = (const Klijent *)b;
    return (ka->id > kb->id) - (ka->id < kb->id);
}

int prikupi_podatke_klijenta(Klijent *klijent) {
    if (klijent == NULL) {
        return ERR_NULL_PTR;
    }

    memset(klijent, 0, sizeof(Klijent));

    if (ucitaj_string("Ime: ", klijent->ime, sizeof(klijent->ime)) != ERR_OK ||
        !je_validan_string(klijent->ime, 2, MAX_IME - 1)) {
        return ERR_INVALID_INPUT;
    }
    if (ucitaj_string("Prezime: ", klijent->prezime, sizeof(klijent->prezime)) != ERR_OK ||
        !je_validan_string(klijent->prezime, 2, MAX_PREZIME - 1)) {
        return ERR_INVALID_INPUT;
    }
    if (ucitaj_string("Telefon: ", klijent->telefon, sizeof(klijent->telefon)) != ERR_OK ||
        !je_validan_telefon(klijent->telefon)) {
        return ERR_INVALID_INPUT;
    }
    if (ucitaj_string("Email: ", klijent->email, sizeof(klijent->email)) != ERR_OK ||
        !je_validan_email(klijent->email)) {
        return ERR_INVALID_INPUT;
    }
    if (ucitaj_string("Adresa: ", klijent->adresa, sizeof(klijent->adresa)) != ERR_OK) {
        return ERR_INVALID_INPUT;
    }

    klijent->datum_registracije = trenutno_datum_vrijeme();
    return ERR_OK;
}

int pronadji_max_id_klijenta(const Klijent *lista, int broj) {
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
