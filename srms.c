#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINE_MAX 512

typedef struct {
    char username[64];
    char password[64];
    char role[16];
} Credential;

typedef struct {
    char regNo[64];
    char name[128];
    char branch[64];
    float cgpa;
    char yearSem[32];
} Student;

Credential *creds = NULL;
size_t creds_count = 0;

Student *students = NULL;
size_t students_count = 0;

char *trim_inplace(char *s) {
    if (!s) return s;
    while (isspace((unsigned char)*s)) s++;
    if (*s == 0) return s;
    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--;
    *(end + 1) = 0;
    return s;
}

void load_credentials(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        creds = NULL;
        creds_count = 0;
        return;
    }
    char line[LINE_MAX];
    while (fgets(line, sizeof(line), f)) {
        char *p = line;
        if (trim_inplace(p)[0] == '\0') continue;
        char *user = strtok(p, ",");
        char *pass = strtok(NULL, ",");
        char *role = strtok(NULL, ",\n\r");
        if (!user || !pass || !role) continue;
        creds = realloc(creds, (creds_count + 1) * sizeof(Credential));
        strncpy(creds[creds_count].username, trim_inplace(user), sizeof(creds[creds_count].username)-1);
        creds[creds_count].username[sizeof(creds[creds_count].username)-1] = '\0';
        strncpy(creds[creds_count].password, trim_inplace(pass), sizeof(creds[creds_count].password)-1);
        creds[creds_count].password[sizeof(creds[creds_count].password)-1] = '\0';
        strncpy(creds[creds_count].role, trim_inplace(role), sizeof(creds[creds_count].role)-1);
        creds[creds_count].role[sizeof(creds[creds_count].role)-1] = '\0';
        creds_count++;
    }
    fclose(f);
}

void load_students(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        students = NULL;
        students_count = 0;
        return;
    }
    char line[LINE_MAX];
    while (fgets(line, sizeof(line), f)) {
        if (trim_inplace(line)[0] == '\0') continue;
        char *p = line;
        char *reg = strtok(p, ",");
        char *name = strtok(NULL, ",");
        char *branch = strtok(NULL, ",");
        char *cgpa_s = strtok(NULL, ",");
        char *year = strtok(NULL, ",\n\r");
        if (!reg || !name || !branch || !cgpa_s || !year) continue;
        students = realloc(students, (students_count + 1) * sizeof(Student));
        strncpy(students[students_count].regNo, trim_inplace(reg), sizeof(students[students_count].regNo)-1);
        students[students_count].regNo[sizeof(students[students_count].regNo)-1] = '\0';
        strncpy(students[students_count].name, trim_inplace(name), sizeof(students[students_count].name)-1);
        students[students_count].name[sizeof(students[students_count].name)-1] = '\0';
        strncpy(students[students_count].branch, trim_inplace(branch), sizeof(students[students_count].branch)-1);
        students[students_count].branch[sizeof(students[students_count].branch)-1] = '\0';
        students[students_count].cgpa = (float)atof(trim_inplace(cgpa_s));
        strncpy(students[students_count].yearSem, trim_inplace(year), sizeof(students[students_count].yearSem)-1);
        students[students_count].yearSem[sizeof(students[students_count].yearSem)-1] = '\0';
        students_count++;
    }
    fclose(f);
}

void save_students(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("error: cannot open %s for writing\n", filename);
        return;
    }
    for (size_t i = 0; i < students_count; ++i) {
        fprintf(f, "%s, %s, %s, %.2f, %s\n",
            students[i].regNo,
            students[i].name,
            students[i].branch,
            students[i].cgpa,
            students[i].yearSem);
    }
    fclose(f);
}

int find_student_index_by_reg(const char *reg) {
    for (size_t i = 0; i < students_count; ++i) {
        if (strcmp(students[i].regNo, reg) == 0) return (int)i;
    }
    return -1;
}

void press_enter_to_continue() {
    printf("\npress enter to continue...");
    int c = getchar();
    if (c != '\n') { while (getchar() != '\n' && getchar() != EOF); }
}

