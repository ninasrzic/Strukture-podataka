#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE (1024)
#define MAX_SIZE (50)
#define Failed_To_Allocate (NULL)
#define ErrorInCreation (-1)
#define Sucess (1)
#define Fail_To_Open_File (NULL)
#define Error_Calclating


//5. Napisati program koji iz datoteke čita postfiks izraz i zatim korištenjem stoga računa
//rezultat.Stog je potrebno realizirati preko vezane liste.

struct _Stack;
typedef struct _Stack* Position;
typedef struct _Stack {
	int Element;
	Position next;
}Stack;

int Push(Position Head, int Number);
int StackPrint(Position FirstElement);
int Pop(Position Head);
int Top(Position FirstElement);
char* ReadFromFile(char* filename);
int CalculatePostfix(Position Head, char* Postfix);
int CountStack(Position FirstElement);
int ClearStack(Position Head);

Position AllocateMemory();


int main() {

	Stack Head = { .Element = 0, .next = NULL };
	char Postfix[MAX_LINE] = { 0 };
	strcpy(Postfix, ReadFromFile("Postfix.txt"));
	if (!Postfix)
		return Fail_To_Open_File;
	int x = CalculatePostfix(&Head, Postfix);
	printf("%d", x);
	return 0;
}

int ClearStack(Position Head) {
	while (CountStack(Head->next) > 0)
		Pop(Head);
	return 0;
}

int CountStack(Position FirstElement) {
	int counter = 0;
	while (FirstElement != NULL) {
		FirstElement = FirstElement->next;
		counter++;
	}
	return counter;
}

int CalculatePostfix(Position Head, char* Postfix) {
	int counter = 0;
	char currentChar = Postfix[counter];
	while (currentChar != '\0') {
		currentChar = Postfix[counter];
		if (isdigit(currentChar) != 0)
			Push(Head, currentChar - '0');
		else {
			int first = 0;
			int second = 0;
			switch (currentChar)
			{
			case '+':
				first = Pop(Head);
				second = Pop(Head);
				if (first && second) {
					Push(Head, first + second);
				}
				else {
					printf("Wrong postfix");
					return 0;
				}
				break;

			case '-':
				first = Pop(Head);
				second = Pop(Head);
				if (first && second) {
					Push(Head, second - first);
				}
				else {
					printf("Wrong postfix");
					return 0;
				}
				break;

			case '*':
				first = Pop(Head);
				second = Pop(Head);
				if (first && second) {
					Push(Head, first * second);
				}
				else {
					printf("Wrong postfix");
					return 0;
				}
				break;

			case '/':
				first = Pop(Head);
				second = Pop(Head);
				if (first && second) {
					Push(Head, first / second);
				}
				else {
					printf("Wrong postfix");
					return 0;
				}
				break;
			}

		}
		counter++;
	}
	if (CountStack(Head->next) != 1) {
		return Error_Calclating;
	}
	return Top(Head->next);
}


char* ReadFromFile(char* filename) {
	FILE* FilePointer = NULL;
	FilePointer = fopen(filename, "r");
	if (!FilePointer)
		return Fail_To_Open_File;
	char Buffer[MAX_LINE] = { 0 };
	fgets(Buffer, MAX_LINE, FilePointer);
	return Buffer;
}


int Top(Position FirstElement) {
	if (FirstElement == NULL) {
		printf("Empty stack");
		return 0;
	}
	while (FirstElement->next != NULL)
		FirstElement = FirstElement->next;
	return FirstElement->Element;
}

Position AllocateMemory() {
	Position ToAllocate = NULL;
	ToAllocate = (Position)malloc(sizeof(Stack));
	if (ToAllocate)
		return ToAllocate;
	else
		return Failed_To_Allocate;
}


int Pop(Position Head) {
	if (Head->next == NULL)
		return NULL;
	while (Head->next->next != NULL)
		Head = Head->next;
	int ToReturn = Head->next->Element;
	free(Head->next);
	Head->next = NULL;
	return ToReturn;
}


int Push(Position Head, int Number) {
	while (Head->next != NULL)
		Head = Head->next;
	Position StackToAdd = NULL;
	StackToAdd = AllocateMemory();
	if (!StackToAdd)
		return ErrorInCreation;
	StackToAdd->Element = Number;
	StackToAdd->next = NULL;
	Head->next = StackToAdd;
	return Sucess;
}

int StackPrint(Position FirstElement) {
	while (FirstElement != NULL) {
		printf("\n%d", FirstElement->Element);
		FirstElement = FirstElement->next;
	}
	return Sucess;
}
