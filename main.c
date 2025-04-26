#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void red(){
    printf("\033[0;31m");
}

void green(){
    printf("\033[0;32m");
}

void blue(){
    printf("\033[0;34m");
}

void purple(){
    printf("\033[0;35m");
}

void white(){
    printf("\033[0;37m");
}

struct masina {
    char marca[10];
    char tip_vehicul[10];
    int an;
    int nr_locuri;
    int disponibilitate;
};

void clearall() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitforEnter() {
    printf("Apasa ENTER pentru a continua...");
    while (getchar() != '\n');
    getchar();
}

void incarca_masini(struct masina m[], int *n) {
    FILE *file = fopen("cars.txt", "r");
    if (file == NULL) {
        printf("Eroare: Nu s-a putut deschide fisierul cars.txt!\n");
        return;
    }
    char l[200];
    /* *n = 0;
    while(fgets(l, sizeof(l), file)){
        printf("%s", l);
    }*/
    *n = 0;
    while (fscanf(file, "%s %s %d %d %d", m[*n].marca, m[*n].tip_vehicul, &m[*n].an, &m[*n].nr_locuri, &m[*n].disponibilitate) != EOF) {
        printf("Marca: %s, Tip: %s, An: %d, Locuri: %d, Disponibilitate: %d\n",
                m[*n].marca, m[*n].tip_vehicul, m[*n].an, m[*n].nr_locuri,
                m[*n].disponibilitate ? 1 : 0);
        (*n)++;
    }
    fclose(file);
}

void salveaza_masini(struct masina m[], int n) {
    FILE *file = fopen("cars.txt", "w");
    for (int i = 0; i < n; i++)
        fprintf(file, "%s %s %d %d %d\n", m[i].marca, m[i].tip_vehicul, m[i].an, m[i].nr_locuri, m[i].disponibilitate);
    fclose(file);
}

void adauga_masina(struct masina m[], int *n) {
    clearall();
    printf("\n=======");
    green();
    printf(" Adaugare ");
    white();
    printf("autoturism=======\n");
    printf("Marca: "); scanf("%s", m[*n].marca);
    printf("Tip vehicul (ex. sedan, suv, coupe): "); scanf("%s", m[*n].tip_vehicul);
    printf("An fabricatie (ex. 2005, 2020): "); scanf("%d", &m[*n].an);
    printf("Numar locuri (ex. 2, 5, 7): "); scanf("%d", &m[*n].nr_locuri);
    m[*n].disponibilitate = 1;
    (*n)++;
    salveaza_masini(m, *n);
    printf("Masina a fost adaugata cu succes!\n");
    waitforEnter();
}

void sterge_masina(struct masina m[], int *n) {
    clearall();
    printf("\n=======");
    red();
    printf(" Stergere ");
    white();
    printf("autoturism =======\n");
    char marca[20];
    printf("Introduceti marca masinii pe care doriti sa o stergeti: "); scanf("%s", marca);
    int gasit = 0;
    for (int i = 0; i < (*n); i++) {
        if (strcmp(m[i].marca, marca) == 0) {
            gasit = 1;
            for (int j = i; j < (*n) - 1; j++)
                m[j] = m[j + 1];
            (*n)--;
            salveaza_masini(m, *n);
            printf("Masina a fost stearsa cu succes!\n");
            break;
        }
    }
    if (gasit == 0)
        printf("Masina nu a fost gasita!\n");
    waitforEnter();
}

void cauta_masina(struct masina m[], int n) {
    clearall();
    printf("\n=======");
    blue();
    printf(" Cautare ");
    white();
    printf("autoturism =======\n");
    char marca[20];
    printf("Introduceti marca masinii pe care doriti sa o cautati: "); scanf("%s", marca);
    FILE *file = fopen("cars.txt", "r");
    if (!file) {
        printf("Eroare la deschiderea fisierului!\n");
        waitforEnter();
        return;
    }

    char tip[10], marcaf[10];
    int an, nr_locuri, disponibilitate;
    int gasit = 0;

    while (fscanf(file, "%s %s %d %d %d", marcaf, tip, &an, &nr_locuri, &disponibilitate) != EOF) {
        if (strcmp(marcaf, marca) == 0) {
            printf("Masina gasita: %s %s %d %d locuri - %d\n", marcaf, tip, an, nr_locuri, disponibilitate ? 1 : 0);
            gasit = 1;
        }
    }

    fclose(file);

    if (!gasit) {
        printf("Masina nu a fost gasita!\n");
    }
    waitforEnter();
}

void rezerva_masina(struct masina m[], int n) {
    clearall();
    printf("\n=======");
    blue();
    printf(" Rezervare ");
    white();
    printf("autoturism =======\n");
    char marca[20];
    printf("Introduceti marca masinii pe care doriti sa o rezervati: "); scanf("%s", marca);
    int gasit = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(m[i].marca, marca) == 0 && m[i].disponibilitate == 1) {
            m[i].disponibilitate = 0;
            printf("Masina %s a fost rezervata!\n", m[i].marca);
            gasit = 1;
            FILE *file = fopen("rezervari.txt", "a");
            if (file) {
                fprintf(file, "%s %s %d %d\n", m[i].marca, m[i].tip_vehicul, m[i].an, m[i].nr_locuri);
                fclose(file);
            } else {
                printf("Eroare la deschiderea fișierului de rezervări!\n");
            }
            break;
        }
    }

    if (!gasit) {
        printf("Masina nu este disponibila sau nu exista!\n");
    } else {
        salveaza_masini(m, n);
    }

    waitforEnter();
}

