#include <stdio.h>
#include <stdlib.h>

#define MAX_BODOVA 100

typedef struct {
    char ime[50];
    char prezime[50];
    int bodovi;
} Student;

int main() {
    char imeDatoteke[] = "studenti.txt";
    FILE *fp = fopen(imeDatoteke, "r");
    if (!fp) {
        printf("Greška pri otvaranju datoteke!\n");
        return -1;
    }

    int brojStudenata = 0;
    char tempIme[50], tempPrezime[50];
    int tempBodovi;

    while (fscanf(fp, "%s %s %d", tempIme, tempPrezime, &tempBodovi) == 3) {
        brojStudenata++;
    }

    rewind(fp);

    Student *studenti = (Student *)malloc(brojStudenata * sizeof(Student));
    if (!studenti) {
        printf("Greška pri alociranju memorije!\n");
        fclose(fp);
        return -1;
    }
    for (int i = 0; i < brojStudenata; i++) {
        fscanf(fp, "%s %s %d", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi);
    }
    fclose(fp);

    for (int i = 0; i < brojStudenata; i++) {
        double relativniBodovi = (double)studenti[i].bodovi / MAX_BODOVA * 100;
        printf("Ime: %s, Prezime: %s, Bodovi: %d, Relativni bodovi: %.2f%%\n",
               studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, relativniBodovi);
    }

    free(studenti);

    return 0;
}
