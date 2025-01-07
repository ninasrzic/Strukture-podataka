#define _CRT_SECURE_NO_WARNINGS
#define ALLOCATION_ERROR (1)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct Node Node;
typedef Node* Position;
struct Node {
    int element;
    Position left;
    Position right;
};

Position addNode(Position root, int newElement);
Position minNode(Position root);
int printInOrder(Position root);
int printPreOrder(Position root);
int printPostOrder(Position root);
int printLevelOrder(Position root);
Position deleteElement(Position root, int element);
bool findElement(Position root, int element);

int main()
{
    Position root = NULL;
    root = calloc(1, sizeof(Node));

    if (root == NULL) {
        printf("Error allocating memory\n");
        return ALLOCATION_ERROR;
    }

    root->left = NULL;
    root->right = NULL;
    printf("Enter the element for the root node:");
    scanf("%d", &root->element);

    while (1) {
        printf("\nMenu:\n");
        printf("1 - Insert element into the tree\n");
        printf("2 - Print elements of the tree\n");
        printf("3 - Delete element\n");
        printf("4 - Find element\n");
        printf("5 - Exit\n");

        int choice = 0;
        printf("Choose:");
        scanf("%d", &choice);

        if (choice == 1) {
            int newElement = 0;
            printf("Enter element:");
            scanf("%d", &newElement);
            addNode(root, newElement);
        }
        else if (choice == 2) {
            printf("\nPrint menu:\n");
            printf("1 - In order\n");
            printf("2 - Pre order\n");
            printf("3 - Post order\n");
            printf("4 - Level order\n");

            int printChoice = 0;
            printf("Choose the print type:");
            scanf("%d", &printChoice);

            switch (printChoice) {
            case 1:
                printInOrder(root);
                break;
            case 2:
                printPreOrder(root);
                break;
            case 3:
                printPostOrder(root);
                break;
            case 4:
                printLevelOrder(root);
                break;
            default:
                printf("Invalid choice\n");
            }
        }
        else if (choice == 3) {
            int element = 0;
            printf("Enter the element to delete:");
            scanf("%d", &element);
            deleteElement(root, element);
        }
        else if (choice == 4) {
            int element = 0;
            printf("Enter the element to find:");
            scanf("%d", &element);
            if (findElement(root, element)) {
                printf("Element pronaden!\n");
            }
            else {
                printf("Element nije pronaden!\n");
            }
            //printf("%d",root);
        }
        else if (choice == 5) {
            printf("\nExiting the program\n");
            break;
        }
    }
    free(root);
    return 0;
}

Position addNode(Position root, int newElement) {
    if (root == NULL) {
        root = (Position)calloc(1, sizeof(Node));
        if (root == NULL) {
            return ALLOCATION_ERROR;
        }
        root->element = newElement;
        root->left = NULL;
        root->right = NULL;
    }
    else if (newElement > root->element) {
        root->right = addNode(root->right, newElement);
    }
    else if (newElement < root->element) {
        root->left = addNode(root->left, newElement);
    }

    return root;
}

int printInOrder(Position root) {
    if (root == NULL) {
        return 0;
    }
    printInOrder(root->left);
    printf("%d, ", root->element);
    printInOrder(root->right);
    return 0;
}

int printPreOrder(Position root) {
    if (root == NULL) {
        return 0;
    }
    printf("%d, ", root->element);
    printPreOrder(root->left);
    printPreOrder(root->right);
    return 0;
}

int printPostOrder(Position root) {
    if (root == NULL) {
        return 0;
    }
    printPostOrder(root->left);
    printPostOrder(root->right);
    printf("%d, ", root->element);
    return 0;
}

int printLevelOrder(Position root) {
    if (root == NULL) {
        return 1;
    }

    Position queue[100];
    int front = 0, rear = 0;
    queue[rear++] = root;

    while (front < rear) {
        Position current = queue[front++];
        printf("%d ", current->element);

        if (current->left != NULL)
            queue[rear++] = current->left;

        if (current->right != NULL)
            queue[rear++] = current->right;
    }
    return 0;
}

Position deleteElement(Position root, int element) {
    if (root == NULL) {
        printf("Element not found");
        return NULL;
    }

    if (element < root->element) {
        root->left = deleteElement(root->left, element);
    }
    else if (element > root->element) {
        root->right = deleteElement(root->right, element);
    }
    else if (root->left != NULL && root->right != NULL) {
        Position temp = minNode(root->right);
        root->element = temp->element;
        root->right = deleteElement(root->right, root->element);
    }
    else {
        Position toDelete = root;
        if (root->left == NULL) {
            root = root->right;
        }
        else {
            root = root->left;
        }
        free(toDelete);
    }

    return root;
}

Position minNode(Position root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

bool findElement(Position root, int element) {
    while (1) {
        if (root == NULL) {
            return NULL;
        }
        else if (element < root->element) {
            root = root->left;
        }
        else if (element > root->element) {
            root = root->right;
        }
        else {
            return true;
        }
    }
    return false;
}
