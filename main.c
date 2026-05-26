#include "sastav.h"

// Definiranje eksterne varijable koja se prati kroz cijeli projekt (Koncept 8, 31)
int brojacOperacija = 0;

int main(void) {
    // Odabir konkretnog primitivnog tipa za kontrolu izbornika (Koncept 2, 25)
    int odabirKorisnika = -1; 
    
    // Osiguravanje postojanja datoteke pri pokretanju
    FILE* inicijalizacijaFp = fopen(DATOTEKA_SASTAV, "ab");
    if (inicijalizacijaFp != NULL) {
        fclose(inicijalizacijaFp);
    }

    // Izbornik i petlja izvršavanja (Koncept 10, 33)
    while (odabirKorisnika != IZLAZ) {
        prikaziIzbornik();
        
        if (scanf("%d", &odabirKorisnika) != 1) {
            printf("Pogresan unos. Molimo unesite brojcanu vrijednost.\n");
            ocistiUnos();
            continue;
        }
        ocistiUnos(); 

        switch ((OpcijeIzbornika)odabirKorisnika) {
            case DODAJ_IGRACA:
                dodajIgraca(DATOTEKA_SASTAV);
                break;
            case PRIKAZI_SASTAV:
                prikaziSveIgrace(DATOTEKA_SASTAV);
                break;
            case AZURIRAJ_IGRACA:
                azurirajIgraca(DATOTEKA_SASTAV);
                break;
            case OBRISI_IGRACA:
                obrisiIgraca(DATOTEKA_SASTAV);
                break;
            case SORTIRAJ_PO_BROJU:
                sortirajIgrace(DATOTEKA_SASTAV);
                break;
            case PRETRAZI_PO_BROJU:
                pretraziIgraca(DATOTEKA_SASTAV);
                break;
            case BROJ_IGRACA_REKURZIJA:
                pokreniRekurzivnoBrojanje(DATOTEKA_SASTAV);
                break;
            case IZLAZ:
                printf("Izlazak iz sustava za sastav utakmica...\n");
                break;
            default:
                printf("Opcija nije prepoznata, pokusajte ponovno.\n");
                break;
        }
    }

    // Ispis završne statistike korištenjem eksterne varijable
    printf("Ukupno izvrsenih operacija nad datotekom tijekom sesije: %d\n", brojacOperacija);

    return 0;
}