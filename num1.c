/*getuid (void) — возвращает реальный пользовательский ID процесса;
getgid (void) — возвращает реальный ID группы процесса;
geteuid (void) — возвращает эффективный пользовательский ID процесса;
getegid (void) — возвращает эффективный ID группы процесса.
Pid_t тип данных - целое число со знаком, который способен представить ID процесса. В библиотеке GNU, это - int.*/


#include <unistd.h>
#include <stdio.h> 
#include <sys/types.h>

int main(){

    printf(" xxx ===== Некоторые Атрибуты Текущего Процесса ===== xxx \n");

    pid_t current_pid = getpid();
    pid_t parent_pid = getppid();
    pid_t group_id = getpgid(0);
    uid_t real_uid = getuid();
    gid_t real_gid = getgid();
    uid_t effective_uid = geteuid();
    gid_t effective_gid = getegid();

    printf("1. Идентификатор текущего процесса (PID): %d\n", current_pid);
    printf("2. Идентификатор родительского процесса (PPID): %d\n", parent_pid);
    printf("3. Идентификатор группы вызывающего процесса (PGID): %d\n", group_id);
    printf("4. Реальный идентификатор владельца (RUID): %d\n", real_uid);
    printf("5. Реальный идентификатор группы владельца (RGID): %d\n", real_gid);
    printf("6. Эффективный идентификатор владельца (EUID): %d\n", effective_uid);
    printf("7. Эффективный идентификатор группы владельца (EGID): %d\n", effective_gid);
    
    printf("\n=== Дополнительная информация ===\n");
    printf("Идентификатор сессии процесса (SID): %d\n", getsid(0));
    
    return 0;
}