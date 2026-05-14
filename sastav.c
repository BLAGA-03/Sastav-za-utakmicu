#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_IME 50
#define MAX_PREZIME 50
#define MAX_POZICIJA 30

typedef struct {
    char ime[MAX_IME];
    char prezime[MAX_PREZIME];
    int broj;
    char pozicija[MAX_POZICIJA];
} Igrac;

void ocistiBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void unesiTekst(char *polje, int velicina) {
    if (fgets(polje, velicina, stdin) == NULL) {
        polje[0] = '\0';
        return;
    }
    size_t len = strlen(polje);
    if (len > 0 && polje[len - 1] == '\n') {
        polje[len - 1] = '\0';
    }
}

void ispisiSastav(const Igrac *sastav, int brojIgraca) {
    if (brojIgraca == 0) {
        printf("\nNema unesenih igraca.\n");
        return;
    }

    printf("\n%-4s %-15s %-15s %-6s %-10s\n", "RB", "Ime", "Prezime", "Broj", "Pozicija");
    printf("-----------------------------------------------------\n");
    for (int i = 0; i < brojIgraca; i++) {
        printf("%-4d %-15s %-15s %-6d %-10s\n", i + 1,
               sastav[i].ime, sastav[i].prezime, sastav[i].broj, sastav[i].pozicija);
    }
}

void obrisiIgraca(Igrac **sastav, int *brojIgraca) {
    if (*brojIgraca == 0) {
        printf("\nNema igraca za brisanje.\n");
        return;
    }

    ispisiSastav(*sastav, *brojIgraca);
    printf("Unesite RB igraca kojeg zelite obrisati: ");
    int rb;
    if (scanf("%d", &rb) != 1) {
        printf("Neispravan unos!\n");
        ocistiBuffer();
        return;
    }
    ocistiBuffer();

    if (rb < 1 || rb > *brojIgraca) {
        printf("Nepostojeci redni broj.\n");
        return;
    }

    int index = rb - 1;
    for (int i = index; i < *brojIgraca - 1; i++) {
        (*sastav)[i] = (*sastav)[i + 1];
    }

    (*brojIgraca)--;
    if (*brojIgraca > 0) {
        Igrac *novi = realloc(*sastav, (*brojIgraca) * sizeof(Igrac));
        if (novi != NULL) {
            *sastav = novi;
        }
    } else {
        free(*sastav);
        *sastav = NULL;
    }

    printf("Igrac je obrisan.\n");
}

void spremiUDatoteku(const Igrac *sastav, int brojIgraca) {
    FILE *fp = fopen("sastav.txt", "w");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke za pisanje!\n");
        return;
    }
    fprintf(fp, "%d\n", brojIgraca);
    for (int i = 0; i < brojIgraca; i++) {
        fprintf(fp, "%s\n%s\n%d\n%s\n",
                sastav[i].ime, sastav[i].prezime, sastav[i].broj, sastav[i].pozicija);
    }
    fclose(fp);
    printf("Podaci su uspjesno spremljeni u 'sastav.txt'.\n");
}

Igrac *ucitajIzDatoteke(int *brojIgraca) {
    FILE *fp = fopen("sastav.txt", "r");
    if (fp == NULL) {
        *brojIgraca = 0;
        return NULL;
    }

    if (fscanf(fp, "%d", brojIgraca) != 1) {
        fclose(fp);
        *brojIgraca = 0;
        return NULL;
    }
    int c = fgetc(fp);
    if (c != '\n' && c != EOF) {
        while ((c = fgetc(fp)) != '\n' && c != EOF);
    }

    if (*brojIgraca <= 0) {
        fclose(fp);
        *brojIgraca = 0;
        return NULL;
    }

    Igrac *sastav = malloc((*brojIgraca) * sizeof(Igrac));
    if (sastav == NULL) {
        fclose(fp);
        *brojIgraca = 0;
        return NULL;
    }

    for (int i = 0; i < *brojIgraca; i++) {
        if (fgets(sastav[i].ime, MAX_IME, fp) == NULL) {
            sastav[i].ime[0] = '\0';
        } else {
            sastav[i].ime[strcspn(sastav[i].ime, "\n")] = '\0';
        }

        if (fgets(sastav[i].prezime, MAX_PREZIME, fp) == NULL) {
            sastav[i].prezime[0] = '\0';
        } else {
            sastav[i].prezime[strcspn(sastav[i].prezime, "\n")] = '\0';
        }

        if (fscanf(fp, "%d", &sastav[i].broj) != 1) {
            sastav[i].broj = 0;
        }
        c = fgetc(fp);
        if (c != '\n' && c != EOF) {
            while ((c = fgetc(fp)) != '\n' && c != EOF);
        }

        if (fgets(sastav[i].pozicija, MAX_POZICIJA, fp) == NULL) {
            sastav[i].pozicija[0] = '\0';
        } else {
            sastav[i].pozicija[strcspn(sastav[i].pozicija, "\n")] = '\0';
        }
    }

    fclose(fp);
    return sastav;
}

int main(void) {
    int brojIgraca = 0;
    int izbor;
    Igrac *sastav = ucitajIzDatoteke(&brojIgraca);
    if (sastav != NULL) {
        printf("Ucitano %d igraca iz datoteke.\n", brojIgraca);
    }

    do {
        printf("\n=== Izbornik: Sastav za utakmicu ===\n");
        printf("1. Dodaj igraca\n");
        printf("2. Ispisi sastav\n");
        printf("3. Obrisi igraca\n");
        printf("4. Spremi i izlaz\n");
        printf("Odaberite opciju: ");

        if (scanf("%d", &izbor) != 1) {
            printf("Neispravan unos!\n");
            ocistiBuffer();
            continue;
        }
        ocistiBuffer();

        switch (izbor) {
            case 1:
                brojIgraca++;
                Igrac *novi = realloc(sastav, brojIgraca * sizeof(Igrac));
                if (novi == NULL) {
                    printf("Greska kod alokacije memorije!\n");
                    free(sastav);
                    return 1;
                }
                sastav = novi;

                printf("Ime: ");
                unesiTekst(sastav[brojIgraca - 1].ime, MAX_IME);
                printf("Prezime: ");
                unesiTekst(sastav[brojIgraca - 1].prezime, MAX_PREZIME);
                printf("Broj dresa: ");
                while (scanf("%d", &sastav[brojIgraca - 1].broj) != 1) {
                    printf("Unesite broj dresa (samo znamenke): ");
                    ocistiBuffer();
                }
                ocistiBuffer();
                printf("Pozicija: ");
                unesiTekst(sastav[brojIgraca - 1].pozicija, MAX_POZICIJA);
                break;

            case 2:
                ispisiSastav(sastav, brojIgraca);
                break;

            case 3:
                obrisiIgraca(&sastav, &brojIgraca);
                break;

            case 4:
                spremiUDatoteku(sastav, brojIgraca);
                printf("Kraj programa.\n");
                break;

            default:
                printf("Pogresna opcija.\n");
        }
    } while (izbor != 4);

    free(sastav);
    return 0;
}
