#ifndef STRUKTURE__H_INCLUDED
#define STRUKTURE__H_INCLUDED
#define FBLOKIRANJA 3
#define OZNAKA_KRAJA_DATOTEKE -1


typedef struct {
int sifraUverenja;
char prezimeMehanicara[11];
float cena;
char prezimeVlasnika[11];
char oznakaVrsteVozila[3];
int deleted;
} SLOG;


typedef struct Blok {
	SLOG slogovi[FBLOKIRANJA];
} BLOK;




#endif // STRUKTURE__H_INCLUDED
