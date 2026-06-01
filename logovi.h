#ifndef LOGOVI_H
#define LOGOVI_H

typedef struct LogCvor {
    char poruka[128];
    struct LogCvor *sljedeci;
} LogCvor;

int dodaj_log(LogCvor **glava, const char *poruka);
void ispisi_logove(const LogCvor *glava);
void oslobodi_logove(LogCvor **glava);

#endif
