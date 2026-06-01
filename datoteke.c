#include "datoteke.h"
#include "config.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int napravi_privremenu_putanju(const char *putanja, char *buffer, size_t velicina) {
    if (putanja == NULL || buffer == NULL || velicina == 0U) {
        return ERR_NULL_PTR;
    }

    if (snprintf(buffer, velicina, "%s.tmp", putanja) >= (int)velicina) {
        return ERR_INVALID_RANGE;
    }

    return ERR_OK;
}

static int spremi_zapise(const char *putanja, const void *lista, int broj, size_t velicina_zapisa) {
    FILE *fp;
    char privremena[MAX_PATH];
    int kod_zatvaranja;

    if (putanja == NULL || broj < 0 || velicina_zapisa == 0U) {
        return ERR_NULL_PTR;
    }
    if (broj > 0 && lista == NULL) {
        return ERR_NULL_PTR;
    }

    if (napravi_privremenu_putanju(putanja, privremena, sizeof(privremena)) != ERR_OK) {
        return ERR_INVALID_RANGE;
    }

    fp = fopen(privremena, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Ne mogu otvoriti %s: %s\n", privremena, strerror(errno));
        return ERR_FILE_OPEN;
    }

    if (fwrite(&broj, sizeof(int), 1, fp) != 1) {
        perror("fwrite broj");
        (void)fclose(fp);
        (void)remove(privremena);
        return ERR_FILE_WRITE;
    }

    if (broj > 0 && fwrite(lista, velicina_zapisa, (size_t)broj, fp) != (size_t)broj) {
        perror("fwrite zapisi");
        (void)fclose(fp);
        (void)remove(privremena);
        return ERR_FILE_WRITE;
    }

    kod_zatvaranja = fclose(fp);
    if (kod_zatvaranja != 0) {
        perror("fclose");
        (void)remove(privremena);
        return ERR_FILE_WRITE;
    }

    return sigurno_zamijeni_datoteku(privremena, putanja);
}

static int ucitaj_zapise(const char *putanja, void **lista, int *broj, size_t velicina_zapisa) {
    FILE *fp;
    int procitani_broj = 0;
    void *memorija = NULL;
    long velicina;

    if (putanja == NULL || lista == NULL || broj == NULL || velicina_zapisa == 0U) {
        return ERR_NULL_PTR;
    }

    *lista = NULL;
    *broj = 0;

    fp = fopen(putanja, "rb");
    if (fp == NULL) {
        if (errno == ENOENT) {
            return ERR_OK;
        }
        fprintf(stderr, "Ne mogu otvoriti %s: %s\n", putanja, strerror(errno));
        return ERR_FILE_OPEN;
    }

    if (fseek(fp, 0L, SEEK_END) != 0) {
        perror("fseek");
        fclose(fp);
        return ERR_FILE_READ;
    }

    velicina = ftell(fp);
    if (velicina < 0) {
        perror("ftell");
        fclose(fp);
        return ERR_FILE_READ;
    }

    rewind(fp);

    if (velicina == 0) {
        fclose(fp);
        return ERR_OK;
    }

    if (velicina < (long)sizeof(int)) {
        fprintf(stderr, "Datoteka %s je ostecena: nedostaje broj zapisa.\n", putanja);
        fclose(fp);
        return ERR_FILE_READ;
    }

    if (fread(&procitani_broj, sizeof(int), 1, fp) != 1) {
        if (feof(fp)) {
            fclose(fp);
            return ERR_OK;
        }
        perror("fread broj");
        fclose(fp);
        return ERR_FILE_READ;
    }

    if (procitani_broj < 0) {
        fclose(fp);
        return ERR_FILE_READ;
    }

    if (procitani_broj == 0) {
        fclose(fp);
        return ERR_OK;
    }

    if ((long)sizeof(int) + (long)procitani_broj * (long)velicina_zapisa > velicina) {
        fprintf(stderr, "Datoteka %s je ostecena: nedostaju zapisi.\n", putanja);
        fclose(fp);
        return ERR_FILE_READ;
    }

    memorija = calloc((size_t)procitani_broj, velicina_zapisa);
    if (memorija == NULL) {
        fclose(fp);
        return ERR_MEMORY;
    }

    if (fread(memorija, velicina_zapisa, (size_t)procitani_broj, fp) != (size_t)procitani_broj) {
        if (ferror(fp)) {
            perror("fread zapisi");
        }
        free(memorija);
        fclose(fp);
        return ERR_FILE_READ;
    }

    if (fclose(fp) != 0) {
        perror("fclose");
        free(memorija);
        return ERR_FILE_READ;
    }
    *lista = memorija;
    *broj = procitani_broj;
    return ERR_OK;
}

