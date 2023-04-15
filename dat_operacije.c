#include "dat_operacije.h"

FILE *otvoriDatoteku(char *filename) {
	FILE *fajl = fopen(filename, "rb+");
	if (fajl == NULL) {
		printf("Doslo je do greske! Moguce da datoteka \"%s\" ne postoji.\n", filename);
	} else {
		printf("Datoteka \"%s\" otvorena.\n", filename);
	}
	return fajl;
}

void kreirajDatoteku(char *filename) {
	FILE *fajl = fopen(filename, "wb");
	if (fajl == NULL) {
		printf("Doslo je do greske prilikom kreiranja datoteke \"%s\"!\n", filename);
	} else {

		BLOK blok;
		blok.slogovi[0].sifraUverenja=OZNAKA_KRAJA_DATOTEKE;
		fwrite(&blok, sizeof(BLOK), 1, fajl);
		printf("Datoteka \"%s\" uspesno kreirana.\n", filename);
		fclose(fajl);
	}
}

SLOG *pronadjiSlog(FILE *fajl, int sifra) {
	if (fajl == NULL) {
		return NULL;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;

	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraUverenja== OZNAKA_KRAJA_DATOTEKE) {

				return NULL;
			}

			if (blok.slogovi[i].sifraUverenja== sifra && !blok.slogovi[i].deleted) {


				SLOG *slog = (SLOG *)malloc(sizeof(SLOG));
				memcpy(slog, &blok.slogovi[i], sizeof(SLOG));
				return slog;
			}
		}
	}

	return NULL;
}

void dodajSlog(FILE *fajl, SLOG *slog) {
	if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	SLOG *slogStari = pronadjiSlog(fajl, slog->sifraUverenja);
	if (slogStari != NULL) {

        printf("Vec postoji slog sa tim evid brojem!\n");
        return;
    }

	BLOK blok;
	fseek(fajl, -sizeof(BLOK), SEEK_END);
	fread(&blok, sizeof(BLOK), 1, fajl);

	int i;
	for (i = 0; i < FBLOKIRANJA; i++) {
		if (blok.slogovi[i].sifraUverenja== OZNAKA_KRAJA_DATOTEKE) {

			memcpy(&blok.slogovi[i], slog, sizeof(SLOG));
			break;
		}
	}

	i++;

	if (i < FBLOKIRANJA) {

		blok.slogovi[i].sifraUverenja= OZNAKA_KRAJA_DATOTEKE;


		fseek(fajl, -sizeof(BLOK), SEEK_CUR);
		fwrite(&blok, sizeof(BLOK), 1, fajl);
	} else {


		fseek(fajl, -sizeof(BLOK), SEEK_CUR);
		fwrite(&blok, sizeof(BLOK), 1, fajl);


		BLOK noviBlok;
		noviBlok.slogovi[0].sifraUverenja= OZNAKA_KRAJA_DATOTEKE;

		fwrite(&noviBlok, sizeof(BLOK), 1, fajl);
	}

	if (ferror(fajl)) {
		printf("Greska pri upisu u fajl!\n");
	} else {
		printf("Upis novog sloga zavrsen.\n");
	}
}

void ispisiSveSlogove(FILE *fajl) {
	if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	int rbBloka = 0;
	printf("        SifraUverenja         PrzMeh   Cena            PrzVlasnika VrstaVozila\n");
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraUverenja== OZNAKA_KRAJA_DATOTEKE){
				printf("B%d S%d *\n", rbBloka, i);
                break;
			}
			if (!blok.slogovi[i].deleted) {
                printf("B%d S%d ", rbBloka, i);
                ispisiSlog(&blok.slogovi[i]);
                printf("\n");
            }
		}
		rbBloka++;
	}
}

void ispisiSlog(SLOG *slog) {
	printf("%13d    %15s %6f %11s %4s",

        slog->sifraUverenja,
		slog->prezimeMehanicara,
		slog->cena,
		slog->prezimeVlasnika,
		slog->oznakaVrsteVozila);
}


void obrisiSlogLogicki(FILE *fajl, int sifra) {
	if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {

            if (blok.slogovi[i].sifraUverenja==OZNAKA_KRAJA_DATOTEKE) {
                printf("Nema tog sloga u datoteci\n");
                return;
            }

			if (blok.slogovi[i].sifraUverenja== sifra && !blok.slogovi[i].deleted) {

				blok.slogovi[i].deleted = 1;
				fseek(fajl, -sizeof(BLOK), SEEK_CUR);
				fwrite(&blok, sizeof(BLOK), 1, fajl);

                printf("Slog je logicki obrisan.\n");
				return;
			}
		}
	}
}

