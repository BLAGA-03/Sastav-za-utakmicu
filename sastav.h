#ifndef SASTAV_H
#define SASTAV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Makro definicije i funkcije (Koncept 9, 11)
#define MAX_IME 50
#define DATOTEKA_SASTAV "sastav.bin"
#define ISPISI_GRESKU(poruka) perror("Sustavna pogreška: " poruka)

// Enum za pozicije igrača (Koncept 4, 11, 27)
typedef enum {
    VRATAR = 1,
    OBRANA,
    VEZNI,
    NAPAD
} Pozicija;

// Enum za status u momčadi (Koncept 4, 27)
typedef enum {
    POCETNA_POSTAVA = 1,
    KLUPA
} StatusMomcadi;

// Enum za glavne opcije izbornika (Koncept 11, 27)
typedef enum {
    IZLAZ = 0,
    DODAJ_IGRACA,
    PRIKAZI_SASTAV,
    AZURIRAJ_IGRACA,
    OBRISI_IGRACA,
    SORTIRAJ_PO_BROJU,
    PRETRAZI_PO_BROJU,
    BROJ_IGRACA_REKURZIJA
} OpcijeIzbornika;

// Glavna struktura za igrača - Složeni tip podataka (Koncept 3, 4, 26, 36)
typedef struct {
    int id;
    char imePrezime[MAX_IME];
    int brojDresa;
    Pozicija pozicija;
    StatusMomcadi status;
    int jeKapetan; // 1 = Da, 0 = Ne
} Igrac;

// Deklaracija eksterne globalne varijable (Koncept 8, 31)
extern int brojacOperacija;

// Deklaracije glavnih CRUDI funkcija (Koncept 1, 7, 13, 36)
void prikaziIzbornik(void);
void dodajIgraca(const char* filename);
void prikaziSveIgrace(const char* filename);
void azurirajIgraca(const char* filename);
void obrisiIgraca(const char* filename);

// Deklaracije naprednih funkcija (Koncept 23, 24, 25, 26)
void sortirajIgrace(const char* filename);
void pretraziIgraca(const char* filename);
void pokreniRekurzivnoBrojanje(const char* filename);

// Pomoćne inline funkcije (Koncept 9)
static inline void ocistiUnos(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

#endif // SASTAV_H