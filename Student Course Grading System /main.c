#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Student {
    char name[50];
    int quiz;
    int midterm;
    int assignment;
    int exam;
    int project;
    float total;
    char grade;
};

// -------------------- GRADE --------------------
char calculateGrade(float total) {
    if(total >= 45) return 'A';
    else if(total >= 40) return 'B';
    else if(total >= 35) return 'C';
    else if(total >= 30) return 'D';
    else return 'F';
}

// -------------------- FILE --------------------
FILE *fp;

// -------------------- REGISTER --------------------
void registerStudent() {
    struct Student s;

    fp = fopen("students.dat", "ab");

    printf("\nEnter name: ");
    scanf(" %49s", s.name);

    printf("Quiz: ");
    scanf("%d", &s.quiz);

    printf("Midterm: ");
    scanf("%d", &s.midterm);

    printf("Assignment: ");
    scanf("%d", &s.assignment);

    printf("Exam: ");
    scanf("%d", &s.exam);

    printf("Project: ");
    scanf("%d", &s.project);

    s.total = s.quiz + s.midterm + s.assignment + s.exam + s.project;
    s.grade = calculateGrade(s.total);

    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);

    printf("\nStudent added successfully!\n");
}

// -------------------- LIST ALL --------------------
void listStudents() {
    struct Student s;

    fp = fopen("students.dat", "rb");

    if(fp == NULL) {
        printf("\nNo records found!\n");
        return;
    }

    printf("\n--- ALL STUDENTS ---\n");

    while(fread(&s, sizeof(s), 1, fp)) {
        printf("\nName: %s", s.name);
        printf("\nTotal: %.2f", s.total);
        printf("\nGrade: %c\n", s.grade);
    }

    fclose(fp);
}

// -------------------- CASE INSENSITIVE COMPARE --------------------
int compareIgnoreCase(char a[], char b[]) {
    return strcasecmp(a, b); // works in Linux/Mac/modern compilers
}

// -------------------- SEARCH --------------------
void searchStudent() {
    struct Student s;
    char name[50];
    int found = 0;

    fp = fopen("students.dat", "rb");

    if(fp == NULL) {
        printf("\nNo records!\n");
        return;
    }

    printf("\nEnter name to search: ");
    scanf(" %49s", name);

    while(fread(&s, sizeof(s), 1, fp)) {
        if(compareIgnoreCase(s.name, name) == 0) {
            printf("\nFOUND STUDENT\n");
            printf("Name: %s\n", s.name);
            printf("Total: %.2f\n", s.total);
            printf("Grade: %c\n", s.grade);
            found = 1;
            break;
        }
    }

    if(!found)
        printf("\nStudent not found!\n");

    fclose(fp);
}

// -------------------- UPDATE MARKS --------------------
void updateStudent() {
    struct Student s;
    char name[50];
    FILE *temp;

    fp = fopen("students.dat", "rb");
    temp = fopen("temp.dat", "wb");

    if(fp == NULL) {
        printf("\nNo records!\n");
        return;
    }

    printf("\nEnter student name to update: ");
    scanf(" %49s", name);

    while(fread(&s, sizeof(s), 1, fp)) {

        if(compareIgnoreCase(s.name, name) == 0) {

            printf("\nEnter new marks:\n");

            printf("Quiz: ");
            scanf("%d", &s.quiz);

            printf("Midterm: ");
            scanf("%d", &s.midterm);

            printf("Assignment: ");
            scanf("%d", &s.assignment);

            printf("Exam: ");
            scanf("%d", &s.exam);

            printf("Project: ");
            scanf("%d", &s.project);

            s.total = s.quiz + s.midterm + s.assignment + s.exam + s.project;
            s.grade = calculateGrade(s.total);

            printf("\nUpdated successfully!\n");
        }

        fwrite(&s, sizeof(s), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("students.dat");
    rename("temp.dat", "students.dat");
}

// -------------------- DELETE --------------------
void deleteStudent() {
    struct Student s;
    char name[50];
    FILE *temp;

    fp = fopen("students.dat", "rb");
    temp = fopen("temp.dat", "wb");

    if(fp == NULL) {
        printf("\nNo records!\n");
        return;
    }

    printf("\nEnter name to delete: ");
    scanf(" %49s", name);

    while(fread(&s, sizeof(s), 1, fp)) {

        if(compareIgnoreCase(s.name, name) != 0) {
            fwrite(&s, sizeof(s), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("students.dat");
    rename("temp.dat", "students.dat");

    printf("\nDeleted successfully!\n");
}

// -------------------- MAIN MENU --------------------
int main() {
    int choice;

    while(1) {
        printf("\n===== STUDENT DATABASE SYSTEM =====\n");
        printf("1. Register Student\n");
        printf("2. List All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");

        printf("Choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: registerStudent(); break;
            case 2: listStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: exit(0);
            default: printf("\nInvalid choice!\n");
        }
    }

    return 0;
}