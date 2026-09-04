#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024

// Node definition for a line
typedef struct LineNode {
    char *text;
    struct LineNode *next;
} LineNode;

// Function Prototypes
void display_document(LineNode *head);
LineNode* insert_line(LineNode *head, int line_num, const char *text);
LineNode* delete_line(LineNode *head, int line_num);
void save_to_file(LineNode *head, const char *filename);
LineNode* load_from_file(const char *filename);
void show_stats(LineNode *head);
void free_document(LineNode *head);

int main() {
    LineNode *head = NULL;
    char command[32];
    char buffer[MAX_LINE_LEN];
    int line_num;

    printf("--- Welcome to the Team C Line Editor ---\n");
    printf("Type 'help' for a list of commands.\n\n");

    while (1) {
        printf("> ");
        if (scanf("%31s", command) != 1) break;

        if (strcmp(command, "display") == 0) {
            display_document(head);
        } 
        else if (strcmp(command, "insert") == 0) {
            if (scanf("%d", &line_num) != 1) {
                printf("Error: Invalid line number format.\n");
                continue;
            }
            // Consume the space and read the rest of the line
            getchar(); 
            if (fgets(buffer, MAX_LINE_LEN, stdin)) {
                buffer[strcspn(buffer, "\n")] = 0; // Strip trailing newline
                head = insert_line(head, line_num, buffer);
            }
        } 
        else if (strcmp(command, "delete") == 0) {
            if (scanf("%d", &line_num) != 1) {
                printf("Error: Invalid line number format.\n");
                continue;
            }
            head = delete_line(head, line_num);
        } 
        else if (strcmp(command, "save") == 0) {
            if (scanf("%1023s", buffer) != 1) continue;
            save_to_file(head, buffer);
        } 
        else if (strcmp(command, "load") == 0) {
            if (scanf("%1023s", buffer) != 1) continue;
            free_document(head);
            head = load_from_file(buffer);
        } 
        else if (strcmp(command, "stats") == 0) {
            show_stats(head);
        } 
        else if (strcmp(command, "help") == 0) {
            printf("Commands: display, insert [num] [text], delete [num], save [file], load [file], stats, exit\n");
        } 
        else if (strcmp(command, "exit") == 0) {
            break;
        } 
        else {
            printf("Error: Unknown command. Type 'help'.\n");
        }
    }

    free_document(head);
    return 0;
}

// CORE FEATURE: Display the document
void display_document(LineNode *head) {
    if (!head) {
        printf("[Document is empty]\n");
        return;
    }
    int count = 1;
    LineNode *curr = head;
    while (curr) {
        printf("%d: %s\n", count++, curr->text);
        curr = curr->next;
    }
}

// CORE FEATURE: Insert a line (1-indexed)
LineNode* insert_line(LineNode *head, int line_num, const char *text) {
    if (line_num < 1) {
        printf("Error: Line numbers start at 1.\n");
        return head;
    }

    LineNode *new_node = malloc(sizeof(LineNode));
    new_node->text = strdup(text);
    new_node->next = NULL;

    // Insert at front
    if (line_num == 1) {
        new_node->next = head;
        return new_node;
    }

    LineNode *curr = head;
    for (int i = 1; curr != NULL && i < line_num - 1; i++) {
        curr = curr->next;
    }

    if (!curr) {
        printf("Error: Line number out of bounds. Appending to end instead.\n");
        if (!head) {
            return new_node;
        }
        curr = head;
        while (curr->next) curr = curr->next;
    }

    new_node->next = curr->next;
    curr->next = new_node;
    return head;
}

// CORE FEATURE: Delete a line
LineNode* delete_line(LineNode *head, int line_num) {
    if (!head) {
        printf("Error: Document is empty.\n");
        return NULL;
    }
    if (line_num < 1) {
        printf("Error: Invalid line number.\n");
        return head;
    }

    LineNode *temp = head;

    if (line_num == 1) {
        head = head->next;
        free(temp->text);
        free(temp);
        return head;
    }

    LineNode *prev = NULL;
    for (int i = 1; temp != NULL && i < line_num; i++) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) {
        printf("Error: Line %d does not exist.\n", line_num);
        return head;
    }

    prev->next = temp->next;
    free(temp->text);
    free(temp);
    return head;
}

// CORE FEATURE: Save to a file
void save_to_file(LineNode *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file for writing.\n");
        return;
    }
    LineNode *curr = head;
    while (curr) {
        fprintf(file, "%s\n", curr->text);
        curr = curr->next;
    }
    fclose(file);
    printf("Document successfully saved to %s\n", filename);
}

// CORE FEATURE: Load from a file
LineNode* load_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s. Starting empty.\n", filename);
        return NULL;
    }
    LineNode *head = NULL, *tail = NULL;
    char buffer[MAX_LINE_LEN];

    while (fgets(buffer, MAX_LINE_LEN, file)) {
        buffer[strcspn(buffer, "\n")] = 0;
        LineNode *new_node = malloc(sizeof(LineNode));
        new_node->text = strdup(buffer);
        new_node->next = NULL;

        if (!head) {
            head = new_node;
            tail = head;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }
    fclose(file);
    printf("Loaded document from %s\n", filename);
    return head;
}

// BONUS FEATURE: Document Statistics
void show_stats(LineNode *head) {
    int lines = 0, words = 0;
    LineNode *curr = head;
    while (curr) {
        lines++;
        char *str = strdup(curr->text);
        char *token = strtok(str, " \t");
        while (token) {
            words++;
            token = strtok(NULL, " \t");
        }
        free(str);
        curr = curr->next;
    }
    printf("Statistics: %d Lines, %d Words\n", lines, words);
}

// Utility: Clean memory
void free_document(LineNode *head) {
    while (head) {
        LineNode *temp = head;
        head = head->next;
        free(temp->text);
        free(temp);
    }
}
