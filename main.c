/*
 * Frizerski salon - sustav za upravljanje
 * Programiranje 2, C projekt
 */

#include "config.h"
#include "datoteke.h"
#include "frizeri.h"
#include "globals.h"
#include "izbornik.h"
#include "klijenti.h"
#include "logovi.h"
#include "termini.h"
#include "usluge.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

static void postavi_zadnje_idjeve(const Klijent *klijenti, int broj_klijenata,
                                  const Frizer *frizeri, int broj_frizera,
                                  const Usluga *usluge, int broj_usluga,
                                  const Termin *termini, int broj_termina) {
    g_last_klijent_id = pronadji_max_id_klijenta(klijenti, broj_klijenata);
    g_last_frizer_id = pronadji_max_id_frizera(frizeri, broj_frizera);
    g_last_usluga_id = pronadji_max_id_usluge(usluge, broj_usluga);
    g_last_termin_id = pronadji_max_id_termina(termini, broj_termina);
}

static int ucitaj_sve_podatke(Klijent **klijenti, int *broj_klijenata,
                              Frizer **frizeri, int *broj_frizera,
                              Usluga **usluge, int *broj_usluga,
                              Termin **termini, int *broj_termina) {
    int kod;

    kod = ucitaj_klijente(PATH_KLIJENTI, klijenti, broj_klijenata);
    if (kod != ERR_OK) {
        return kod;
    }
    kod = ucitaj_frizere(PATH_FRIZERI, frizeri, broj_frizera);
    if (kod != ERR_OK) {
        SAFE_FREE(*klijenti);
        *broj_klijenata = 0;
        return kod;
    }
    kod = ucitaj_usluge(PATH_USLUGE, usluge, broj_usluga);
    if (kod != ERR_OK) {
        SAFE_FREE(*klijenti);
        SAFE_FREE(*frizeri);
        *broj_klijenata = 0;
        *broj_frizera = 0;
        return kod;
    }
    kod = ucitaj_termine(PATH_TERMINI, termini, broj_termina);
    if (kod != ERR_OK) {
        SAFE_FREE(*klijenti);
        SAFE_FREE(*frizeri);
        SAFE_FREE(*usluge);
        *broj_klijenata = 0;
        *broj_frizera = 0;
        *broj_usluga = 0;
        return kod;
    }

    azuriraj_brojace(*broj_klijenata, *broj_frizera, *broj_usluga, *broj_termina);
    postavi_zadnje_idjeve(*klijenti, *broj_klijenata, *frizeri, *broj_frizera,
                          *usluge, *broj_usluga, *termini, *broj_termina);
    g_promjene_nisu_spremljene = 0;
    return ERR_OK;
}

static int spremi_sve_podatke(const Klijent *klijenti, int broj_klijenata,
                              const Frizer *frizeri, int broj_frizera,
                              const Usluga *usluge, int broj_usluga,
                              const Termin *termini, int broj_termina) {
    int kod;

    kod = spremi_klijente(PATH_KLIJENTI, klijenti, broj_klijenata);
    if (kod != ERR_OK) {
        return kod;
    }
    kod = spremi_frizere(PATH_FRIZERI, frizeri, broj_frizera);
    if (kod != ERR_OK) {
        return kod;
    }
    kod = spremi_usluge(PATH_USLUGE, usluge, broj_usluga);
    if (kod != ERR_OK) {
        return kod;
    }
    kod = spremi_termine(PATH_TERMINI, termini, broj_termina);
    if (kod != ERR_OK) {
        return kod;
    }

    g_promjene_nisu_spremljene = 0;
    return ERR_OK;
}

static void oslobodi_svu_memoriju(Klijent **klijenti, Frizer **frizeri,
                                  Usluga **usluge, Termin **termini,
                                  LogCvor **logovi) {
    if (klijenti != NULL) {
        SAFE_FREE(*klijenti);
    }
    if (frizeri != NULL) {
        SAFE_FREE(*frizeri);
    }
    if (usluge != NULL) {
        SAFE_FREE(*usluge);
    }
    if (termini != NULL) {
        SAFE_FREE(*termini);
    }
    oslobodi_logove(logovi);
}

static void ispisi_zaglavlje_programa(void) {
    printf("========================================\n");
    printf("        SALON FRIZURA - C PROJEKT       \n");
    printf("========================================\n");
}

int main(void) {
    Klijent *klijenti = NULL;
    Frizer *frizeri = NULL;
    Usluga *usluge = NULL;
    Termin *termini = NULL;
    LogCvor *logovi = NULL;

    int broj_klijenata = 0;
    int broj_frizera = 0;
    int broj_usluga = 0;
    int broj_termina = 0;
    int opcija = -1;
    int kod;

    inicijaliziraj_globalne_varijable();
    ispisi_zaglavlje_programa();

    kod = ucitaj_sve_podatke(&klijenti, &broj_klijenata, &frizeri, &broj_frizera,
                             &usluge, &broj_usluga, &termini, &broj_termina);
    if (kod != ERR_OK) {
        printf("Ucitavanje nije uspjelo, kod: %d\n", kod);
    }

    (void)dodaj_log(&logovi, "Program pokrenut i podaci ucitani.");

    while (opcija != GLAVNI_IZLAZ) {
        prikazi_glavni_izbornik();
        if (ucitaj_int("Odabir: ", &opcija) != ERR_OK) {
            printf("Neispravan unos.\n");
            continue;
        }

        switch ((GlavniIzbornik)opcija) {
            case GLAVNI_KLIJENTI:
                prikazi_izbornik_klijenti(&klijenti, &broj_klijenata);
                (void)dodaj_log(&logovi, "Otvoren izbornik klijenata.");
                break;
            case GLAVNI_FRIZERI:
                prikazi_izbornik_frizeri(&frizeri, &broj_frizera);
                (void)dodaj_log(&logovi, "Otvoren izbornik frizera.");
                break;
            case GLAVNI_USLUGE:
                prikazi_izbornik_usluge(&usluge, &broj_usluga);
                (void)dodaj_log(&logovi, "Otvoren izbornik usluga.");
                break;
            case GLAVNI_TERMINI:
                prikazi_izbornik_termini(&termini, &broj_termina, klijenti, broj_klijenata,
                                         frizeri, broj_frizera, usluge, broj_usluga);
                (void)dodaj_log(&logovi, "Otvoren izbornik termina.");
                break;
            case GLAVNI_IZVJESTAJI:
                prikazi_izbornik_izvjestaji(klijenti, broj_klijenata, frizeri, broj_frizera,
                                            usluge, broj_usluga, termini, broj_termina);
                break;
            case GLAVNI_POSTAVKE:
                prikazi_izbornik_postavke();
                printf("\nLogovi sesije:\n");
                ispisi_logove(logovi);
                break;
            case GLAVNI_IZLAZ:
                kod = spremi_sve_podatke(klijenti, broj_klijenata, frizeri, broj_frizera,
                                         usluge, broj_usluga, termini, broj_termina);
                if (kod == ERR_OK) {
                    printf("Podaci su spremljeni.\n");
                } else {
                    printf("Spremanje nije uspjelo, kod: %d\n", kod);
                }
                break;
            default:
                printf("Nepostojeca opcija.\n");
                break;
        }
    }

    oslobodi_svu_memoriju(&klijenti, &frizeri, &usluge, &termini, &logovi);
    printf("Dovidenja!\n");
    return EXIT_SUCCESS;
}
