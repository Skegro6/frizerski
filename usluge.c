#include "usluge.h"
#include "config.h"
#include "globals.h"
#include "utils.h"
#include "validacija.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int smanji_memoriju_usluga(Usluga **lista, int broj) {
    Usluga *temp;

    if (lista == NULL) {
        return ERR_NULL_PTR;
    }
    if (broj == 0) {
        SAFE_FREE(*lista);
        return ERR_OK;
    }

    temp = (Usluga *)realloc(*lista, (size_t)broj * sizeof(Usluga));
    if (temp != NULL) {
        *lista = temp;
    }
    return ERR_OK;
}

int dodaj_uslugu(Usluga **lista, int *broj, const Usluga *nova_usluga) {
    Usluga *temp;

    if (lista == NULL || broj == NULL || nova_usluga == NULL) {
        return ERR_NULL_PTR;
    }
    if (*broj < 0 || *broj >= MAX_USLUGA) {
        return ERR_INVALID_RANGE;
    }
    if (*broj > 0 && *lista == NULL) {
        return ERR_NULL_PTR;
    }

    temp = (Usluga *)realloc(*lista, (size_t)(*broj + 1) * sizeof(Usluga));
    if (temp == NULL) {
        return ERR_MEMORY;
    }

    *lista = temp;
    (*lista)[*broj] = *nova_usluga;
    (*lista)[*broj].id = generiraj_novi_id(&g_last_usluga_id);
    (*broj)++;
    g_ukupno_usluga = *broj;
    g_promjene_nisu_spremljene = 1;
    return ERR_OK;
}

Usluga *pronadji_uslugu_po_id(Usluga *lista, int broj, int id) {
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

Usluga *pronadji_uslugu_bsearch(Usluga *lista, int broj, int id) {
    Usluga kljuc;

    if (lista == NULL || broj <= 0 || id <= 0) {
        return NULL;
    }

    qsort(lista, (size_t)broj, sizeof(Usluga), usporedi_usluge_po_id);
    memset(&kljuc, 0, sizeof(Usluga));
    kljuc.id = id;
    return (Usluga *)bsearch(&kljuc, lista, (size_t)broj, sizeof(Usluga), usporedi_usluge_po_id);
}

Usluga *pronadji_uslugu_po_nazivu(Usluga *lista, int broj, const char *naziv) {
    if (lista == NULL || broj <= 0 || naziv == NULL) {
        return NULL;
    }
    for (int i = 0; i < broj; i++) {
        if (strcmp(lista[i].naziv, naziv) == 0) {
            return &lista[i];
        }
    }
    return NULL;
}

int uredi_uslugu(Usluga *lista, int broj, int id, const Usluga *nova) {
    Usluga *usluga;

    if (lista == NULL || broj <= 0 || id <= 0 || nova == NULL) {
        return ERR_NULL_PTR;
    }

    usluga = pronadji_uslugu_po_id(lista, broj, id);
    if (usluga == NULL) {
        return ERR_NOT_FOUND;
    }

    (void)snprintf(usluga->naziv, sizeof(usluga->naziv), "%s", nova->naziv);
    usluga->cijena = nova->cijena;
    usluga->trajanje_minuta = nova->trajanje_minuta;
    usluga->vrsta = nova->vrsta;
    g_promjene_nisu_spremljene = 1;
    return ERR_OK;
}

int obrisi_uslugu(Usluga **lista, int *broj, int id) {
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
    (void)smanji_memoriju_usluga(lista, *broj);
    g_ukupno_usluga = *broj;
    g_promjene_nisu_spremljene = 1;
    return ERR_OK;
}

void ispisi_uslugu(const Usluga *usluga) {
    if (usluga == NULL) {
        printf("Nema podataka.\n");
        return;
    }

    printf("ID: %d\n", usluga->id);
    printf("Naziv: %s\n", usluga->naziv);
    printf("Vrsta: %s\n", vrsta_usluge_u_string(usluga->vrsta));
    printf("Cijena: %.2f EUR\n", usluga->cijena);
    printf("Trajanje: %d min\n", usluga->trajanje_minuta);
}

void ispisi_sve_usluge(const Usluga *lista, int broj) {
    if (lista == NULL || broj <= 0) {
        printf("Nema usluga.\n");
        return;
    }

    printf("\n%-5s %-25s %-15s %-12s %-10s\n", "ID", "Naziv", "Vrsta", "Cijena", "Trajanje");
    for (int i = 0; i < broj; i++) {
        printf("%-5d %-25s %-15s %-12.2f %-10d\n", lista[i].id, lista[i].naziv,
               vrsta_usluge_u_string(lista[i].vrsta), lista[i].cijena, lista[i].trajanje_minuta);
    }
}

void ispisi_sortirane_usluge(Usluga *lista, int broj) {
    Usluga *kopija;

    if (lista == NULL || broj <= 0) {
        printf("Nema usluga.\n");
        return;
    }

    kopija = (Usluga *)malloc((size_t)broj * sizeof(Usluga));
    if (kopija == NULL) {
        printf("Greska pri alokaciji memorije.\n");
        return;
    }

    memcpy(kopija, lista, (size_t)broj * sizeof(Usluga));
    qsort(kopija, (size_t)broj, sizeof(Usluga), usporedi_usluge_po_cijeni);
    ispisi_sve_usluge(kopija, broj);
    free(kopija);
}

int usporedi_usluge_po_cijeni(const void *a, const void *b) {
    const Usluga *ua = (const Usluga *)a;
    const Usluga *ub = (const Usluga *)b;
    return (ua->cijena > ub->cijena) - (ua->cijena < ub->cijena);
}

int usporedi_usluge_po_id(const void *a, const void *b) {
    const Usluga *ua = (const Usluga *)a;
    const Usluga *ub = (const Usluga *)b;
    return (ua->id > ub->id) - (ua->id < ub->id);
}

int prikupi_podatke_usluge(Usluga *usluga) {
    int vrsta;

    if (usluga == NULL) {
        return ERR_NULL_PTR;
    }

    memset(usluga, 0, sizeof(Usluga));

    if (ucitaj_string("Naziv usluge: ", usluga->naziv, sizeof(usluga->naziv)) != ERR_OK ||
        !je_validan_string(usluga->naziv, 2, MAX_NAZIV_USLUGE - 1)) {
        return ERR_INVALID_INPUT;
    }
    if (ucitaj_float("Cijena (EUR): ", &usluga->cijena) != ERR_OK ||
        !je_validna_cijena(usluga->cijena)) {
        return ERR_INVALID_INPUT;
    }
    if (ucitaj_int("Trajanje (minute): ", &usluga->trajanje_minuta) != ERR_OK ||
        usluga->trajanje_minuta <= 0) {
        return ERR_INVALID_INPUT;
    }
    if (ucitaj_int("Vrsta (1 sisanje, 2 bojanje, 3 pranje, 4 tretman, 5 brijanje, 6 modeliranje): ", &vrsta) != ERR_OK ||
        vrsta < SISANJE || vrsta > MODELIRANJE) {
        return ERR_INVALID_INPUT;
    }

    usluga->vrsta = (VrstaUsluge)vrsta;
    return ERR_OK;
}

int pronadji_max_id_usluge(const Usluga *lista, int broj) {
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
