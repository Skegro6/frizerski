#ifndef CONFIG_H
#define CONFIG_H

#define MAX_KLIJENTI        1000
#define MAX_FRIZERA         100
#define MAX_USLUGA          50
#define MAX_TERMINA         5000

#define MAX_IME             50
#define MAX_PREZIME         50
#define MAX_TELEFON         20
#define MAX_EMAIL           100
#define MAX_ADRESA          100
#define MAX_SPECIJALNOST    50
#define MAX_NAZIV_USLUGE    40
#define MAX_NAPOMENA        100
#define MAX_PATH            256
#define MAX_UNOS            256

#define RADNI_SATI_OD       8
#define RADNI_SATI_DO       20

#define PATH_KLIJENTI       "data/klijenti.dat"
#define PATH_FRIZERI        "data/frizeri.dat"
#define PATH_USLUGE         "data/usluge.dat"
#define PATH_TERMINI        "data/termini.dat"
#define PATH_POSTAVKE       "data/postavke.dat"

#define ERR_OK              0
#define ERR_NULL_PTR        -1
#define ERR_FILE_OPEN       -2
#define ERR_FILE_READ       -3
#define ERR_FILE_WRITE      -4
#define ERR_MEMORY          -5
#define ERR_NOT_FOUND       -6
#define ERR_INVALID_INPUT   -7
#define ERR_DUPLICATE       -8
#define ERR_INVALID_RANGE   -9

#define STATUS_OK(x)        ((x) == ERR_OK)

#endif
