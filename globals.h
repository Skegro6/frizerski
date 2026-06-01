#ifndef GLOBALS_H
#define GLOBALS_H

#include "tipovi.h"

extern int g_ukupno_klijenata;
extern int g_ukupno_frizera;
extern int g_ukupno_usluga;
extern int g_ukupno_termina;

extern char g_naziv_salona[100];
extern char g_adresa_salona[MAX_ADRESA];
extern char g_telefon_salona[MAX_TELEFON];
extern char g_email_salona[MAX_EMAIL];

extern int g_last_klijent_id;
extern int g_last_frizer_id;
extern int g_last_usluga_id;
extern int g_last_termin_id;

extern float g_ukupan_prihod;
extern int g_ukupno_odradenih_termina;
extern int g_promjene_nisu_spremljene;

int generiraj_novi_id(int *last_id);
void inicijaliziraj_globalne_varijable(void);
void ispisi_globalne_informacije(void);
void azuriraj_brojace(int broj_klijenata, int broj_frizera, int broj_usluga, int broj_termina);
void resetiraj_globalne_varijable(void);
int ucitaj_globalne_varijable(const char *putanja);
int spremi_globalne_varijable(const char *putanja);

#endif
