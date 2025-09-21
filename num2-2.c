#include <stdio.h>
#include <unistd.h>

int main(){
    fork();
    printf("Hi 1 \n");
    fork();
    printf("Hi 2\n");
    return 0;

}