void obrisiSlogFizicki(FILE *fajl, int sifra) {


    SLOG *slog = pronadjiSlog(fajl, sifra);
    if (slog == NULL) {
        printf("Slog koji zelite obrisati ne postoji!\n");
        return;
    }


    BLOK blok, naredniBlok;
    int sifraZaBrisanje=sifra;

    fseek(fajl, 0, SEEK_SET);
    while (fread(&blok, 1, sizeof(BLOK), fajl)) {
        for (int i = 0; i < FBLOKIRANJA; i++) {

            if (blok.slogovi[i].sifraUverenja==OZNAKA_KRAJA_DATOTEKE) {

                if (i == 0) {

                    printf("(skracujem fajl...)\n");
                    fseek(fajl, -sizeof(BLOK), SEEK_END);
					long bytesToKeep = ftell(fajl);
					ftruncate(fileno(fajl), bytesToKeep);
					fflush(fajl);
                }

                printf("Slog je fizicki obrisan.\n");
                return;
            }

            if (blok.slogovi[i].sifraUverenja== sifraZaBrisanje){

                if (blok.slogovi[i].sifraUverenja==sifra && blok.slogovi[i].deleted) {

                    continue;
                }


                for (int j = i+1; j < FBLOKIRANJA; j++) {
                    memcpy(&(blok.slogovi[j-1]), &(blok.slogovi[j]), sizeof(SLOG));
                }


                int podatakaProcitano = fread(&naredniBlok, sizeof(BLOK), 1, fajl);

                if (podatakaProcitano) {

                    fseek(fajl, -sizeof(BLOK), SEEK_CUR);

                    memcpy(&(blok.slogovi[FBLOKIRANJA-1]), &(naredniBlok.slogovi[0]), sizeof(SLOG));

                   sifraZaBrisanje= naredniBlok.slogovi[0].sifraUverenja;
                }

                fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                fwrite(&blok, sizeof(BLOK), 1, fajl);
                fseek(fajl, 0, SEEK_CUR);

                if (!podatakaProcitano) {
                    printf("Slog je fizicki obrisan.\n");
                    return;
                }

                break;
            }

        }
    }
}
void UkupanBrojUverenjaMehanicara(FILE* fajl,char* mehanicar)
{
     if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}
	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	int b=0;
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraUverenja== OZNAKA_KRAJA_DATOTEKE){
                break;
			}
			if (!blok.slogovi[i].deleted) {
                    if(strcmp(blok.slogovi[i].prezimeMehanicara,mehanicar)==0)
                    {
                        b++;
                    }
            }
		}

    }
     printf("Ukupno uverenja: %d\n",b);
}
void PovecajCeneUverenjaPetrovica(FILE* fajl){
    if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}
	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraUverenja== OZNAKA_KRAJA_DATOTEKE){
                return;
			}
			if (!blok.slogovi[i].deleted) {
                    if(strcmp(blok.slogovi[i].prezimeMehanicara,"Petrovic")==0)
                    {
                        blok.slogovi[i].cena+=1000;



                    }
            }
		}
		fseek(fajl, -sizeof(BLOK), SEEK_CUR);
        fwrite(&blok, sizeof(BLOK), 1, fajl);
        fseek(fajl,0,SEEK_CUR);
	}

}
typedef struct{
    char vrsta[3];
    float sumaCena;
    int cnt;
}VRSTA;
void IspisProsecnihCenaPoVrsti(FILE* fajl)
{
     if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	int rbBloka = 0;
	VRSTA v[20];
	int brojac=0;
	int pok=0;
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraUverenja== OZNAKA_KRAJA_DATOTEKE){
                break;
			}
			if (!blok.slogovi[i].deleted) {
                   for(int j=0;j<brojac;j++)
                   {
                       if(strcmp(v[j].vrsta,blok.slogovi[i].oznakaVrsteVozila)==0)
                       {
                           v[j].cnt+=1;
                           v[j].sumaCena+=blok.slogovi[i].cena;
                           pok=1;
                       }
                   }
                   if(pok==0)
                   {
                       VRSTA vv;
                       strcpy(vv.vrsta,blok.slogovi[i].oznakaVrsteVozila);
                       vv.cnt=1;
                       vv.sumaCena=blok.slogovi[i].cena;
                       v[brojac]=vv;
                       brojac++;
                   }
                   pok=0;
            }
		}
	}
	for(int i=0;i<brojac;i++)
    {
        printf("Vrsta: %s \n     %lf\n",v[i].vrsta,v[i].sumaCena/(double)v[i].cnt);
    }
}