int cmp_reg(const void *a, const void *b) {
    const Student *sa = (const Student*)a;
    const Student *sb = (const Student*)b;
    return strcmp(sa->regNo, sb->regNo);
}
void display_all_students() {
    if (students_count == 0) {
        printf("no records found.\n");
        return;
    }
    Student *copy = malloc(students_count * sizeof(Student));
    if (!copy) return;
    for (size_t i = 0; i < students_count; ++i) copy[i] = students[i];
    qsort(copy, students_count, sizeof(Student), cmp_reg);

    printf("\n----------------------------------------------------------------------------\n");
    printf("%-12s %-20s %-12s %-6s %-8s\n", "regno", "name", "branch", "cgpa", "year/sem");
    printf("----------------------------------------------------------------------------\n");
    for (size_t i = 0; i < students_count; ++i) {
        printf("%-12s %-20s %-12s %-6.2f %-8s\n",
            copy[i].regNo, copy[i].name, copy[i].branch, copy[i].cgpa, copy[i].yearSem);
    }
    printf("----------------------------------------------------------------------------\n");
    free(copy);
}

void add_student() {
    Student s;
    printf("enter registration number: ");
    if (scanf("%63s", s.regNo) != 1) { while(getchar()!='\n'); return; }
    if (find_student_index_by_reg(s.regNo) != -1) {
        printf("registration number already exists!\n");
        while(getchar()!='\n');
        return;
    }
    while(getchar()!='\n');
    printf("enter name: ");
    if (!fgets(s.name, sizeof(s.name), stdin)) return;
    trim_inplace(s.name);
    printf("enter branch: ");
    if (!fgets(s.branch, sizeof(s.branch), stdin)) return;
    trim_inplace(s.branch);
    printf("enter cgpa: ");
    if (scanf("%f", &s.cgpa) != 1) { while(getchar()!='\n'); return; }
    while(getchar()!='\n');
    printf("enter year/sem: ");
    if (!fgets(s.yearSem, sizeof(s.yearSem), stdin)) return;
    trim_inplace(s.yearSem);

    students = realloc(students, (students_count + 1) * sizeof(Student));
    students[students_count++] = s;
    save_students("students.txt");
    printf("student added successfully.\n");
}

void search_by_reg() {
    char reg[64];
    printf("enter registration number to search: ");
    if (scanf("%63s", reg) != 1) { while(getchar()!='\n'); return; }
    while(getchar()!='\n');
    int idx = find_student_index_by_reg(reg);
    if (idx == -1) { printf("record not found.\n"); return; }
    Student *s = &students[idx];
    printf("\nrecord found:\n");
    printf("name     : %s\nbranch   : %s\ncgpa     : %.2f\nyear/sem : %s\n",
           s->name, s->branch, s->cgpa, s->yearSem);
}

void update_student() {
    char reg[64];
    printf("enter registration number to update: ");
    if (scanf("%63s", reg) != 1) { while(getchar()!='\n'); return; }
    while(getchar()!='\n');
    int idx = find_student_index_by_reg(reg);
    if (idx == -1) { printf("record not found.\n"); return; }
    Student *s = &students[idx];
    printf("editing details for %s\n", s->name);
    printf("new name (leave empty to keep current): ");
    char buf[256];
    if (!fgets(buf, sizeof(buf), stdin)) return;
    trim_inplace(buf);
    if (buf[0] != '\0') strncpy(s->name, buf, sizeof(s->name)-1);
    printf("new branch (leave empty to keep current): ");
    if (!fgets(buf, sizeof(buf), stdin)) return;
    trim_inplace(buf);
    if (buf[0] != '\0') strncpy(s->branch, buf, sizeof(s->branch)-1);
    printf("new cgpa (enter -1 to keep current): ");
    float cg;
    if (scanf("%f", &cg) == 1) {
        if (cg >= 0.0f) s->cgpa = cg;
    }
    while(getchar()!='\n');
    printf("new year/sem (leave empty to keep current): ");
    if (!fgets(buf, sizeof(buf), stdin)) return;
    trim_inplace(buf);
    if (buf[0] != '\0') strncpy(s->yearSem, buf, sizeof(s->yearSem)-1);

    save_students("students.txt");
    printf("record updated.\n");
}

void delete_student() {
    char reg[64];
    printf("enter registration number to delete: ");
    if (scanf("%63s", reg) != 1) { while(getchar()!='\n'); return; }
    while(getchar()!='\n');
    int idx = find_student_index_by_reg(reg);
    if (idx == -1) { printf("record not found.\n"); return; }
    for (size_t i = idx; i + 1 < students_count; ++i) students[i] = students[i+1];
    students_count--;
    if (students_count == 0) { free(students); students = NULL; }
    else students = realloc(students, students_count * sizeof(Student));
    save_students("students.txt");
    printf("record deleted.\n");
}

