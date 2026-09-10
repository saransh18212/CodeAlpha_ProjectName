#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "students.dat"
#define NAME_LENGTH 60
#define COURSE_LENGTH 60

typedef struct {
    int id;
    char name[NAME_LENGTH];
    int age;
    char course[COURSE_LENGTH];
    float marks;
} Student;

void clear_input(void)
{
    int character;
    while ((character = getchar()) != '\n' && character != EOF) {
    }
}

void read_line(char *text, size_t size)
{
    if (fgets(text, (int)size, stdin) == NULL) {
        text[0] = '\0';
        return;
    }
    text[strcspn(text, "\n")] = '\0';
}

int find_student(int id, Student *student, long *position)
{
    FILE *file = fopen(DATA_FILE, "rb");
    Student record;
    Student *result = student != NULL ? student : &record;
    long current_position;

    if (file == NULL) {
        return 0;
    }

    while ((current_position = ftell(file)) >= 0 && fread(result, sizeof(*result), 1, file) == 1) {
        if (result->id == id) {
            if (position != NULL) {
                *position = current_position;
            }
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void add_student(void)
{
    Student student;
    FILE *file;

    printf("Student ID: ");
    if (scanf("%d", &student.id) != 1) {
        clear_input();
        printf("Invalid ID.\n");
        return;
    }
    clear_input();

    if (find_student(student.id, NULL, NULL)) {
        printf("A student with that ID already exists.\n");
        return;
    }

    printf("Name: ");
    read_line(student.name, sizeof(student.name));
    printf("Age: ");
    if (scanf("%d", &student.age) != 1 || student.age < 1) {
        clear_input();
        printf("Invalid age.\n");
        return;
    }
    clear_input();
    printf("Course: ");
    read_line(student.course, sizeof(student.course));
    printf("Marks: ");
    if (scanf("%f", &student.marks) != 1 || student.marks < 0.0f || student.marks > 100.0f) {
        clear_input();
        printf("Marks must be between 0 and 100.\n");
        return;
    }
    clear_input();

    file = fopen(DATA_FILE, "ab");
    if (file == NULL || fwrite(&student, sizeof(student), 1, file) != 1) {
        printf("Unable to save the student record.\n");
        if (file != NULL) {
            fclose(file);
        }
        return;
    }
    fclose(file);
    printf("Student added successfully.\n");
}

void display_student(const Student *student)
{
    printf("%-8d %-24s %-6d %-24s %6.2f\n", student->id, student->name, student->age, student->course, student->marks);
}

void display_students(void)
{
    Student student;
    FILE *file = fopen(DATA_FILE, "rb");
    int count = 0;

    if (file == NULL) {
        printf("No student records found.\n");
        return;
    }

    printf("%-8s %-24s %-6s %-24s %s\n", "ID", "Name", "Age", "Course", "Marks");
    printf("--------------------------------------------------------------------------\n");
    while (fread(&student, sizeof(student), 1, file) == 1) {
        display_student(&student);
        count++;
    }
    fclose(file);

    if (count == 0) {
        printf("No student records found.\n");
    }
}

void search_student(void)
{
    int id;
    Student student;

    printf("Enter student ID: ");
    if (scanf("%d", &id) != 1) {
        clear_input();
        printf("Invalid ID.\n");
        return;
    }
    clear_input();

    if (find_student(id, &student, NULL)) {
        printf("ID       Name                     Age    Course                   Marks\n");
        display_student(&student);
    } else {
        printf("Student not found.\n");
    }
}

void update_student(void)
{
    int id;
    Student student;
    long position;
    FILE *file;

    printf("Enter student ID to update: ");
    if (scanf("%d", &id) != 1) {
        clear_input();
        printf("Invalid ID.\n");
        return;
    }
    clear_input();

    if (!find_student(id, &student, &position)) {
        printf("Student not found.\n");
        return;
    }

    printf("New name (current: %s): ", student.name);
    read_line(student.name, sizeof(student.name));
    printf("New age (current: %d): ", student.age);
    if (scanf("%d", &student.age) != 1 || student.age < 1) {
        clear_input();
        printf("Invalid age.\n");
        return;
    }
    clear_input();
    printf("New course (current: %s): ", student.course);
    read_line(student.course, sizeof(student.course));
    printf("New marks (current: %.2f): ", student.marks);
    if (scanf("%f", &student.marks) != 1 || student.marks < 0.0f || student.marks > 100.0f) {
        clear_input();
        printf("Marks must be between 0 and 100.\n");
        return;
    }
    clear_input();

    file = fopen(DATA_FILE, "r+b");
    if (file == NULL || fseek(file, position, SEEK_SET) != 0 || fwrite(&student, sizeof(student), 1, file) != 1) {
        printf("Unable to update the student record.\n");
        if (file != NULL) {
            fclose(file);
        }
        return;
    }
    fclose(file);
    printf("Student updated successfully.\n");
}

void delete_student(void)
{
    int id;
    Student student;
    FILE *source;
    FILE *temporary;
    int found = 0;

    printf("Enter student ID to delete: ");
    if (scanf("%d", &id) != 1) {
        clear_input();
        printf("Invalid ID.\n");
        return;
    }
    clear_input();

    source = fopen(DATA_FILE, "rb");
    temporary = fopen("students.tmp", "wb");
    if (source == NULL || temporary == NULL) {
        printf("No student records found or file could not be opened.\n");
        if (source != NULL) {
            fclose(source);
        }
        if (temporary != NULL) {
            fclose(temporary);
            remove("students.tmp");
        }
        return;
    }

    while (fread(&student, sizeof(student), 1, source) == 1) {
        if (student.id == id) {
            found = 1;
        } else {
            fwrite(&student, sizeof(student), 1, temporary);
        }
    }
    fclose(source);
    fclose(temporary);

    if (found && remove(DATA_FILE) == 0 && rename("students.tmp", DATA_FILE) == 0) {
        printf("Student deleted successfully.\n");
    } else {
        remove("students.tmp");
        printf(found ? "Unable to update the student file.\n" : "Student not found.\n");
    }
}

int main(void)
{
    int choice;

    do {
        printf("\n=== Student Management System ===\n");
        printf("1. Add student\n");
        printf("2. Display all students\n");
        printf("3. Search student\n");
        printf("4. Update student\n");
        printf("5. Delete student\n");
        printf("0. Exit\n");
        printf("Choose an option: ");

        if (scanf("%d", &choice) != 1) {
            clear_input();
            printf("Please enter a number.\n");
            continue;
        }
        clear_input();

        switch (choice) {
            case 1: add_student(); break;
            case 2: display_students(); break;
            case 3: search_student(); break;
            case 4: update_student(); break;
            case 5: delete_student(); break;
            case 0: printf("Goodbye.\n"); break;
            default: printf("Invalid option.\n");
        }
    } while (choice != 0);

    return 0;
}
