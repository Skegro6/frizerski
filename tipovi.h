#ifndef TIPOVI_H
#define TIPOVI_H

#include "config.h"

typedef enum {
    SISANJE = 1,
    BOJANJE,
    PRANJE,
    TRETMAN,
    BRIJANJE,
    MODELIRANJE
} VrstaUsluge;

typedef enum {
    STATUS_NEAKTIVAN = 0,
    STATUS_AKTIVAN = 1,
    STATUS_NA_GODISNJEM = 2
} StatusZaposlenika;

typedef enum {
    TERMIN_SLOBODAN = 0,
    TERMIN_ZAUZET = 1,
    TERMIN_OTKAZAN = 2,
    TERMIN_ODRADEN = 3
} StatusTermina;

typedef enum {
    GLAVNI_IZLAZ = 0,
    GLAVNI_KLIJENTI = 1,
    GLAVNI_FRIZERI = 2,
    GLAVNI_USLUGE = 3,
    GLAVNI_TERMINI = 4,
    GLAVNI_IZVJESTAJI = 5,
    GLAVNI_POSTAVKE = 6
} GlavniIzbornik;

typedef enum {
    CRUD_NATRAG = 0,
    CRUD_DODAJ = 1,
    CRUD_PREGLED = 2,
    CRUD_UREDI = 3,
    CRUD_OBRISI = 4,
    CRUD_TRAZI = 5,
    CRUD_SORTIRAJ = 6
} OpcijaCrud;

typedef enum {
    PRETRAGA_PO_ID = 1,
    PRETRAGA_PO_TEKSTU = 2
} VrstaPretrage;

typedef struct {
    int dan;
    int mjesec;
    int godina;
} Datum;

typedef struct {
    int sati;
    int minute;
} Vrijeme;

typedef struct {
    Datum datum;
    Vrijeme vrijeme;
} DatumVrijeme;

typedef union {
    int id;
    char tekst[MAX_PREZIME];
} KljucPretrage;

typedef struct {
    int id;
    char ime[MAX_IME];
    char prezime[MAX_PREZIME];
    char telefon[MAX_TELEFON];
    char email[MAX_EMAIL];
    char adresa[MAX_ADRESA];
    DatumVrijeme datum_registracije;
} Klijent;

typedef struct {
    int id;
    char ime[MAX_IME];
    char prezime[MAX_PREZIME];
    char telefon[MAX_TELEFON];
    StatusZaposlenika status;
    char specijalnost[MAX_SPECIJALNOST];
    float placa;
    DatumVrijeme datum_zaposljavanja;
} Frizer;

typedef struct {
    int id;
    char naziv[MAX_NAZIV_USLUGE];
    float cijena;
    int trajanje_minuta;
    VrstaUsluge vrsta;
} Usluga;

typedef struct {
    int id;
    int id_klijenta;
    int id_frizera;
    int id_usluge;
    DatumVrijeme datum_vrijeme;
    float uplacena_cijena;
    StatusTermina status;
    char napomena[MAX_NAPOMENA];
} Termin;

#endif
