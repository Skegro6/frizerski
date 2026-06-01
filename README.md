# Frizerski salon - C projekt

Projekt je modularno napisan u C jeziku i sadrzi CRUD sustav za:
- klijente
- frizere
- usluge
- termine

## Pokriveni koncepti

- `typedef struct`, `typedef enum`, `typedef union`
- globalne varijable preko `extern`
- `static` funkcije i lokalna static varijabla
- makro `SAFE_FREE`
- `static inline` funkcija
- izbornici i podizbornici
- pokazivaci i zastita parametara
- dinamicka memorija: `malloc`, `calloc`, `realloc`, `free`
- sigurno oslobadanje memorije i postavljanje pokazivaca na `NULL`
- binarne datoteke
- `fopen`, `fclose`, `fread`, `fwrite`
- `fseek`, `ftell`, `rewind`
- `remove`, `rename`
- `errno`, `perror`, `strerror`, `feof`, `ferror`
- `qsort`, `bsearch`
- pokazivaci na funkcije kroz `qsort` i `bsearch`
- jednostruko povezani popis za logove sesije

## Kompajliranje

```bash
make
```

## Pokretanje

```bash
make run
```

## Ciscenje

```bash
make clean
```
