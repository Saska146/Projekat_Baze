#include <stdio.h>
#include <stdlib.h>
#include "dat_operacije.h"
int main()
{
    int running = 1;
	int userInput;

	FILE *fajl = NULL;

	while (running) {
		printf("Odaberite opciju:\n");
		printf("1 - Otvaranje datoteke\n");
		printf("2 - Formiranje datoteke\n");
		printf("3 - Pretraga datoteke\n");
		printf("4 - Unos sloga\n");
		printf("5 - Ispis svih slogova\n");
		printf("6 - Broj uverenja za odredjenog mehanicara\n");
		printf("7 - Brisanje sloga (fizicki)\n");
		printf("8 - Povecaj cenu uverenja mehanicara Petrovic\n");
		printf("9 - Prikazi prosek cene za svaku vrstu vozila\n");
		printf("0 - Izlaz\n");
		if (fajl == NULL) {
			printf("!!! PAZNJA: datoteka jos uvek nije otvorena !!!\n");
		}
		scanf("%d", &userInput);
		getc(stdin);

		switch(userInput) {
			case 1:
				{
					char filename[20];
					printf("Unesite ime datoteke za otvaranje: ");
					scanf("%s", &filename[0]);
					fajl = otvoriDatoteku(filename);
					printf("\n");
					break;
				}
			case 2:
				{
					char filename[20];
					printf("Unesite ime datoteke za kreiranje: ");
					scanf("%s", filename);
					kreirajDatoteku(filename);
					printf("\n");
					break;
				}
			case 3:
				{
					int sifra;
					printf("Unesite evid. broj trazenog prijema: ");
					scanf("%d", &sifra);
					SLOG *slog = pronadjiSlog(fajl, sifra);
					if (slog == NULL) {
                        printf("Nema tog sloga u datoteci.\n");
					} else {
                        printf("SifraUverenja   PrezimeMehanicara DatumIzdavanja Cena PrezimeVlasnika VrstaVozila\n");
                        ispisiSlog(slog);
                        printf("\n");
					}
					printf("\n");
					break;
				}
			case 4:
				{
					SLOG slog;
					printf("Sifra uverenja: ");
					scanf("%d", &slog.sifraUverenja);
					printf("Prezime mehanicara (10 karaktera): ");
					scanf("%s", slog.prezimeMehanicara);
					printf("Cena pregleda: ");
					scanf("%f", &slog.cena);
					printf("Prezime vlasnika vozila (10 karaktera): ");
					scanf("%s", slog.prezimeVlasnika);
					printf("Oznaka vrste vozila (2 karaktera): ");
					scanf("%s", slog.oznakaVrsteVozila);
					slog.deleted = 0;
					dodajSlog(fajl, &slog);
					printf("\n");
					break;
				}
			case 5:
				{
					ispisiSveSlogove(fajl);
					printf("\n");
					break;
				}
			case 6:
				{
				    char pr[11];
                    printf("Unesite prezime: \n");
                    scanf("%s",pr);
                    UkupanBrojUverenjaMehanicara(fajl,pr);
					break;
				}
			case 7:
				{
					int evidBroj;
					printf("Unesite evid. broj sloga za logicko brisanje: ");
					scanf("%d", &evidBroj);
					obrisiSlogLogicki(fajl, evidBroj);
					printf("\n");
					break;
				}
			case 8:
				{
                    PovecajCeneUverenjaPetrovica(fajl);
					break;
				}
            case 9:
				{
				   IspisProsecnihCenaPoVrsti(fajl);
					break;
				}

			case 0:
				{
					running = 0;
					if (fajl != NULL) {
						fclose(fajl);
					}
				}
		}
	}

    return 0;
}