int spremi_klijente(const char *putanja, const Klijent *lista, int broj) {
    return spremi_zapise(putanja, lista, broj, sizeof(Klijent));
}

int ucitaj_klijente(const char *putanja, Klijent **lista, int *broj) {
    return ucitaj_zapise(putanja, (void **)lista, broj, sizeof(Klijent));
}

int spremi_frizere(const char *putanja, const Frizer *lista, int broj) {
    return spremi_zapise(putanja, lista, broj, sizeof(Frizer));
}

int ucitaj_frizere(const char *putanja, Frizer **lista, int *broj) {
    return ucitaj_zapise(putanja, (void **)lista, broj, sizeof(Frizer));
}

int spremi_usluge(const char *putanja, const Usluga *lista, int broj) {
    return spremi_zapise(putanja, lista, broj, sizeof(Usluga));
}

int ucitaj_usluge(const char *putanja, Usluga **lista, int *broj) {
    return ucitaj_zapise(putanja, (void **)lista, broj, sizeof(Usluga));
}

int spremi_termine(const char *putanja, const Termin *lista, int broj) {
    return spremi_zapise(putanja, lista, broj, sizeof(Termin));
}

int ucitaj_termine(const char *putanja, Termin **lista, int *broj) {
    return ucitaj_zapise(putanja, (void **)lista, broj, sizeof(Termin));
}

int datoteka_postoji(const char *putanja) {
    FILE *fp;

    if (putanja == NULL) {
        return 0;
    }

    fp = fopen(putanja, "rb");
    if (fp == NULL) {
        return 0;
    }

    fclose(fp);
    return 1;
}

long velicina_datoteke(const char *putanja) {
    FILE *fp;
    long velicina;

    if (putanja == NULL) {
        return -1L;
    }

    fp = fopen(putanja, "rb");
    if (fp == NULL) {
        return -1L;
    }

    if (fseek(fp, 0L, SEEK_END) != 0) {
        fclose(fp);
        return -1L;
    }

    velicina = ftell(fp);
    rewind(fp);
    fclose(fp);
    return velicina;
}

int obrisi_datoteku(const char *putanja) {
    if (putanja == NULL) {
        return ERR_NULL_PTR;
    }

    if (remove(putanja) != 0) {
        if (errno == ENOENT) {
            return ERR_OK;
        }
        perror("remove");
        return ERR_FILE_OPEN;
    }

    return ERR_OK;
}

int kopiraj_datoteku(const char *izvor, const char *odrediste) {
    FILE *src;
    FILE *dst;
    char buffer[4096];
    size_t procitano;

    if (izvor == NULL || odrediste == NULL) {
        return ERR_NULL_PTR;
    }

    src = fopen(izvor, "rb");
    if (src == NULL) {
        perror("kopiraj fopen src");
        return ERR_FILE_OPEN;
    }

    dst = fopen(odrediste, "wb");
    if (dst == NULL) {
        perror("kopiraj fopen dst");
        fclose(src);
        return ERR_FILE_OPEN;
    }

    while ((procitano = fread(buffer, 1U, sizeof(buffer), src)) > 0U) {
        if (fwrite(buffer, 1U, procitano, dst) != procitano) {
            perror("kopiraj fwrite");
            fclose(src);
            fclose(dst);
            return ERR_FILE_WRITE;
        }
    }

    if (ferror(src)) {
        perror("kopiraj fread");
        fclose(src);
        fclose(dst);
        return ERR_FILE_READ;
    }

    if (fclose(src) != 0) {
        perror("kopiraj fclose src");
        fclose(dst);
        return ERR_FILE_READ;
    }
    if (fclose(dst) != 0) {
        perror("kopiraj fclose dst");
        return ERR_FILE_WRITE;
    }
    return ERR_OK;
}

int sigurno_zamijeni_datoteku(const char *privremena, const char *originalna) {
    if (privremena == NULL || originalna == NULL) {
        return ERR_NULL_PTR;
    }

    if (remove(originalna) != 0 && errno != ENOENT) {
        perror("remove originalna");
        return ERR_FILE_OPEN;
    }

    if (rename(privremena, originalna) != 0) {
        perror("rename privremena");
        return ERR_FILE_WRITE;
    }

    return ERR_OK;
}
