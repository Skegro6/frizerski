#include "frizeri.h"
#include "config.h"
#include "globals.h"
#include "utils.h"
#include "validacija.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int smanji_memoriju_frizera(Frizer **lista, int broj) {
    Frizer *temp;

    if (lista == NULL) {
        return ERR_NULL_PTR;
    }
    if (broj == 0) {
        SAFE_FREE(*lista);
        return ERR_OK;
    }

    temp = (Frizer *)realloc(*lista, (size_t)broj * sizeof(Frizer));
    if (temp != NULL) {
        *lista = temp;
    }
    return ERR_OK;
}

int dodaj_frizera(Frizer **lista, int *broj, const Frizer *novi_frizer) {
    Frizer *temp;

    if (lista == NULL || broj == NULL || novi_frizer == NULL) {
        return ERR_NULL_PTR;
    }
    if (*broj < 0 || *broj >= MAX_FRIZERA) {
        return ERR_INVALID_RANGE;
    }
    if (*broj > 0 && *lista == NULL) {
        return ERR_NULL_PTR;
    }

    temp = (Frizer *)realloc(*lista, (size_t)(*broj + 1) * sizeof(Frizer));
    if (temp == NULL) {
        return ERR_MEMORY;
    }

    *lista = temp;
    (*lista)[*broj] = *novi_frizer;
    (*lista)[*broj].id = generiraj_novi_id(&g_last_frizer_id);
    (*broj)++;
    g_ukupno_frizera = *broj;
    g_promjene_nisu_spremljene = 1;
    return ERR_OK;
}

