#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For toupper function

#define MAX_DESC_LENGTH 100

// Define the Entry structure
typedef struct {
    char description[MAX_DESC_LENGTH];
    double amount;
    char type; // 'I' for income, 'E' for expense
} Entry;

// Function prototypes
void clearInputBuffer();
int getMenuChoice();
void addEntry(Entry **entries, int *count);
void viewEntries(Entry *entries, int count);
double calculateBalance(Entry *entries, int count);
void saveToFile(Entry *entries, int count, const char *filename);
int loadFromFile(Entry **entries, const char *filename);

// Clear input buffer to handle extra characters
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Get validated menu choice
int getMenuChoice() {
    int choice;
    while (1) {
        printf("\nSimple Budget Tracker\n");
        printf("1. Add Entry\n");
        printf("2. View Entries\n");
        printf("3. Calculate Total Balance\n");
        printf("4. Save and Exit\n");
        printf("Enter your choice: ");
        
        // Check if input is a valid integer
        if (scanf("%d", &choice) == 1) {
            clearInputBuffer(); // Clear any extra input
            
            // Check if choice is within valid range
            if (choice >= 1 && choice <= 4) {
                return choice;
            }
        } else {
            clearInputBuffer(); // Clear invalid input
        }
        
        printf("Invalid input. Please enter a number between 1 and 4.\n");
    }
}

int main() {
    Entry *entries = NULL;
    int count = 0;
    int choice;

    // Load entries from file
    count = loadFromFile(&entries, "budget.dat");

    do {
        choice = getMenuChoice(); // Use new input handling function

        switch (choice) {
            case 1:
                addEntry(&entries, &count);
                break;
            case 2:
                viewEntries(entries, count);
                break;
            case 3:
                printf("\nTotal Balance: %.2f\n", calculateBalance(entries, count));
                break;
            case 4:
                saveToFile(entries, count, "budget.dat");
                printf("Data saved successfully. Exiting...\n");
                break;
        }
    } while (choice != 4);

    free(entries);
    return 0;
}

void addEntry(Entry **entries, int *count) {
    Entry newEntry;

    // Description input
    printf("Enter description: ");
    if (fgets(newEntry.description, MAX_DESC_LENGTH, stdin) == NULL) {
        printf("Error reading description.\n");
        return;
    }
    newEntry.description[strcspn(newEntry.description, "\n")] = '\0';

    // Amount input with validation
    while (1) {
        printf("Enter amount: ");
        if (scanf("%lf", &newEntry.amount) == 1) {
            clearInputBuffer();
            if (newEntry.amount >= 0) break;
        }
        clearInputBuffer();
        printf("Invalid amount. Please enter a non-negative number.\n");
    }

    // Type input with validation
    while (1) {
        printf("Enter type (I for income, E for expense): ");
        char type;
        if (scanf(" %c", &type) == 1) {
            clearInputBuffer();
            type = toupper(type);
            if (type == 'I' || type == 'E') {
                newEntry.type = type;
                break;
            }
        }
        clearInputBuffer();
        printf("Invalid type. Please enter 'I' for income or 'E' for expense.\n");
    }

    // Add new entry
    (*count)++;
    *entries = realloc(*entries, (*count) * sizeof(Entry));
    if (*entries == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    (*entries)[*count - 1] = newEntry;
    printf("Entry added successfully.\n");
}

void viewEntries(Entry *entries, int count) {
    if (count == 0) {
        printf("No entries to display.\n");
        return;
    }

    printf("\nList of Entries:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s | %.2f | %s\n", i + 1, entries[i].description, entries[i].amount,
               (entries[i].type == 'I') ? "Income" : "Expense");
    }
}

double calculateBalance(Entry *entries, int count) {
    double balance = 0.0;

    for (int i = 0; i < count; i++) {
        if (entries[i].type == 'I') {
            balance += entries[i].amount;
        } else if (entries[i].type == 'E') {
            balance -= entries[i].amount;
        }
    }

    return balance;
}

void saveToFile(Entry *entries, int count, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fwrite(&count, sizeof(int), 1, file);
    fwrite(entries, sizeof(Entry), count, file);

    fclose(file);
}

int loadFromFile(Entry **entries, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("No existing data file found. Starting fresh.\n");
        return 0;
    }

    int count;
    fread(&count, sizeof(int), 1, file);

    *entries = malloc(count * sizeof(Entry));
    if (*entries == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    fread(*entries, sizeof(Entry), count, file);

    fclose(file);
    return count;
}