#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Article {
    char name[1024];
    float price;
    int quantity;
    struct Article* next;
} Article;

typedef struct Invoice {
    int year;
    int month;
    int day;
    Article* articleList;
    struct Invoice* next;
} Invoice;

typedef Invoice* InvoicePtr;
typedef Article* ArticlePtr;


int caseInsensitiveCompare(const char* str1, const char* str2) {
    while (*str1 && *str2) {
        if (tolower((unsigned char)*str1) != tolower((unsigned char)*str2)) {
            return tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
        }
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

// dodavanje artikla abecedno
void addArticleSorted(ArticlePtr head, ArticlePtr newArticle) {
    while (head->next != NULL && strcmp(head->next->name, newArticle->name) < 0) {
        head = head->next;
    }
    newArticle->next = head->next;
    head->next = newArticle;
}

// spajanje svih artikla sa isitm nazivom
int mergeArticles(ArticlePtr head, ArticlePtr newArticle) {
    while (head != NULL) {
        if (caseInsensitiveCompare(head->name, newArticle->name) == 0) {
            head->quantity += newArticle->quantity;
            free(newArticle);
            return 1;
        }
        head = head->next;
    }
    return 0;
}

// spajanje svih racuna sa istim datumom
int mergeInvoices(InvoicePtr head, InvoicePtr newInvoice) {
    InvoicePtr current = head;
    while (current != NULL) {
        if (current->day == newInvoice->day &&
            current->month == newInvoice->month &&
            current->year == newInvoice->year) {

            ArticlePtr newArticle = newInvoice->articleList->next;
            while (newArticle != NULL) {
                ArticlePtr nextArticle = newArticle->next;
                if (!mergeArticles(current->articleList->next, newArticle)) {
                    addArticleSorted(current->articleList, newArticle);
                }
                newArticle = nextArticle;
            }
            free(newInvoice->articleList);
            free(newInvoice);
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// dodavanje novog racuna tako da i dalje ostanu sortirani po datumu
void addInvoiceSorted(InvoicePtr head, InvoicePtr newInvoice) {
    InvoicePtr temp = head;
    while (temp->next != NULL &&
        (temp->next->year < newInvoice->year ||
            (temp->next->year == newInvoice->year && temp->next->month < newInvoice->month) ||
            (temp->next->year == newInvoice->year && temp->next->month == newInvoice->month && temp->next->day < newInvoice->day))) {
        temp = temp->next;
    }
    newInvoice->next = temp->next;
    temp->next = newInvoice;
}

// nalaženje prvog ra?una na po?etni traženi datum
InvoicePtr findStartDate(InvoicePtr head, int year, int month, int day) {
    while (head != NULL &&
        (head->year < year ||
            (head->year == year && head->month < month) ||
            (head->year == year && head->month == month && head->day < day))) {
        head = head->next;
    }
    return head;
}

// nalaženje prvog ra?una nakon zadnjeg traženog datuma
InvoicePtr findEndDate(InvoicePtr head, int year, int month, int day) {
    while (head != NULL &&
        (head->year < year ||
            (head->year == year && head->month < month) ||
            (head->year == year && head->month == month && head->day <= day))) {
        head = head->next;
    }
    return head;
}

// Main function
int main() {
    FILE* mainFile = fopen("Filename.txt", "r");
    if (mainFile == NULL) {
        printf("Error: Unable to open Filename.txt\n");
        return -1;
    }

    Invoice mainHead;
    mainHead.next = NULL;

    char fileName[1024] = { 0 };
    while (fscanf(mainFile, "%1023s", fileName) != EOF) {
        FILE* invoiceFile = fopen(fileName, "r");
        if (invoiceFile == NULL) {
            printf("Error: Unable to open %s\n", fileName);
            continue;
        }

        InvoicePtr newInvoice = (InvoicePtr)malloc(sizeof(Invoice));
        fscanf(invoiceFile, "%d %d %d", &newInvoice->year, &newInvoice->month, &newInvoice->day);
        newInvoice->articleList = (ArticlePtr)malloc(sizeof(Article));
        newInvoice->articleList->next = NULL;
        newInvoice->next = NULL;

        while (!feof(invoiceFile)) {
            ArticlePtr newArticle = (ArticlePtr)malloc(sizeof(Article));
            if (fscanf(invoiceFile, "%1023s %d %f", newArticle->name, &newArticle->quantity, &newArticle->price) != 3) {
                free(newArticle);
                break;
            }
            newArticle->next = NULL;
            if (!mergeArticles(newInvoice->articleList->next, newArticle)) {
                addArticleSorted(newInvoice->articleList, newArticle);
            }
        }

        fclose(invoiceFile);

        if (!mergeInvoices(mainHead.next, newInvoice)) {
            addInvoiceSorted(&mainHead, newInvoice);
        }
    }

    fclose(mainFile);

    int startYear, startMonth, startDay, endYear, endMonth, endDay;
    char articleName[1024];
    printf("Enter the start date (year month day): ");
    scanf("%d %d %d", &startYear, &startMonth, &startDay);
    printf("Enter the end date (year month day): ");
    scanf("%d %d %d", &endYear, &endMonth, &endDay);
    printf("Enter the name of the article: ");
    scanf("%1023s", articleName);

    InvoicePtr startInvoice = findStartDate(mainHead.next, startYear, startMonth, startDay);
    InvoicePtr endInvoice = findEndDate(mainHead.next, endYear, endMonth, endDay);

    float totalCost = 0;
    int totalQuantity = 0;

    while (startInvoice != endInvoice) {
        ArticlePtr currentArticle = startInvoice->articleList->next;
        while (currentArticle != NULL) {
            if (caseInsensitiveCompare(currentArticle->name, articleName) == 0) {
                totalQuantity += currentArticle->quantity;
                totalCost += currentArticle->quantity * currentArticle->price;
                break;
            }
            currentArticle = currentArticle->next;
        }
        startInvoice = startInvoice->next;
    }

    printf("Total quantity: %d\n", totalQuantity);
    printf("Total cost: %.2f\n", totalCost);

    return 0;
}
