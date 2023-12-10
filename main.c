#include <stdio.h>
#include <string.h>

struct Student {
    char firstName[20];
    char lastName[20];
    int age;
    char grade[10];
    char score[5];
    int studentId;
};

int areStringsEqual(char *str1, char *str2) {
    return strcmp(str1, str2) == 0 ? 0 : 1;
}

void displayStudent(struct Student student) {
    printf("Студент ID: %d\n", student.studentId);
    printf("Имя: %s\n", student.firstName);
    printf("Фамилия: %s\n", student.lastName);
    printf("Возраст: %d\n", student.age);
    printf("Класс: %s\n", student.grade);
    printf("Средний балл: %s\n\n", student.score);
}

int getMaxId() {
    FILE *db = fopen("students.dat", "rb");

    if (db == NULL) {
        printf("База данных не найдена.\n");
        return 0;
    }

    struct Student student;
    int highestId = 0;

    while (fread(&student, sizeof(struct Student), 1, db)) {
        if (student.studentId > highestId) {
            highestId = student.studentId;
        }
    }

    fclose(db);
    return highestId;
}

void findStudent() {
    FILE *db = fopen("students.dat", "rb");
    struct Student student;

    printf("Введите ID, имя, фамилию, возраст, класс и средний балл через пробел.\nЕсли неизвестно, напишите 0.\nВведите: ");
    int id, age;
    char firstName[20], lastName[20], grade[10], score[5];
    scanf("%d %s %s %d %s %s", &id, firstName, lastName, &age, grade, score);
    printf("\n");

    int found = 0;
    while (fread(&student, sizeof(struct Student), 1, db)) {
        if ((id == 0 || id == student.studentId) &&
            (areStringsEqual(firstName, "0") || areStringsEqual(firstName, student.firstName)) &&
            (areStringsEqual(lastName, "0") || areStringsEqual(lastName, student.lastName)) &&
            (age == 0 || age == student.age) &&
            (areStringsEqual(grade, "0") || areStringsEqual(grade, student.grade)) &&
            (areStringsEqual(score, "0") || areStringsEqual(score, student.score))) {
            displayStudent(student);
            found = 1;
        }
    }

    fclose(db);
}

void listStudents() {
    FILE *db = fopen("students.dat", "rb");

    struct Student student;

    while (fread(&student, sizeof(struct Student), 1, db)) {
        displayStudent(student);
        
    }

    fclose(db);

    

    printf("Хотите найти студента в базе данных?\n 1.Да\n2.Нет\nВведите номер: ");
    char response[4];
    scanf("%s", response);

    if (areStringsEqual(response, "1") == 0) {
        findStudent();
    }
}

void addStudent(int highestId) {
    FILE *db = fopen("students.dat", "ab");
    struct Student student;

    printf("Имя студента: ");
    scanf("%s", student.firstName);

    printf("Фамилия: ");
    scanf("%s", student.lastName);

    printf("Возраст: ");
    scanf("%d", &student.age);

    printf("Класс: ");
    scanf("%s", student.grade);

    printf("Средний балл: ");
    scanf("%s", student.score);

    student.studentId = highestId + 1;

    fwrite(&student, sizeof(struct Student), 1, db);
    fclose(db);
}

void removeStudent(int id) {
    FILE *db = fopen("students.dat", "r+");
    FILE *tempDb = fopen("temp_students.dat", "w");
    struct Student student;

    int found = 0;
    while (fread(&student, sizeof(struct Student), 1, db)) {
        if (student.studentId != id) {
            fwrite(&student, sizeof(struct Student), 1, tempDb);
        } else {
            found = 1;
        }
    }

    fclose(db);
    fclose(tempDb);

    if (found) {
        remove("students.dat");
        rename("temp_students.dat", "students.dat");
        printf("Студент с ID %d удален.\n", id);
    } else {
        remove("temp_students.dat");
        printf("Студент с ID %d не найден.\n", id);
    }
}

int main() {
    int highestId;
    int running = 1;

    while (running) {
        int selection;
        
        printf("\nВыберите действие:\n");
        printf("1 - Показать студента\n2 - Добавить студента\n3 - Удалить студента\n4 - Выйти\n");
        printf("Ваш выбор: ");
        scanf("%d", &selection);
        printf("\n");

        switch (selection) {
            case 1:
                listStudents();
                break;
            case 2:
                highestId = getMaxId();
                addStudent(highestId);
                break;
            case 3:
                printf("Введите ID студента для удаления: ");
                int toDelete;
                scanf("%d", &toDelete);
                removeStudent(toDelete);
                break;
            case 4:
                running = 0;
                break;
            default:
                printf("Неверный выбор. Попробуйте еще раз.\n");
                break;
        }
    }

    return 0;
}
