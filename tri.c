#define _CRT_SECURE_NO_WARNINGS

#define FILE_NOT_OPENED (-1)
#define MAX_LINE (1024)
#define MAX_SIZE (128)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//3. Prethodnom zadatku dodati funkcije :
//A.dinamički dodaje novi element iza određenog elementa,
//B.dinamički dodaje novi element ispred određenog elementa,
//C.sortira listu po prezimenima osoba,
//D.upisuje listu u datoteku,
//E.čita listu iz datoteke.

struct _Person;
typedef struct _Person* Poz;
typedef struct  _Person {
    char name[MAX_SIZE], surname[MAX_SIZE];
    int god;
    Poz next;
}_person;

int Add(Poz Head, char name[MAX_SIZE], char surname[MAX_SIZE], int god);
//Dodavanje na pocetak
int AddAfter(Poz Head, char targetsurname[MAX_SIZE], char name[MAX_SIZE], char surname[MAX_SIZE], int god);
//Dodavanje nakon odredenog
int AddBefore(Poz Head, char targetsurname[MAX_SIZE], char name[MAX_SIZE], char surname[MAX_SIZE], int god);
//Dodavanje prije odredenog
int Print(Poz HeadNext);
//Printanje cijele liste
int AddEnd(Poz Head, char name[MAX_SIZE], char surname[MAX_SIZE], int god);
//Dodavanje na kraj
Poz Find(Poz Head, char surname[MAX_SIZE]);
//Trazenje po prezimenu
Poz FindPrev(Poz Head, char surname[MAX_SIZE]);
//Trazenje prethodnog po prezimenu
int DeleteByPosition(Poz PrevLoc);
//Brisanje unosom pozicije prethodnika
int DeletePerson(Poz Head, char surname[MAX_SIZE]);
//Brisanje unosom prezimena
Poz AllocateMemory();
//Alociranje memorije
int WriteInFile(Poz Headnext);
//Pise listu u dadoteku
int ReadFromFile(Poz Head);
//Ucitava listu iz dadoteke
void ClearMemory(Poz Head);

int main() {
    _person Head = { .next = NULL, .name = {0},.surname = {0}, .god = 0 };

    int toggle = 1;
    int odabir = 0;
    char name[MAX_SIZE] = { 0 }, surname[MAX_SIZE] = { 0 }, target[MAX_SIZE] = { 0 };
    int god = 0;
    while (toggle) {
        printf("Odaberite koju radnju zelite: \n 1. Dodavanje na pocetak\n 2. Dodavanje na kraj\n 3. Dodavanje nakon\n 4. Dodavanje ispred,\n 5. Brisanje\n 6. Ispis liste\n 7. Ispis liste u dadoteku\n 8. Ucitavanje liste iz dadoteke\n 9. Zavrsi \n");
        scanf("%d", &odabir);
        if (odabir >= 1 && odabir <= 9) {
            switch (odabir)
            {
            case 1:
                printf("format: IME PREZIME GODINA\n");
                scanf("%s %s %d", name, surname, &god);
                Add(&Head, name, surname, god);
                break;
            case 2:
                printf("format: IME PREZIME GODINA\n");
                scanf("%s %s %d", name, surname, &god);
                AddEnd(&Head, name, surname, god);
                break;
            case 3:
                printf("format: PREZIME nakon IME PREZIME GODINA\n");
                scanf("%s %s %s %d", target, name, surname, &god);
                AddAfter(&Head, target, name, surname, god);
                break;
            case 4:
                printf("format: PREZIME prije IME PREZIME GODINA\n");
                scanf("%s %s %s %d", target, name, surname, &god);
                AddBefore(&Head, target, name, surname, god);
                break;
            case 5:
                printf("format: PREZIME\n");
                scanf("%s", target);
                DeletePerson(&Head, target);
                break;
            case 6:
                Print(Head.next);
                break;
            case 7:
                WriteInFile(Head.next);
                break;
            case 8:
                ReadFromFile(&Head);
                break;
            case 9:
                printf("Program zavrsen");
                toggle = 0;
                break;

            default:
                break;

            }

        }
        else {
            printf("Pogresan unos pokusajte opet \n");
        }
        printf("\n------------------------------------------------------------------------\n------------------------------------------------------------------------\n");
    }
    ClearMemory(&Head);
    return 0;
}

void ClearMemory(Poz Head) {
    Poz current = Head->next;
    while (current != NULL) {
        Poz temp = current;
        current = current->next;
        free(temp);
    }
}

