#include <stdio.h> // perror , fgets
#include <stdlib.h> // waitpid
#include <string.h> // strcspn, strtok, strcmp
#include <unistd.h> // системные вызовы: getcwd,fork,execvp,chdir

// Объявление переменных среды
extern char **environ;

// Функция для вывода приглашения
void print_prompt() {
    char cwd[1024];
    // Получаем текущий рабочий каталог и выводим его как приглашение
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s> ", cwd);
    } else {
        perror("getcwd() error"); // Обрабатываем ошибку, если не удалось получить текущий каталог
    }
}

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

// Функция для обработки команды clr
void execute_clr() {
    // Очищаем экран
    printf("\033[H\033[J");
}
int main(int argc, char *argv[]){
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *args[100];
    // Установить переменную среды shell
    setenv("shell", argv[0], 1);

    while (1) {
        print_prompt(); // Выводим приглашение

        read = getline(&line, &len, stdin); // Считываем строку ввода
        if (read == -1) {
            break;
        }
        // Убираем символ новой строки
        line[strcspn(line, "\n")] = 0;

        // Разделяем строку на аргументы
        int i = 0;
        args[i] = strtok(line, " ");
        while (args[i] != NULL) {
            args[++i] = strtok(NULL, " ");
        }
    }
}