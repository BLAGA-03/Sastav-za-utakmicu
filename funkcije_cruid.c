#include "sastav.h"

// Statička globalna varijabla - vidljiva samo unutar ove datoteke (Koncept 6, 29)
static int trenutniId = 1;

// Pomoćna funkcija za ažuriranje ID-a pri pokretanju
static void AzurirajTrenutniId(const char* filename) {
    FILE* fp = fopen(filename, "rb"); // Otvaranje datoteke (Koncept 19, 42)
    if (!fp) return;
    
    Igrac privremeni;
    while (fread(&privremeni, sizeof(Igrac), 1, fp) == 1) {
        if (privremeni.id >= trenutniId) {
            trenutniId = privremeni.id + 1;
        }
    }
    fclose(fp);
}

// Prikaz glavnog izbornika (Koncept 10, 33)
void prikaziIzbornik(void) {
    printf("\n==== NOGOMETNI SASTAV MANAGER ====\n");
    printf("%d. Unos novog igraca (Create)\n", DODAJ_IGRACA);
    printf("%d. Prikaz trenutnog sastava (Read)\n", PRIKAZI_SASTAV);
    printf("%d. Azuriranje podataka igraca (Update)\n", AZURIRAJ_IGRACA);
    printf("%d. Uklanjanje igraca iz sastava (Delete)\n", OBRISI_IGRACA);
    printf("%d. Sortiranje igraca po broju dresa (qsort)\n", SORTIRAJ_PO_BROJU);
    printf("%d. Pretrazivanje igraca po dresu (bsearch)\n", PRETRAZI_PO_BROJU);
    printf("%d. Rekurzivno prebrojavanje kapetana (Rekurzija)\n", BROJ_IGRACA_REKURZIJA);
    printf("%d. Izlaz iz programa\n", IZLAZ);
    printf("==================================\n");
    printf("Vas odabir: ");
}

// --- CREATE & INSERT (Koncept 1, 2) ---
void dodajIgraca(const char* filename) {
    brojacOperacija++;
    AzurirajTrenutniId(filename);

    FILE* fp = fopen(filename, "ab");
    if (fp == NULL) {
        ISPISI_GRESKU("Otvaranje datoteke nije uspjelo"); // Upravljanje pogreškama (Koncept 22, 45)
        return;
    }

    // Dinamičko zauzimanje memorije za složeni tip (Koncept 16, 17, 39, 40)
    Igrac* noviIgrac = (Igrac*)malloc(sizeof(Igrac));
    if (noviIgrac == NULL) {
        ISPISI_GRESKU("Alokacija memorije za igraca neuspjesna");
        fclose(fp);
        return;
    }

    noviIgrac->id = trenutniId++;

    printf("Unesite ime i prezime igraca: ");
    fgets(noviIgrac->imePrezime, MAX_IME, stdin);
    noviIgrac->imePrezime[strcspn(noviIgrac->imePrezime, "\n")] = 0;

    // Odabir primitivnih tipova podataka za cjelobrojne vrijednosti (Koncept 2, 25)
    printf("Unesite broj dresa: ");
    scanf("%d", &noviIgrac->brojDresa);

    int odabirPozicije;
    printf("Odaberite poziciju (1-Vratar, 2-Obrana, 3-Vezni, 4-Napad): ");
    scanf("%d", &odabirPozicije);
    noviIgrac->pozicija = (Pozicija)odabirPozicije;

    int odabirStatusa;
    printf("Odaberite status u momcadi (1-Pocetna postava, 2-Klupa): ");
    scanf("%d", &odabirStatusa);
    noviIgrac->status = (StatusMomcadi)odabirStatusa;

    printf("Je li igrac kapetan momcadi? (1-Da, 0-Ne): ");
    scanf("%d", &noviIgrac->jeKapetan);
    ocistiUnos();

    // Zapis u binarnu datoteku
    fwrite(noviIgrac, sizeof(Igrac), 1, fp);
    
    // Sigurno zatvaranje i anuliranje (Koncept 18, 19, 41, 42)
    fclose(fp);
    free(noviIgrac);
    noviIgrac = NULL; 

    printf("Igrac uspjesno upisan u sastav utakmice!\n");
}

