#include "sastav.h"

// Funkcija za usporedbu - Pokazivač na funkciju za qsort i bsearch (Koncept 23, 24, 26, 50)
int usporediPoBrojuDresa(const void* a, const void* b) {
    const Igrac* igracA = (const Igrac*)a;
    const Igrac* igracB = (const Igrac*)b;
    return (igracA->brojDresa - igracB->brojDresa);
}

// --- SORTIRANJE (Koncept 23, 46) ---
void sortirajIgrace(const char* filename) {
    brojacOperacija++;
    FILE* fp = fopen(filename, "rb");
    if (!fp) return;

    // Dohvat veličine pomoću fseek, ftell i rewind (Koncept 20, 43)
    fseek(fp, 0, SEEK_END);
    long velicina = ftell(fp);
    rewind(fp);

    int brojIgraca = velicina / sizeof(Igrac);
    if (brojIgraca == 0) {
        printf("Nema igraca u datoteci za sortiranje.\n");
        fclose(fp);
        return;
    }

    // Dinamičko zauzimanje polja - Izbjegavanje VLA polja (Koncept 15, 16, 38, 39, 57)
    Igrac* poljeIgraca = (Igrac*)calloc(brojIgraca, sizeof(Igrac));
    if (!poljeIgraca) {
        ISPISI_GRESKU("Alokacija memorije za sortiranje neuspjesna");
        fclose(fp);
        return;
    }

    fread(poljeIgraca, sizeof(Igrac), brojIgraca, fp);
    fclose(fp);

    // Korištenje ugrađene qsort funkcije (Koncept 23, 46)
    qsort(poljeIgraca, brojIgraca, sizeof(Igrac), usporediPoBrojuDresa);

    fp = fopen(filename, "wb");
    if (fp) {
        fwrite(poljeIgraca, sizeof(Igrac), brojIgraca, fp);
        fclose(fp);
        printf("Sastav je uspjesno sortiran po brojevima dresova!\n");
    }

    // Sigurno brisanje memorije i anuliranje pokazivača (Koncept 18, 41)
    free(poljeIgraca);
    poljeIgraca = NULL;
}

// --- PRETRAŽIVANJE (Koncept 24, 48) ---
void pretraziIgraca(const char* filename) {
    brojacOperacija++;
    sortirajIgrace(filename); // Bsearch zahtijeva sortirano polje

    FILE* fp = fopen(filename, "rb");
    if (!fp) return;

    fseek(fp, 0, SEEK_END);
    int brojIgraca = ftell(fp) / sizeof(Igrac);
    rewind(fp);

    if (brojIgraca == 0) {
        printf("Nema igraca u bazi.\n");
        fclose(fp);
        return;
    }

    Igrac* poljeIgraca = (Igrac*)malloc(brojIgraca * sizeof(Igrac));
    if (!poljeIgraca) {
        fclose(fp);
        return;
    }
    fread(poljeIgraca, sizeof(Igrac), brojIgraca, fp);
    fclose(fp);

    int trazeniBroj;
    printf("Unesite broj dresa igraca kojeg pretrazujete: ");
    scanf("%d", &trazeniBroj);
    ocistiUnos();

    Igrac kljuc;
    kljuc.brojDresa = trazeniBroj;

    // Korištenje ugrađene bsearch funkcije s pokazivačem na funkciju (Koncept 24, 26, 48, 50)
    Igrac* pronadjen = (Igrac*)bsearch(&kljuc, poljeIgraca, brojIgraca, sizeof(Igrac), usporediPoBrojuDresa);

    if (pronadjen != NULL) {
        printf("\n[IGRAC PRONADJEN] ID: %d | Ime: %s | Dres: %d | Pozicija: %s\n", 
               pronadjen->id, pronadjen->imePrezime, pronadjen->brojDresa,
               (pronadjen->pozicija == VRATAR) ? "Vratar" : (pronadjen->pozicija == OBRANA) ? "Obrana" : (pronadjen->pozicija == VEZNI) ? "Vezni" : "Napad");
    } else {
        printf("Igrac s brojem dresa %d nije u sastavu za utakmicu.\n", trazeniBroj);
    }

    free(poljeIgraca);
    poljeIgraca = NULL;
}

// --- REKURZIVNI ALGORITAM (Koncept 25, 49) ---
// Rekurzivna funkcija koja provjerava i broji koliko igrača u polju ima ulogu kapetana
static int rekurzijaKapetani(const Igrac* polje, int indeks, int ukupno) {
    if (indeks >= ukupno) { // Bazni slučaj
        return 0;
    }
    int trenutniKapetan = polje[indeks].jeKapetan ? 1 : 0;
    return trenutniKapetan + rekurzijaKapetani(polje, indeks + 1, ukupno);
}

void pokreniRekurzivnoBrojanje(const char* filename) {
    brojacOperacija++;
    FILE* fp = fopen(filename, "rb");
    if (!fp) return;

    fseek(fp, 0, SEEK_END);
    int brojIgraca = ftell(fp) / sizeof(Igrac);
    rewind(fp);

    if (brojIgraca == 0) {
        printf("Nema igraca u sastavu.\n");
        fclose(fp);
        return;
    }

    Igrac* polje = (Igrac*)malloc(brojIgraca * sizeof(Igrac));
    if (!polje) {
        fclose(fp);
        return;
    }
    fread(polje, sizeof(Igrac), brojIgraca, fp);
    fclose(fp);

    // Poziv rekurzije (Koncept 25, 49)
    int ukupnoKapetana = rekurzijaKapetani(polje, 0, brojIgraca);
    printf("Ukupan broj igraca u zapisniku: %d\n", brojIgraca);
    printf("Broj kapetana u sastavu (izracunato rekurzivno): %d\n", ukupnoKapetana);

    free(polje);
    polje = NULL;
}