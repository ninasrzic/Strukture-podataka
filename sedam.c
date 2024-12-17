#define _CRT_SECURE_NO_WARNINGS


//7. Napisati program koji pomoću vezanih listi(stabala) predstavlja strukturu direktorija.
//Omogućiti unos novih direktorija i pod - direktorija, ispis sadržaja direktorija i
//povratak u prethodni direktorij.Točnije program treba preko menija simulirati
//korištenje DOS naredbi : 1 - "md", 2 - "cd dir", 3 - "cd..", 4 - "dir" i 5 – izlaz.


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct direktorij;
typedef struct direktorij tocka;
typedef struct direktorij* Tocka;

struct SimStack;
typedef struct SimStack* Stack;

struct direktorij
{
    char ime[10];
    Tocka Next;
    Tocka Child;
};

struct SimStack
{
    Tocka Element;
    Stack Next;
};

void IspisDir(Tocka, char*);
Tocka IzadjiDir(Tocka, char*, Stack);
Tocka UdjiDir(Tocka, char*, char*, Stack);
void StvoriDir(Tocka, char*);
void Push(Stack, Tocka);
Tocka Pop(Stack);

int main()
{
    char instr[20], ime[10], naredba[10], path[100];
    char* i;
    tocka root;
    struct SimStack S;

    Tocka UpDir = &root;
    S.Next = NULL;

    printf("Unesi ime root direktorija: ");
    scanf(" %9s", root.ime);
    root.Child = NULL;
    root.Next = NULL;

    path[0] = 0;
    strcat(path, root.ime);
    strcat(path, ":\\");

    printf("\nUnesi naredbu:");
    printf("\n\tdir");
    printf("\n\tcd..");
    printf("\n\tcd ime_dir");
    printf("\n\tmk ime_dir");
    printf("\n\n\texit - za kraj\n");

    // Čišćenje input buffera nakon scanf
    getchar();

    strcpy(naredba, "");
    while (strcmp(naredba, "exit") != 0)
    {
        printf("\n\n%s>", path);

        // Resetovanje promenljivih
        memset(instr, 0, sizeof(instr));
        memset(ime, 0, sizeof(ime));
        memset(naredba, 0, sizeof(naredba));

        fgets(instr, sizeof(instr), stdin);
        instr[strcspn(instr, "\n")] = 0; // Uklanja novi red iz ulaza

        i = strchr(instr, ' ');

        if (i == NULL)
        {
            strncpy(naredba, instr, sizeof(naredba) - 1);
        }
        else
        {
            strncpy(naredba, instr, i - instr);
            naredba[i - instr] = 0;
            strcpy(ime, i + 1);
        }

        if (strcmp(naredba, "dir") == 0)
            IspisDir(UpDir, path);
        else if (strcmp(naredba, "cd..") == 0)
            UpDir = IzadjiDir(UpDir, path, &S);
        else if (strcmp(naredba, "cd") == 0)
        {
            if (ime[0] == 0)
                printf("Greska u sintaksi naredbe! Treba biti: cd ime_dir\n");
            else
                UpDir = UdjiDir(UpDir, ime, path, &S);
        }
        else if (strcmp(naredba, "mk") == 0)
        {
            if (ime[0] == 0)
                printf("Greska u sintaksi naredbe! Treba biti: mk ime_dir\n");
            else
                StvoriDir(UpDir, ime);
        }
        else if (strcmp(naredba, "exit") != 0)
            printf("Pogresan unos!\n");
    }

    return 0;
}

void IspisDir(Tocka N, char* path)
{
    int i = 0;
    printf("\nDirectory of %s", path);

    N = N->Child;
    while (N != NULL)
    {
        printf("\n\t%s", N->ime);
        N = N->Next;
        i++;
    }

    printf("\n\t%d Dir(s)", i);
}

Tocka IzadjiDir(Tocka N, char* path, Stack S)
{
    Tocka temp;
    char* a;

    temp = Pop(S);
    if (temp == NULL)
    {
        printf("Nalazimo se u root direktoriju! Iz njega se ne moze izaci!\n");
        return N;
    }
    else
    {
        a = strrchr(path, '\\');
        if (a != NULL)
        {
            *a = 0;
            a = strrchr(path, '\\');
            if (a != NULL)
                *(a + 1) = 0;
        }
        return temp;
    }
}

Tocka UdjiDir(Tocka N, char* ime, char* path, Stack S)
{
    Tocka temp = N->Child;

    while (temp != NULL && strcmp(temp->ime, ime) != 0)
        temp = temp->Next;

    if (temp == NULL)
        printf("\nNe postoji pod-direktorij s tim imenom!\n");
    else
    {
        Push(S, N);
        strcat(path, temp->ime);
        strcat(path, "\\");
        return temp;
    }
    return N;
}

void StvoriDir(Tocka N, char* ime)
{
    Tocka q;

    q = (Tocka)malloc(sizeof(tocka));
    if (q == NULL)
    {
        printf("\nGreska u alokaciji memorije!\n");
        return;
    }

    strcpy(q->ime, ime);
    q->Child = NULL;
    q->Next = NULL;

    if (N->Child == NULL)
        N->Child = q;
    else
    {
        q->Next = N->Child;
        N->Child = q;
    }
}

void Push(Stack S, Tocka T)
{
    Stack q;

    q = (Stack)malloc(sizeof(struct SimStack));
    if (q == NULL)
    {
        printf("\nGreska kod alokacije memorije!\n");
        return;
    }
    q->Element = T;
    q->Next = S->Next;
    S->Next = q;
}

Tocka Pop(Stack S)
{
    Tocka q;
    Stack temp;

    if (S->Next == NULL)
        return NULL;

    q = S->Next->Element;
    temp = S->Next;
    S->Next = temp->Next;

    free(temp);
    return q;
}
