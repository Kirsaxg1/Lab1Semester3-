#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define LINE_MAX_LENGHT 1000

void create_fork_bomb(int height);
int search_in_file(const char* file_path, const char* str_search);

void create_fork_bomb(int height) {
    if (height <= 0) {
        return;
    }

    pid_t pid = fork();

    if (pid == 0) {
        create_fork_bomb(height - 1);
        sleep(1);
        exit(0);
    } else if (pid > 0) {
        create_fork_bomb(height - 1);
        wait(NULL);
    } else {
        exit(1);
    }
}

int search_in_file(const char* file_path, const char* str_search) {
    if (str_search == NULL || strlen(str_search) == 0) {
        exit(-1);
    }

    size_t count = 0;
    char buffer[LINE_MAX_LENGHT];

    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error open file: %s\n", file_path);
        exit(-1);
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char* pos = buffer;
        while ((pos = strstr(pos, str_search)) != NULL) {
            pos++;
            count++;
        }
    }
    fclose(file);
    exit(count);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Использование: %s <файл-со-списком> <строка>\n", argv[0]);
        return 1;
    }

    char buffer[LINE_MAX_LENGHT];
    char* search_str = argv[2];

    FILE* list_file = fopen(argv[1], "r");
    if (list_file == NULL) {
        printf("Error open file %s\n", argv[1]);
        return 1;
    }

    size_t count_file = 0;
    while (fgets(buffer, sizeof(buffer), list_file) != NULL) {
        count_file++;
    }
    rewind(list_file);

    // FIX: Правильный тип для PID
    pid_t* child_pids = malloc(count_file * sizeof(pid_t));
    char** files = malloc(count_file * sizeof(char*));

    if (child_pids == NULL || files == NULL) {
        printf("Error memory allocation\n");
        fclose(list_file);
        free(child_pids);
        free(files);
        return 1;
    }

    // FIX: Читаем и сохраняем имена файлов
    for (size_t i = 0; i < count_file; i++) {
        if (fgets(buffer, sizeof(buffer), list_file) == NULL) break;
        buffer[strcspn(buffer, "\n")] = '\0';
        files[i] = strdup(buffer);
        if (files[i] == NULL) {
            printf("Error allocating memory for file name\n");
            // Cleanup
            for (size_t j = 0; j < i; j++) free(files[j]);
            fclose(list_file);
            free(child_pids);
            free(files);
            return 1;
        }
    }
    fclose(list_file);

    // FIX: Создаем процессы и сохраняем реальные PID
    for (size_t i = 0; i < count_file; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            search_in_file(files[i], search_str);
        } else if (pid > 0) {
            child_pids[i] = pid; // FIX: Сохраняем реальный PID
        } else {
            printf("Error fork for %s\n", files[i]);
            child_pids[i] = -1; // Помечаем ошибку
        }
    }

    int total_found = 0;
    for (size_t i = 0; i < count_file; i++) {
        if (child_pids[i] == -1) continue; // Пропускаем неудачные fork
        
        int status;
        waitpid(child_pids[i], &status, 0);

        if (WIFEXITED(status)) {
            int result = WEXITSTATUS(status);
            if (result > 0) {
                printf("File %s: found %d occurrences\n", files[i], result);
                total_found += result;
            } else if (result == 255) {
                printf("Error processing file: %s\n", files[i]);
            }
        }
    }

    if (total_found == 0) {
        printf("Str '%s' not found in any files\nFork bomb - ON\n", search_str);
        create_fork_bomb(strlen(search_str));
    }

    // FIX: Освобождаем память
    for (size_t i = 0; i < count_file; i++) {
        free(files[i]);
    }
    free(files);
    free(child_pids);

    return 0;
}