int WriteInFile(Poz Headnext) {
    FILE* file;
    file = fopen("ispis.txt", "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return FILE_NOT_OPENED;
    }

    while (Headnext != NULL) {
        fprintf(file, "\n---------------------------------------");
        fprintf(file, "\n ime: %s\n prezime: %s\n god: %d", Headnext->name, Headnext->surname, Headnext->god);
        Headnext = Headnext->next;
    }

    fclose(file);
    return 0;
}

int ReadFromFile(Poz Head) {
    FILE* file;
    file = fopen("ispis.txt", "r");
    if (!file) {
        printf("Error opening file for reading.\n");
        return FILE_NOT_OPENED;
    }

    char name[MAX_SIZE], surname[MAX_SIZE];
    int god;

    while (fscanf(file, "\n---------------------------------------\n ime: %s\n prezime: %s\n god: %d", name, surname, &god) == 3) {
        AddEnd(Head, name, surname, god);
    }

    fclose(file);
    return 0;
}

Poz AllocateMemory() {
    Poz ToAllocate = NULL;
    ToAllocate = (Poz)malloc(sizeof(_person));
    if (!ToAllocate) {
        printf("Neuspjesna alokacija memorije");
        return NULL;
    }
    return ToAllocate;
}

int DeletePerson(Poz Head, char surname[MAX_SIZE]) {
    Poz temp = NULL;
    temp = FindPrev(Head, surname);
    if (temp != NULL)
        DeleteByPosition(temp);
}

int DeleteByPosition(Poz Head) {
    Poz temp = NULL;
    if (Head != NULL)
    {
        temp = Head->next;
        Head->next = temp->next;
        free(temp);
    }

    return 0;
}

Poz Find(Poz Head, char surname[MAX_SIZE]) {
    while (Head != NULL && strcmp(Head->surname, surname) != 0) {
        Head = Head->next;
    }
    return Head;
}

Poz FindPrev(Poz Head, char surname[MAX_SIZE]) {
    while (Head->next != NULL && strcmp(Head->next->surname, surname) != 0) {
        Head = Head->next;
    }
    if (Head->next == NULL)
        return NULL;
    else
        return Head;
}

int AddEnd(Poz Head, char name[MAX_SIZE], char surname[MAX_SIZE], int god) {
    Poz ToBeAdded = NULL;
    ToBeAdded = AllocateMemory();
    strcpy(ToBeAdded->name, name);
    strcpy(ToBeAdded->surname, surname);
    ToBeAdded->god = god;

    while (Head->next != NULL) {
        Head = Head->next;
    }
    ToBeAdded->next = Head->next;
    Head->next = ToBeAdded;
}

int Add(Poz Head, char name[MAX_SIZE], char surname[MAX_SIZE], int god) {
    Poz ToBeAdded = NULL;
    ToBeAdded = AllocateMemory();
    strcpy(ToBeAdded->name, name);
    strcpy(ToBeAdded->surname, surname);
    ToBeAdded->god = god;

    ToBeAdded->next = Head->next;
    Head->next = ToBeAdded;
}

int AddAfter(Poz Head, char targetsurname[MAX_SIZE], char name[MAX_SIZE], char surname[MAX_SIZE], int god) {
    Head = Find(Head, targetsurname);
    Poz ToBeAdded = NULL;
    ToBeAdded = AllocateMemory();
    strcpy(ToBeAdded->name, name);
    strcpy(ToBeAdded->surname, surname);
    ToBeAdded->god = god;

    ToBeAdded->next = Head->next;
    Head->next = ToBeAdded;
}

int AddBefore(Poz Head, char targetsurname[MAX_SIZE], char name[MAX_SIZE], char surname[MAX_SIZE], int god) {
    Head = FindPrev(Head, targetsurname);
    Poz ToBeAdded = NULL;
    ToBeAdded = AllocateMemory();
    strcpy(ToBeAdded->name, name);
    strcpy(ToBeAdded->surname, surname);
    ToBeAdded->god = god;

    ToBeAdded->next = Head->next;
    Head->next = ToBeAdded;
}


int Print(Poz Headnext) {
    while (Headnext != NULL) {
        printf("\n---------------------------------------");
        printf("\n ime: %s\n prezime: %s\n god: %d", Headnext->name, Headnext->surname, Headnext->god);
        Headnext = Headnext->next;
    }
}