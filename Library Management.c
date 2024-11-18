#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct book {
    int id;
    char title[50];
    char author[50];
    char publisher[50];
    char issueDate[12];
    int isIssued;
    struct book *next;
} *head = NULL, *tail = NULL;

struct member {
    int id;
    char name[50];
    char address[50];
    char joinDate[12];
} member;

FILE *filePtr;

void addBook();
void listBooks();
void issueBook();
void returnBook();
void addMember();
void listMembers();
void removeMember();
void searchBookById(); // Declaration of searchBookById

int main() {
    int choice;

    while (1) {
        printf("\n<== Library Management System ==>\n");
        printf("1. Add Book\n");
        printf("2. View Book List\n");
        printf("3. Issue Book\n");
        printf("4. Return Book\n");
        printf("5. Add Member\n");
        printf("6. View Members List\n");
        printf("7. Remove Member\n");
        printf("8. Search Book by ID\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 0:
                exit(0);
            case 1:
                addBook();
                break;
            case 2:
                listBooks();
                break;
            case 3:
                issueBook();
                break;
            case 4:
                returnBook();
                break;
            case 5:
                addMember();
                break;
            case 6:
                listMembers();
                break;
            case 7:
                removeMember();
                break;
            case 8:
                searchBookById();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
        printf("\nPress any key to continue...");
        getchar();
    }
    return 0;
}

void getCurrentDate(char *dateStr) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(dateStr, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

void addBook() {
    struct book *newBook = (struct book *)malloc(sizeof(struct book));
    if (newBook == NULL) {
        printf("Unable to allocate memory.\n");
        return;
    }

    newBook->next = NULL;
    newBook->isIssued = 0;

    printf("Enter Book ID: ");
    scanf("%d", &newBook->id);
    getchar();

    printf("Enter Book Title: ");
    fgets(newBook->title, sizeof(newBook->title), stdin);
    strtok(newBook->title, "\n");

    printf("Enter Author: ");
    fgets(newBook->author, sizeof(newBook->author), stdin);
    strtok(newBook->author, "\n");

    printf("Enter Publisher: ");
    fgets(newBook->publisher, sizeof(newBook->publisher), stdin);
    strtok(newBook->publisher, "\n");

    if (tail == NULL) {
        head = newBook;
        tail = newBook;
    } else {
        tail->next = newBook;
        tail = newBook;
    }
    printf("\nBook added successfully.\n");
}

void listBooks() {
    struct book *temp = head;
    printf("\n<== Book List ==>\n");
    printf("%-10s %-30s %-30s %-30s %-12s %-10s\n", "ID", "Title", "Author", "Publisher", "Issue Date", "Issued");
    printf("-------------------------------------------------------------------------------------------\n");

    while (temp != NULL) {
        printf("%-10d %-30s %-30s %-30s %-12s %-10s\n",
               temp->id, temp->title, temp->author, temp->publisher, temp->isIssued ? temp->issueDate : "N/A", temp->isIssued ? "Yes" : "No");
        temp = temp->next;
    }
}

void issueBook() {
    int id;
    struct book *temp = head;

    printf("Enter Book ID to issue: ");
    scanf("%d", &id);
    getchar();

    while (temp != NULL) {
        if (temp->id == id) {
            if (temp->isIssued) {
                printf("\nBook is already issued.\n");
                return;
            }
            temp->isIssued = 1;
            getCurrentDate(temp->issueDate);
            printf("\nBook issued successfully.\n");
            return;
        }
        temp = temp->next;
    }
    printf("\nBook not found.\n");
}

void returnBook() {
    int id;
    struct book *temp = head;

    printf("Enter Book ID to return: ");
    scanf("%d", &id);
    getchar();

    while (temp != NULL) {
        if (temp->id == id) {
            if (!temp->isIssued) {
                printf("\nBook is not issued.\n");
                return;
            }
            temp->isIssued = 0;
            printf("\nBook returned successfully.\n");
            return;
        }
        temp = temp->next;
    }
    printf("\nBook not found.\n");
}

void addMember() {
    getCurrentDate(member.joinDate);

    printf("Enter Member ID: ");
    scanf("%d", &member.id);
    getchar();

    printf("Enter Member Name: ");
    fgets(member.name, sizeof(member.name), stdin);
    strtok(member.name, "\n");

    printf("Enter Address: ");
    fgets(member.address, sizeof(member.address), stdin);
    strtok(member.address, "\n");

    filePtr = fopen("member.txt", "ab");
    if (filePtr == NULL) {
        printf("Error opening file.\n");
        return;
    }
    fwrite(&member, sizeof(member), 1, filePtr);
    fclose(filePtr);
    printf("\nMember added successfully.\n");
}

void listMembers() {
    printf("\n<== Member List ==>\n");
    printf("%-10s %-30s %-30s %s\n", "ID", "Name", "Address", "Join Date");
    printf("---------------------------------------------------------------\n");

    filePtr = fopen("member.txt", "rb");
    if (filePtr == NULL) {
        printf("No member records found.\n");
        return;
    }
    while (fread(&member, sizeof(member), 1, filePtr) == 1) {
        printf("%-10d %-30s %-30s %s\n", member.id, member.name, member.address, member.joinDate);
    }
    fclose(filePtr);
}

void removeMember() {
    int id;
    int found = 0;
    FILE *tempFile;
    struct member tempMember;

    printf("Enter Member ID to remove: ");
    scanf("%d", &id);
    getchar();

    filePtr = fopen("member.txt", "rb");
    if (filePtr == NULL) {
        printf("No member records found.\n");
        return;
    }

    tempFile = fopen("temp.txt", "wb");
    if (tempFile == NULL) {
        printf("Error opening temporary file.\n");
        fclose(filePtr);
        return;
    }

    while (fread(&tempMember, sizeof(tempMember), 1, filePtr) == 1) {
        if (tempMember.id != id) {
            fwrite(&tempMember, sizeof(tempMember), 1, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(filePtr);
    fclose(tempFile);

    remove("member.txt");
    rename("temp.txt", "member.txt");

    if (found) {
        printf("\nMember removed successfully.\n");
    } else {
        printf("\nMember not found.\n");
    }
}

void searchBookById() {
    int id;
    struct book *temp = head;

    printf("Enter Book ID to search: ");
    scanf("%d", &id);
    getchar();

    while (temp != NULL) {
        if (temp->id == id) {
            printf("\nBook Found:\n");
            printf("ID: %d\n", temp->id);
            printf("Title: %s\n", temp->title);
            printf("Author: %s\n", temp->author);
            printf("Publisher: %s\n", temp->publisher);
            printf("Issue Date: %s\n", temp->isIssued ? temp->issueDate : "Not Issued");
            printf("Issued: %s\n", temp->isIssued ? "Yes" : "No");
            return;
        }
        temp = temp->next;
    }

    printf("\nBook with ID %d not found.\n", id);
}