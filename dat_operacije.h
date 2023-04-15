#ifndef DAT_OPERACIJE_H_INCLUDED
#define DAT_OPERACIJE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#include "strukture_.h"

FILE *otvoriDatoteku(char *filename);
void kreirajDatoteku(char *filename);
SLOG *pronadjiSlog(FILE *fajl,int sifra);
void dodajSlog(FILE *fajl, SLOG *slog);
void ispisiSveSlogove(FILE *fajl);
void ispisiSlog(SLOG *slog);
void obrisiSlogLogicki(FILE *fajl, int sifra);
void obrisiSlogFizicki(FILE *fajl, int sifra);
void PovecajCeneUverenjaPetrovica(FILE* fajl);


#endif // DAT_OPERACIJE_H_INCLUDED
