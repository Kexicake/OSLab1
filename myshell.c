#include <stdio.h> // perror , fgets
#include <stdlib.h> // waitpid
#include <string.h> // strcspn, strtok, strcmp
#include <unistd.h> // системные вызовы: getcwd,fork,execvp,chdir

// Функция для обработки команды help
void execute_help() {
    printf("myshell - Простая реализация оболочки.\n");
    printf("Поддерживаемые команды:\n");
    printf("cd <directory> - Сменить каталог на <directory>.\n");
    printf("clr - Очистить экран.\n");
    printf("dir <directory> - Вывести содержимое каталога <directory>.\n");
    printf("environ - Вывести все переменные среды.\n");
    printf("echo <comment> - Вывести <comment> на экран.\n");
    printf("help - Показать это сообщение с подсказками.\n");
    printf("pause - Приостановить работу оболочки до нажатия Enter.\n");
    printf("quit - Выйти из оболочки.\n");
}

int main(int argc, char *argv[]){

    // Установить переменную среды shell
    setenv("shell", argv[0], 1);
}