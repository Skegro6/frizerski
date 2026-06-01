#include "globals.h"
#include "config.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

int g_ukupno_klijenata = 0;
int g_ukupno_frizera = 0;
int g_ukupno_usluga = 0;
int g_ukupno_termina = 0;

char g_naziv_salona[100] = "Salon Frizura";
char g_adresa_salona[MAX_ADRESA] = "Ulica 1, Zagreb";
char g_telefon_salona[MAX_TELEFON] = "+385 1 123 4567";
char g_email_salona[MAX_EMAIL] = "salon@frizura.hr";

int g_last_klijent_id = 0;
int g_last_frizer_id = 0;
int g_last_usluga_id = 0;
int g_last_termin_id = 0;

float g_ukupan_prihod = 0.0f;
int g_ukupno_odradenih_termina = 0;
int g_promjene_nisu_spremljene = 0;

static int s_brojac_generiranja_id = 0;

int generiraj_novi_id(int *last_id) {
    if (last_id == NULL) {
        return ERR_NULL_PTR;
    }

    s_brojac_generiranja_id++;
    (*last_id)++;
    g_promjene_nisu_spremljene = 1;
    return *last_id;
}

void inicijaliziraj_globalne_varijable(void) {
    g_ukupno_klijenata = 0;
    g_ukupno_frizera = 0;
    g_ukupno_usluga = 0;
    g_ukupno_termina = 0;

    g_last_klijent_id = 0;
    g_last_frizer_id = 0;
    g_last_usluga_id = 0;
    g_last_termin_id = 0;

    g_ukupan_prihod = 0.0f;
    g_ukupno_odradenih_termina = 0;
    g_promjene_nisu_spremljene = 0;
    s_brojac_generiranja_id = 0;

    (void)snprintf(g_naziv_salona, sizeof(g_naziv_salona), "%s", "Salon Frizura");
    (void)snprintf(g_adresa_salona, sizeof(g_adresa_salona), "%s", "Ulica 1, Zagreb");
    (void)snprintf(g_telefon_salona, sizeof(g_telefon_salona), "%s", "+385 1 123 4567");
    (void)snprintf(g_email_salona, sizeof(g_email_salona), "%s", "salon@frizura.hr");
}

void ispisi_globalne_informacije(void) {
    printf("\n=== GLOBALNE INFORMACIJE ===\n");
    printf("Salon: %s\n", g_naziv_salona);
    printf("Adresa: %s\n", g_adresa_salona);
    printf("Telefon: %s\n", g_telefon_salona);
    printf("Email: %s\n", g_email_salona);
    printf("Klijenti: %d | Frizeri: %d | Usluge: %d | Termini: %d\n",
           g_ukupno_klijenata, g_ukupno_frizera, g_ukupno_usluga, g_ukupno_termina);
    printf("Prihod: %.2f EUR | Odradeni termini: %d\n", g_ukupan_prihod, g_ukupno_odradenih_termina);
    printf("Broj generiranih ID-eva u sesiji: %d\n", s_brojac_generiranja_id);
}

void azuriraj_brojace(int broj_klijenata, int broj_frizera, int broj_usluga, int broj_termina) {
    if (broj_klijenata >= 0) {
        g_ukupno_klijenata = broj_klijenata;
    }
    if (broj_frizera >= 0) {
        g_ukupno_frizera = broj_frizera;
    }
    if (broj_usluga >= 0) {
        g_ukupno_usluga = broj_usluga;
    }
    if (broj_termina >= 0) {
        g_ukupno_termina = broj_termina;
    }
}

void resetiraj_globalne_varijable(void) {
    inicijaliziraj_globalne_varijable();
}

int ucitaj_globalne_varijable(const char *putanja) {
    FILE *fp;

    if (putanja == NULL) {
        return ERR_NULL_PTR;
    }

    fp = fopen(putanja, "rb");
    if (fp == NULL) {
        if (errno == ENOENT) {
            return ERR_OK;
        }
        perror("ucitaj_globalne_varijable fopen");
        return ERR_FILE_OPEN;
    }

    if (fread(&g_ukupno_klijenata, sizeof(int), 1, fp) != 1 ||
        fread(&g_ukupno_frizera, sizeof(int), 1, fp) != 1 ||
        fread(&g_ukupno_usluga, sizeof(int), 1, fp) != 1 ||
        fread(&g_ukupno_termina, sizeof(int), 1, fp) != 1) {
        perror("ucitaj_globalne_varijable fread");
        fclose(fp);
        return ERR_FILE_READ;
    }

    fclose(fp);
    return ERR_OK;
}

int spremi_globalne_varijable(const char *putanja) {
    FILE *fp;

    if (putanja == NULL) {
        return ERR_NULL_PTR;
    }

    fp = fopen(putanja, "wb");
    if (fp == NULL) {
        perror("spremi_globalne_varijable fopen");
        return ERR_FILE_OPEN;
    }

    if (fwrite(&g_ukupno_klijenata, sizeof(int), 1, fp) != 1 ||
        fwrite(&g_ukupno_frizera, sizeof(int), 1, fp) != 1 ||
        fwrite(&g_ukupno_usluga, sizeof(int), 1, fp) != 1 ||
        fwrite(&g_ukupno_termina, sizeof(int), 1, fp) != 1) {
        perror("spremi_globalne_varijable fwrite");
        fclose(fp);
        return ERR_FILE_WRITE;
    }

    fclose(fp);
    g_promjene_nisu_spremljene = 0;
    return ERR_OK;
}