char role_buf[32];
int login(char *out_username, size_t un_size) {
    char username[64], password[64];
    printf("username: ");
    if (scanf("%63s", username) != 1) return 0;
    printf("password: ");
    if (scanf("%63s", password) != 1) return 0;
    for (size_t i = 0; i < creds_count; ++i) {
        if (strcmp(creds[i].username, username) == 0 &&
            strcmp(creds[i].password, password) == 0) {
            strncpy(out_username, username, un_size-1);
            out_username[un_size-1] = '\0';
            strncpy(role_buf, creds[i].role, sizeof(role_buf)-1);
            role_buf[sizeof(role_buf)-1] = '\0';
            return 1;
        }
    }
    return 0;
}

void menu_for_role(const char *role) {
    int choice = 0;
    while (1) {
        printf("\n============================================\n");
        printf("      student record management system\n");
        printf("role: %s\n", role);
        printf("============================================\n");

        if (strcmp(role, "admin") == 0) {
            printf("1. add student details\n");
            printf("2. display all students\n");
            printf("3. search student\n");
            printf("4. update student details\n");
            printf("5. delete student\n");
            printf("6. exit\n");
            printf("enter your choice: ");
            if (scanf("%d", &choice) != 1) { while(getchar()!='\n'); continue; }
            while(getchar()!='\n');

            switch(choice) {
                case 1: add_student(); press_enter_to_continue(); break;
                case 2: display_all_students(); press_enter_to_continue(); break;
                case 3: search_by_reg(); press_enter_to_continue(); break;
                case 4: update_student(); press_enter_to_continue(); break;
                case 5: delete_student(); press_enter_to_continue(); break;
                case 6: return;
                default: printf("invalid choice\n"); press_enter_to_continue();
            }
        }
        else if (strcmp(role, "staff") == 0) {
            printf("1. add student details\n");
            printf("2. display all students\n");
            printf("3. search student\n");
            printf("4. update student details\n");
            printf("5. exit\n");
            printf("enter your choice: ");
            if (scanf("%d", &choice) != 1) { while(getchar()!='\n'); continue; }
            while(getchar()!='\n');

            switch(choice) {
                case 1: add_student(); press_enter_to_continue(); break;
                case 2: display_all_students(); press_enter_to_continue(); break;
                case 3: search_by_reg(); press_enter_to_continue(); break;
                case 4: update_student(); press_enter_to_continue(); break;
                case 5: return;
                default: printf("invalid choice\n"); press_enter_to_continue();
            }
        }
        else {
            printf("1. display all students\n");
            printf("2. search student\n");
            printf("3. exit\n");
            printf("enter your choice: ");
            if (scanf("%d", &choice) != 1) { while(getchar()!='\n'); continue; }
            while(getchar()!='\n');

            switch(choice) {
                case 1: display_all_students(); press_enter_to_continue(); break;
                case 2: search_by_reg(); press_enter_to_continue(); break;
                case 3: return;
                default: printf("invalid choice\n"); press_enter_to_continue();
            }
        }
    }
}

void ensure_credentials_file() {
    FILE *f = fopen("credentials.txt", "r");
    if (f) { fclose(f); return; }
    f = fopen("credentials.txt", "w");
    if (!f) return;
    fprintf(f, "admin,adminpass,admin\n");
    fprintf(f, "staff,staffpass,staff\n");
    fprintf(f, "guest,guestpass,guest\n");
    fclose(f);
    printf("credentials.txt not found: created sample credentials. edit credentials.txt to customize.\n");
}

int main() {
    ensure_credentials_file();
    load_credentials("credentials.txt");
    load_students("students.txt");

    char username[64];
    printf("=== welcome to student record management system ===\n");
    int attempts = 0;
    while (1) {
        printf("\nplease login\n");
        if (login(username, sizeof(username))) {
            printf("login successful. welcome %s (role: %s)\n", username, role_buf);
            menu_for_role(role_buf);
            break;
        } else {
            printf("login failed. try again.\n");
            attempts++;
            if (attempts >= 3) {
                printf("too many failed attempts. exiting.\n");
                break;
            }
        }
    }

    save_students("students.txt");
    free(creds);
    free(students);
    printf("goodbye.\n");
    return 0;
}
