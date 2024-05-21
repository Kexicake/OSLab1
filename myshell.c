#include <stdio.h> // perror , fgets
#include <stdlib.h> // waitpid
#include <string.h> // strcspn, strtok, strcmp
#include <unistd.h> // системные вызовы: getcwd,fork,execvp,chdir

// Функция для обработки команды help
void execute_help() {
    printf("myshell - A simple shell implementation.\n");
    printf("Supported commands:\n");
    printf("cd <directory> - Change directory to <directory>.\n");
    printf("clr - Clear the screen.\n");
    printf("dir <directory> - List the contents of <directory>.\n");
    printf("environ - List all environment variables.\n");
    printf("echo <comment> - Print <comment> to the screen.\n");
    printf("help - Display this help message.\n");
    printf("pause - Pause the shell until Enter is pressed.\n");
    printf("quit - Exit the shell.\n");
}

int main(int argc, char *argv[]){

    // Установить переменную среды shell
    setenv("shell", argv[0], 1);
}