// --- READ (Koncept 1) ---
void prikaziSveIgrace(const char* filename) {
    brojacOperacija++;
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        printf("Sastav je prazan (datoteka ne postoji).\n");
        return;
    }

    Igrac igrac;
    printf("\n%-5s %-25s %-7s %-12s %-15s %-8s\n", "ID", "Ime i Prezime", "Dres", "Pozicija", "Status", "Kapetan");
    printf("----------------------------------------------------------------------------\n");

    // Korištenje feof i ferror za sigurnost datoteka (Koncept 22, 45)
    while (fread(&igrac, sizeof(Igrac), 1, fp) == 1) {
        printf("%-5d %-25s %-7d %-12s %-15s %-8s\n",
            igrac.id,
            igrac.imePrezime,
            igrac.brojDresa,
            (igrac.pozicija == VRATAR) ? "Vratar" : (igrac.pozicija == OBRANA) ? "Obrana" : (igrac.pozicija == VEZNI) ? "Vezni" : "Napad",
            (igrac.status == POCETNA_POSTAVA) ? "Pocetna postava" : "Klupa",
            (igrac.jeKapetan) ? "DA" : "NE");
    }

    if (ferror(fp)) {
        ISPISI_GRESKU("Greska prilikom citanja baze sastava");
    }

    fclose(fp);
}

// --- UPDATE (Koncept 1) ---
void azurirajIgraca(const char* filename) {
    brojacOperacija++;
    FILE* fp = fopen(filename, "r+b"); 
    if (!fp) {
        ISPISI_GRESKU("Nemoguce otvoriti datoteku za izmjenu");
        return;
    }

    int trazeniId;
    printf("Unesite ID igraca kojeg zelite azurirati: ");
    scanf("%d", &trazeniId);
    ocistiUnos();

    Igrac igrac;
    int pronadjen = 0;

    while (fread(&igrac, sizeof(Igrac), 1, fp)) {
        if (igrac.id == trazeniId) {
            pronadjen = 1;
            
            // Korištenje ftell i fseek (Koncept 20, 43)
            long pozicijaZaZapis = ftell(fp) - sizeof(Igrac);

            printf("Mijenjate podatke za: %s\n", igrac.imePrezime);
            printf("Novo ime i prezime: ");
            fgets(igrac.imePrezime, MAX_IME, stdin);
            igrac.imePrezime[strcspn(igrac.imePrezime, "\n")] = 0;

            printf("Novi broj dresa: ");
            scanf("%d", &igrac.brojDresa);
            
            int odabirPozicije, odabirStatusa;
            printf("Nova pozicija (1-Vratar, 2-Obrana, 3-Vezni, 4-Napad): ");
            scanf("%d", &odabirPozicije);
            igrac.pozicija = (Pozicija)odabirPozicije;

            printf("Novi status (1-Pocetna postava, 2-Klupa): ");
            scanf("%d", &odabirStatusa);
            igrac.status = (StatusMomcadi)odabirStatusa;

            printf("Je li kapetan? (1-Da, 0-Ne): ");
            scanf("%d", &igrac.jeKapetan);
            ocistiUnos();

            fseek(fp, pozicijaZaZapis, SEEK_SET);
            fwrite(&igrac, sizeof(Igrac), 1, fp);
            break;
        }
    }

    if (!pronadjen) printf("Igrac s ID-em %d nije u sastavu.\n", trazeniId);
    else printf("Podaci o igracu uspjesno azurirani!\n");

    fclose(fp);
}

// --- DELETE (Koncept 1, 21 s remove/rename) ---
void obrisiIgraca(const char* filename) {
    brojacOperacija++;
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        printf("Datoteka sastava ne postoji.\n");
        return;
    }

    const char* privremenaDatoteka = "temp_sastav.bin";
    FILE* fpPrivremena = fopen(privremenaDatoteka, "wb");
    if (!fpPrivremena) {
        ISPISI_GRESKU("Kreiranje privremene datoteke nije uspjelo");
        fclose(fp);
        return;
    }

    int trazeniId;
    printf("Unesite ID igraca kojeg brisete iz sastava: ");
    scanf("%d", &trazeniId);
    ocistiUnos();

    Igrac igrac;
    int obrisano = 0;

    while (fread(&igrac, sizeof(Igrac), 1, fp) == 1) {
        if (igrac.id == trazeniId) {
            obrisano = 1; 
            continue; // Preskače upisivanje izbrisanog igrača
        }
        fwrite(&igrac, sizeof(Igrac), 1, fpPrivremena);
    }

    fclose(fp);
    fclose(fpPrivremena);

    if (obrisano) {
        // Upotreba funkcija remove() i rename() (Koncept 21, 44)
        remove(filename);
        rename(privremenaDatoteka, filename);
        printf("Igrac uspjesno uklonjen s popisa sastava.\n");
    } else {
        remove(privremenaDatoteka);
        printf("Igrac s ID-em %d nije pronadjen.\n", trazeniId);
    }
}