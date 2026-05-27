#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// -------------------- STRUCTS --------------------

struct Student {
    char name[50];
    char password[50];
    int score;
};

struct Question {
    char question[100];
    char A[50];
    char B[50];
    char C[50];
    char D[50];
    char correct;
};

// -------------------- REGISTER STUDENT --------------------

void registerStudent() {
    struct Student s;
    FILE *fp;

    fp = fopen("students.dat", "ab");

    printf("\nEnter name: ");
    scanf(" %49s", s.name);

    printf("Enter password: ");
    scanf(" %49s", s.password);

    s.score = 0;

    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);

    printf("\nStudent registered successfully!\n");
}

// -------------------- LOGIN --------------------

int login(char name[], char pass[]) {
    struct Student s;
    FILE *fp;

    fp = fopen("students.dat", "rb");

    if(fp == NULL) return 0;

    while(fread(&s, sizeof(s), 1, fp)) {
        if(strcmp(s.name, name) == 0 && strcmp(s.password, pass) == 0) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// -------------------- ADD QUESTION (ADMIN) --------------------

void addQuestion() {
    struct Question q;
    FILE *fp;

    fp = fopen("questions.dat", "ab");

    printf("\nEnter question: ");
    scanf(" %[^\n]", q.question);

    printf("Option A: ");
    scanf(" %[^\n]", q.A);

    printf("Option B: ");
    scanf(" %[^\n]", q.B);

    printf("Option C: ");
    scanf(" %[^\n]", q.C);

    printf("Option D: ");
    scanf(" %[^\n]", q.D);

    printf("Correct option (A/B/C/D): ");
    scanf(" %c", &q.correct);

    fwrite(&q, sizeof(q), 1, fp);
    fclose(fp);

    printf("\nQuestion added successfully!\n");
}

// -------------------- UPDATE SCORE --------------------

void updateScore(char name[], int newScore) {
    struct Student s;
    FILE *fp, *temp;

    fp = fopen("students.dat", "rb");
    temp = fopen("temp.dat", "wb");

    while(fread(&s, sizeof(s), 1, fp)) {

        if(strcmp(s.name, name) == 0) {
            s.score = newScore;
        }

        fwrite(&s, sizeof(s), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("students.dat");
    rename("temp.dat", "students.dat");
}

// -------------------- QUIZ ENGINE --------------------

void takeQuiz(char studentName[]) {
    struct Question q;
    FILE *fp;

    char answer;
    int score = 0;

    fp = fopen("questions.dat", "rb");

    if(fp == NULL) {
        printf("\nNo questions available!\n");
        return;
    }

    printf("\n===== QUIZ START =====\n");

    while(fread(&q, sizeof(q), 1, fp)) {

        printf("\n%s\n", q.question);
        printf("A. %s\n", q.A);
        printf("B. %s\n", q.B);
        printf("C. %s\n", q.C);
        printf("D. %s\n", q.D);

        printf("Answer: ");
        scanf(" %c", &answer);

        if(answer == q.correct) {
            score++;
        }
    }

    fclose(fp);

    printf("\nQuiz finished!\nYour score: %d\n", score);

    updateScore(studentName, score);
}

// -------------------- LEADERBOARD --------------------

void leaderboard() {
    struct Student s;
    FILE *fp;

    fp = fopen("students.dat", "rb");

    if(fp == NULL) {
        printf("\nNo students found!\n");
        return;
    }

    printf("\n===== LEADERBOARD =====\n");

    while(fread(&s, sizeof(s), 1, fp)) {
        printf("Name: %s | Score: %d\n", s.name, s.score);
    }

    fclose(fp);
}

// -------------------- MAIN --------------------

int main() {
    int choice;
    char name[50], pass[50];

    while(1) {

        printf("\n\n===== LEVEL 2 QUIZ SYSTEM =====\n");
        printf("1. Register Student\n");
        printf("2. Take Quiz (Login)\n");
        printf("3. Add Question (Admin)\n");
        printf("4. Leaderboard\n");
        printf("5. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {

            case 1:
                registerStudent();
                break;

            case 2:
                printf("\nEnter name: ");
                scanf(" %49s", name);

                printf("Enter password: ");
                scanf(" %49s", pass);

                if(login(name, pass)) {
                    takeQuiz(name);
                } else {
                    printf("\nInvalid login!\n");
                }
                break;

            case 3:
                addQuestion();
                break;

            case 4:
                leaderboard();
                break;

            case 5:
                exit(0);

            default:
                printf("\nInvalid choice!\n");
        }
    }

    return 0;
}