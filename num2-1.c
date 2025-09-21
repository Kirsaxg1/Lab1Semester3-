#include <stdio.h>
#include <unistd.h>

/*fork() вызывается до того как printf начнет выводить текст*/

int main() {
    int pid;
    pid = fork(); /*создает два процесса */
    
    printf("%d\n", pid);
    printf("tralaleylotralala - %d\n", fork()); /*создает еще два*/
    return 0;
}