Frizer *pronadji_frizera_po_id(Frizer *lista, int broj, int id) {
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

Frizer *pronadji_frizera_bsearch(Frizer *lista, int broj, int id) {
    Frizer kljuc;

    if (lista == NULL || broj <= 0 || id <= 0) {
        return NULL;
    }

    qsort(lista, (size_t)broj, sizeof(Frizer), usporedi_frizere_po_id);
    memset(&kljuc, 0, sizeof(Frizer));
    kljuc.id = id;
    return (Frizer *)bsearch(&kljuc, lista, (size_t)broj, sizeof(Frizer), usporedi_frizere_po_id);
}

Frizer *pronadji_frizera_po_prezimenu(Frizer *lista, int broj, const char *prezime) {
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

int uredi_frizera(Frizer *lista, int broj, int id, const Frizer *novi) {
    Frizer *frizer;

    if (lista == NULL || broj <= 0 || id <= 0 || novi == NULL) {
        return ERR_NULL_PTR;
    }

    frizer = pronadji_frizera_po_id(lista, broj, id);
    if (frizer == NULL) {
        return ERR_NOT_FOUND;
    }

    (void)snprintf(frizer->ime, sizeof(frizer->ime), "%s", novi->ime);
    (void)snprintf(frizer->prezime, sizeof(frizer->prezime), "%s", novi->prezime);
    (void)snprintf(frizer->telefon, sizeof(frizer->telefon), "%s", novi->telefon);
    (void)snprintf(frizer->specijalnost, sizeof(frizer->specijalnost), "%s", novi->specijalnost);
    frizer->placa = novi->placa;
    frizer->status = novi->status;
    g_promjene_nisu_spremljene = 1;
    return ERR_OK;
}

int obrisi_frizera(Frizer **lista, int *broj, int id) {
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
    (void)smanji_memoriju_frizera(lista, *broj);
    g_ukupno_frizera = *broj;
    g_promjene_nisu_spremljene = 1;
    return ERR_OK;
}

void ispisi_frizera(const Frizer *frizer) {
    if (frizer == NULL) {
        printf("Nema podataka.\n");
        return;
    }

    printf("ID: %d\n", frizer->id);
    printf("Ime i prezime: %s %s\n", frizer->ime, frizer->prezime);
    printf("Telefon: %s\n", frizer->telefon);
    printf("Specijalnost: %s\n", frizer->specijalnost);
    printf("Placa: %.2f EUR\n", frizer->placa);
    printf("Status: %s\n", status_zaposlenika_u_string(frizer->status));
}

void ispisi_sve_frizere(const Frizer *lista, int broj) {
    if (lista == NULL || broj <= 0) {
        printf("Nema frizera.\n");
        return;
    }

    printf("\n%-5s %-15s %-15s %-20s %-15s\n", "ID", "Ime", "Prezime", "Specijalnost", "Status");
    for (int i = 0; i < broj; i++) {
        printf("%-5d %-15s %-15s %-20s %-15s\n", lista[i].id, lista[i].ime,
               lista[i].prezime, lista[i].specijalnost, status_zaposlenika_u_string(lista[i].status));
    }
}

void ispisi_sortirane_frizere(Frizer *lista, int broj) {
    Frizer *kopija;

    if (lista == NULL || broj <= 0) {
        printf("Nema frizera.\n");
        return;
    }

    kopija = (Frizer *)malloc((size_t)broj * sizeof(Frizer));
    if (kopija == NULL) {
        printf("Greska pri alokaciji memorije.\n");
        return;
    }

    memcpy(kopija, lista, (size_t)broj * sizeof(Frizer));
    qsort(kopija, (size_t)broj, sizeof(Frizer), usporedi_frizere_po_prezimenu);
    ispisi_sve_frizere(kopija, broj);
    free(kopija);
}

int usporedi_frizere_po_prezimenu(const void *a, const void *b) {
    const Frizer *fa = (const Frizer *)a;
    const Frizer *fb = (const Frizer *)b;
    int rezultat = strcmp(fa->prezime, fb->prezime);

    if (rezultat == 0) {
        rezultat = strcmp(fa->ime, fb->ime);
    }
    return rezultat;
}

int usporedi_frizere_po_id(const void *a, const void *b) {
    const Frizer *fa = (const Frizer *)a;
    const Frizer *fb = (const Frizer *)b;
    return (fa->id > fb->id) - (fa->id < fb->id);
}

int prikupi_podatke_frizera(Frizer *frizer) {
    int status;

    if (frizer == NULL) {
        return ERR_NULL_PTR;
    }

    memset(frizer, 0, sizeof(Frizer));

    if (ucitaj_string("Ime: ", frizer->ime, sizeof(frizer->ime)) != ERR_OK ||
        !je_validan_string(frizer->ime, 2, MAX_IME - 1)) {
        return ERR_INVALID_INPUT;
    }
    if (ucitaj_string("Prezime: ", frizer->prezime, sizeof(frizer->prezime)) != ERR_OK ||
        !je_validan_string(frizer->prezime, 2, MAX_PREZIME - 1)) {
        return ERR_INVALID_INPUT;
    }
    if (ucitaj_string("Telefon: ", frizer->telefon, sizeof(frizer->telefon)) != ERR_OK ||
        !je_validan_telefon(frizer->telefon)) {
        return ERR_INVALID_INPUT;
    }
    if (ucitaj_string("Specijalnost: ", frizer->specijalnost, sizeof(frizer->specijalnost)) != ERR_OK ||
        !je_validan_string(frizer->specijalnost, 2, MAX_SPECIJALNOST - 1)) {
        return ERR_INVALID_INPUT;
    }
    if (ucitaj_float("Placa (EUR): ", &frizer->placa) != ERR_OK || frizer->placa < 0.0f) {
        return ERR_INVALID_INPUT;
    }
    if (ucitaj_int("Status (1 aktivan, 0 neaktivan, 2 godisnji): ", &status) != ERR_OK ||
        (status != STATUS_AKTIVAN && status != STATUS_NEAKTIVAN && status != STATUS_NA_GODISNJEM)) {
        return ERR_INVALID_INPUT;
    }

    frizer->status = (StatusZaposlenika)status;
    frizer->datum_zaposljavanja = trenutno_datum_vrijeme();
    return ERR_OK;
}

int pronadji_max_id_frizera(const Frizer *lista, int broj) {
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
