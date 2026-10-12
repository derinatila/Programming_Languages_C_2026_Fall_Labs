// week5_task3_student_management_system.c
// Name: Derin Atila   Surname: Atila   Student ID: 251ADC003
//
// Task 3: Mini-project - Student management system with file persistence
// Week 5 - Files & Modular Programming
//
// Build: make lab5   Run: ./bin/week5_task3_student_management_system

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50
#define LINE_LEN 256
#define DATA_FILE "students.txt"

typedef struct Student {
    char name[NAME_LEN];
    int id;
    float gpa;
} Student;

// Function prototypes (do not change these signatures — the autograder calls them directly)
int  load_students(Student arr[], int max);    // returns number of records loaded
int  save_students(const Student arr[], int count);  // 0 on success, -1 on error
void add_student(Student arr[], int *count);
void list_students(const Student arr[], int count);
int  read_line(const char *prompt, char *buf, size_t size);

int main(void) {
    Student students[MAX_STUDENTS];
    int count = 0;
    char input[LINE_LEN];
    int running = 1;

    // TODO: Load existing data: count = load_students(students, MAX_STUDENTS);
    count = load_students(students, MAX_STUDENTS);
    printf("Loaded %d student(s) from %s\n", count, DATA_FILE);

    while (running) {
        printf("\n=== Student Management System ===\n");
        printf("1. List students\n");
        printf("2. Add student\n");
        printf("3. Save and Exit\n");

        int choice;
        if (read_line("Select an option: ", input, sizeof input) != 0) {
            choice = 3;  // end of input: save and exit instead of looping forever
        } else {
            char *end;
            long value = strtol(input, &end, 10);
            choice = (end != input && *end == '\0') ? (int)value : 0;  // non-numbers become invalid
        }

        switch (choice) {
            case 1:
                // TODO: Call list_students()
                list_students(students, count);
                break;
            case 2:
                // TODO: Call add_student()
                add_student(students, &count);
                break;
            case 3:
                // TODO: Call save_students(). If it fails, print an error message.
                if (save_students(students, count) != 0) {
                    fprintf(stderr, "Error saving students.\n");
                } else {
                    printf("Data saved. Goodbye!\n");
                }
                running = 0;
                break;
            default:
                printf("Invalid option. Try again.\n");
        }
    }

    return EXIT_SUCCESS;
}

// Provided helper — prints a prompt and reads one line from stdin into buf.
// The trailing newline is removed. Returns 0 on success, -1 on end of input (EOF).
int read_line(const char *prompt, char *buf, size_t size) {
    printf("%s", prompt);
    fflush(stdout);
    if (fgets(buf, (int)size, stdin) == NULL) {
        return -1;
    }
    size_t len = strcspn(buf, "\r\n");
    if (buf[len] == '\0' && len == size - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    buf[len] = '\0';
    return 0;
}

// TODO: Implement load_students()
int load_students(Student arr[], int max) {
    FILE *fp = fopen(DATA_FILE, "r");
    if (fp == NULL) {
        return 0;  // first run: no file is not an error
    }

    int count = 0;
    char line[LINE_LEN];
    while (fgets(line, sizeof(line), fp) != NULL && count < max) {
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0') continue;  // skip blank lines

        Student s;
        if (sscanf(line, "%49[^,],%d,%f", s.name, &s.id, &s.gpa) == 3) {
            arr[count++] = s;
        }
        // malformed lines are silently skipped
    }
    fclose(fp);
    return count;
}

// TODO: Implement save_students()
int save_students(const Student arr[], int count) {
    FILE *fp = fopen(DATA_FILE, "w");
    if (fp == NULL) {
        return -1;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%d,%.2f\n", arr[i].name, arr[i].id, arr[i].gpa);
    }
    fclose(fp);
    return 0;
}

// TODO: Implement add_student()
void add_student(Student arr[], int *count) {
    if (*count >= MAX_STUDENTS) {
        printf("Cannot add more students (limit reached).\n");
        return;
    }

    char nameBuf[NAME_LEN];
    char idBuf[32];
    char gpaBuf[32];

    if (read_line("Enter name: ", nameBuf, sizeof(nameBuf)) != 0) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    if (read_line("Enter ID: ", idBuf, sizeof(idBuf)) != 0) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    if (read_line("Enter GPA: ", gpaBuf, sizeof(gpaBuf)) != 0) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    // Validate name: not empty, no comma
    if (nameBuf[0] == '\0' || strchr(nameBuf, ',') != NULL) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    // Validate ID: must be a full integer
    char *endId;
    long idLong = strtol(idBuf, &endId, 10);
    if (endId == idBuf || *endId != '\0') {
        printf("Invalid input. Student not added.\n");
        return;
    }

    // Validate GPA: must be full float in [0.0, 4.0]
    char *endGpa;
    float gpaVal = strtof(gpaBuf, &endGpa);
    if (endGpa == gpaBuf || *endGpa != '\0' || gpaVal < 0.0f || gpaVal > 4.0f) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    strncpy(arr[*count].name, nameBuf, NAME_LEN - 1);
    arr[*count].name[NAME_LEN - 1] = '\0';
    arr[*count].id = (int)idLong;
    arr[*count].gpa = gpaVal;
    (*count)++;

    printf("Student added successfully!\n");
}

// TODO: Implement list_students()
void list_students(const Student arr[], int count) {
    if (count == 0) {
        printf("No students found.\n");
        return;
    }
    printf("%-5s %-20s %s\n", "ID", "Name", "GPA");
    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %.2f\n", arr[i].id, arr[i].name, arr[i].gpa);
    }
}