void anuleaza_rezervare(struct masina m[], int n) {
    clearall();
    printf("\n=======");
    red();
    printf(" Anulare rezervare ");
    white();
    printf("autoturism =======\n");
    char marca[20];
    printf("Introduceti marca masinii pentru care doriti sa anulati rezervarea: ");
    scanf("%s", marca);
    int gasit = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(m[i].marca, marca) == 0 && m[i].disponibilitate == 0) {
            m[i].disponibilitate = 1;
            printf("Rezervarea pentru %s a fost anulata!\n", m[i].marca);
            gasit = 1;
            salveaza_masini(m, n);
            FILE *file = fopen("rezervari.txt", "r");
            FILE *temp = fopen("temp.txt", "w");

            if (file && temp) {
                char buffer[100];
                while (fgets(buffer, sizeof(buffer), file)) {
                    if (!strstr(buffer, marca)) {
                        fputs(buffer, temp);
                    }
                }
                fclose(file);
                fclose(temp);
                remove("rezervari.txt");
                rename("temp.txt", "rezervari.txt");
            } else {
                printf("Eroare la actualizarea fișierului de rezervări!\n");
            }

            break;
        }
    }

    if (!gasit) {
        printf("Masina nu a fost gasita sau nu era rezervata!\n");
    }

    waitforEnter();
}

void afiseaza_rezervari() {
    clearall();
    printf("\n======= Rezervari ");
    green();
    printf("Active ");
    white();
    printf("=======\n");
    white();
    FILE *file = fopen("rezervari.txt", "r");

    if (!file) {
        printf("Nu exista rezervari active!\n");
    } else {
        char marca[10], tip[10];
        int an, nr_locuri;
        while (fscanf(file, "%s %s %d %d", marca, tip, &an, &nr_locuri) != EOF) {
            printf("Marca: %s, Tip: %s, An: %d, Locuri: %d\n", marca, tip, an, nr_locuri);
        }
        fclose(file);
    }
    waitforEnter();
}

void afiseaza_meniu_sistem() {
    printf("\n======= Sistem de Inchiriere Auto =======\n");
    printf("1. Afisare autoturisme disponibile\n");
    printf("2. Meniu administrare autoturisme\n");
    printf("3. Rezervari autoturisme\n");
    printf("4. Iesire\n");
    printf("Alege o optiune: ");
}

void afiseaza_meniu_administrare() {
    printf("\n======= Meniu ");
    purple();
    printf("Administrare Autoturisme ");
    white();
    printf("=======\n");
    printf("1. Adaugare autoturism\n");
    printf("2. Stergere autoturism\n");
    printf("3. Cautare autoturism\n");
    printf("4. Intoarcere la meniul principal\n");
    printf("Alege o optiune:");
}

void afiseaza_meniu_rezervari() {
    printf("\n======= Meniu ");
    purple();
    printf("Rezervari");
    white();
    printf(" =======\n");
    printf("1. Rezervare autoturism\n");
    printf("2. Anulare rezervare\n");
    printf("3. Afisare autoturisme rezervate\n");
    printf("4. Intoarcere la meniul principal\n");
    printf("Alege o optiune: ");
}

int main()
{
    struct masina m[100];
    int nr_masini = 0;
    int optiune, suboptiune, suboptiune_r;
    do{
        afiseaza_meniu_sistem();
        scanf("%d", &optiune);
        clearall();
        switch(optiune){
            case 1:
                clearall();
                printf("Lista autoturismelor disponibile: \n");
                incarca_masini(m, &nr_masini);
                waitforEnter();
                clearall();
                break;
            case 2:
                do{
                    clearall();
                    afiseaza_meniu_administrare();
                    scanf("%d", &suboptiune);
                    switch(suboptiune){
                        case 1:
                            adauga_masina(m, &nr_masini);
                            break;
                        case 2:
                            sterge_masina(m, &nr_masini);
                            break;
                        case 3:
                            cauta_masina(m, nr_masini);
                            break;
                        case 4:
                            clearall();
                            break;
                        default:
                            printf("Optiune invalida! Alege din nou.\n");
                    }
                }while(suboptiune != 4);
                break;
            case 3:
                do {
                    clearall();
                    afiseaza_meniu_rezervari();
                    scanf("%d", &suboptiune_r);
                    clearall();
                    switch(suboptiune_r){
                        case 1:
                            rezerva_masina(m, nr_masini);
                            break;
                        case 2:
                            anuleaza_rezervare(m, nr_masini);
                            break;
                        case 3:
                            afiseaza_rezervari();
                            break;
                        case 4:
                            clearall();
                            break;
                        default:
                            printf("Optiune invalida! Alege din nou.\n");
                    }
                } while(suboptiune_r!=4);
                break;
            case 4:
                clearall();
                printf("Iesire din aplicatie...\n");
                break;
            default:
                clearall();
                printf("Optiune invalida! Alege din nou.\n");
        }
    } while(optiune != 4);
    return 0;
}
