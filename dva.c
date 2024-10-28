#define _CRT_SECURE_NO_WARNINGS
//#define FILE_NOT_OPENED (-1)
#define MAX_LINE (1024)
#define MAX_SIZE (128)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// 2. Definirati strukturu osoba (ime, prezime, godina rođenja) i napisati program koji:
// A. dinamički dodaje novi element na početak liste,
// B. ispisuje listu,
// C. dinamički dodaje novi element na kraj liste,
// D. pronalazi element u listi (po prezimenu),
// E. briše određeni element iz liste,
// U zadatku se ne smiju koristiti globalne varijable.

struct _Person;
typedef struct _Person* Poz;
typedef struct  _Person {
    char name[MAX_SIZE], surname[MAX_SIZE];
    int god;
    Poz next;
}_person;


int Add(Poz Head, char name[MAX_SIZE], char surname[MAX_SIZE], int god);
//Dodavanje na pocetak
int Print(Poz Headnext);
//Printanje cijele liste
int AddEnd(Poz Head, char name[MAX_SIZE], char surname[MAX_SIZE], int god);
//Dodavanje na kraj
Poz Find(Poz Head, char surname[MAX_SIZE]);
//Trazenje po prezimenu - nepotrebno u ovom zadatku ali ostavljeno radi vjezbe
Poz FindPrev(Poz Head, char surname[MAX_SIZE]);
//Trazenje prethodnog po prezimenu
int DeleteByPosition(Poz PrevLoc);
//Brisanje unosom pozicije prethodnika - moguca integracija sa fjom DeletePerson ali napravljeno ovako zbog modularnosti
int DeletePerson(Poz Head, char surname[MAX_SIZE]);
//Brisanje unosom prezimena
Poz AllocateMemory();
//Alociranje memorije
void ClearMemory(Poz Head);

int main() {
    _person Head = { .next = NULL, .name = {0},.surname = {0}, .god = 0 };
    int a = 1;
    int odabir = 0;
    char name[MAX_SIZE] = { 0 }, surname[MAX_SIZE] = { 0 }, target[MAX_SIZE] = { 0 };
    int god = 0;
    while (a) {
        printf("Odaberite koju radnju zelite: \n 1. Dodavanje na pocetak\n 2. Dodavanje na kraj\n 3. Brisanje\n 4. Ispis liste\n 5. Zavrsi \n");
        scanf("%d", &odabir);
        if (odabir >= 1 && odabir <= 5) {
            switch (odabir)
            {
            case 1:
                printf("format: IME PREZIME GODINA\n");
                if (scanf("%s %s %d", name, surname, &god) == 3) {
                    Add(&Head, name, surname, god);
                }
                else {
                    printf("Invalid format. Please enter the correct format.\n");
                }
                break;
            case 2:
                printf("format: IME PREZIME GODINA\n");
                if (scanf("%s %s %d", name, surname, &god) == 3) {
                    AddEnd(&Head, name, surname, god);
                }
                else {
                    printf("Invalid format. Please enter the correct format.\n");
                }
                break;
            case 3:
                printf("format: PREZIME\n");
                scanf("%s", target);
                DeletePerson(&Head, target);
                break;
            case 4:
                Print(Head.next);
                break;
            case 5:
                printf("Program zavrsen");
                a = 0;
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
    Poz temp;
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
    ToBeAdded=AllocateMemory();
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