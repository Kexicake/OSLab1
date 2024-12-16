#include "myshell.h"

// Массив команд оболочки
char *commands[] = {
    "cd",      // смена директории
    "clr",     // очистка экрана
    "dir",     // просмотр содержимого директории
    "environ", // вывод всех переменных окружения
    "echo",    // вывод текста
    "help",    // вывод справки
    "pause",   // пауза, ожидание нажатия Enter
    "quit"     // выход из оболочки
};

// Массив описаний команд
char *command_descriptions[] = {
    "cd <directory> - change the current default directory to <directory>. If the <directory> argument is missing, print the current directory.",
    "clr - clearing the screen.",
    "dir <directory> - displays the contents of the <directory> directory.",
    "environ - output of all environment variables.",
    "echo <comment> - output to the <comment> screen, after which the transition to a new line is performed.",
    "help - output of the user's guide.",
    "pause - suspends shell operations until the <Enter> key is pressed.",
    "quit - exit the shell."
};

// Массив указателей на функции, соответствующие встроенным командам
int (*command_functions[])(char **, char *, char *, int) = {
    &change_directory,
    &clear_screen,
    &list_directory,
    &print_environment,
    &print_echo,
    &show_help,
    &pause_execution,
    &exit_shell
};

// Функция для получения количества встроенных команд
int get_num_commands() {
    return sizeof(commands) / sizeof(char *);
}

// Функция для смены директории
int change_directory(char **args, char *input_file, char *output_file, int append) {
    if (args[1] == NULL) {
        char *current_dir = getcwd(NULL, 0);
        if (current_dir) {
            printf("Current directory: %s\n", current_dir);
            free(current_dir);
        } else {
            perror("Error");
        }
    } else {
        if (chdir(args[1]) != 0) {
            perror("Error");
        } else {
            setenv("PWD", args[1], 1);
        }
    }
    return 1;
}

// Функция для очистки экрана
int clear_screen() {
    printf("\033c"); // команда для очистки экрана терминала
    return 1;
}

// Функция для вывода содержимого директории
int list_directory(char **args, char *input_file, char *output_file, int append) {
    if (args[1] == NULL) {
        fprintf(stderr, "Usage: dir <directory>\n");
    } else {
        DIR *dir = opendir(args[1]);
        if (dir == NULL) {
            perror("Error opening directory");
        } else {
            FILE *output_stream = stdout;
            if (output_file) {
                output_stream = (append) ? fopen(output_file, "a") : fopen(output_file, "w");
            }

            struct dirent *entry;
            while ((entry = readdir(dir)) != NULL) {
                fprintf(output_stream, "%s\n", entry->d_name);
            }

            closedir(dir);
            if (output_file) fclose(output_stream);
        }
    }
    return 1;
}

// Функция для вывода всех переменных окружения
int print_environment(char **args, char *input_file, char *output_file, int append) {
    FILE *output_stream = stdout;
    if (output_file) {
        output_stream = (append) ? fopen(output_file, "a") : fopen(output_file, "w");
    }

    char **env = environ;
    while (*env != NULL) {
        fprintf(output_stream, "%s\n", *env);
        env++;
    }

    if (output_file) fclose(output_stream);
    return 1;
}

// Функция для вывода строки на экран
int print_echo(char **args, char *input_file, char *output_file, int append) {
    FILE *output_stream = stdout;
    if (output_file) {
        output_stream = (append) ? fopen(output_file, "a") : fopen(output_file, "w");
    }

    if (args[1] == NULL) {
        fprintf(output_stream, "\n");
    } else {
        for (int i = 1; args[i] != NULL; i++) {
            fprintf(output_stream, "%s", args[i]);
            if (args[i + 1] != NULL) {
                fprintf(output_stream, " ");
            }
        }
        fprintf(output_stream, "\n");
    }

    if (output_file) fclose(output_stream);
    return 1;
}

// Функция для вывода справки
int show_help(char **args, char *input_file, char *output_file, int append) {
    FILE *output_stream = stdout;
    if (output_file) {
        output_stream = (append) ? fopen(output_file, "a") : fopen(output_file, "w");
    }

    fprintf(output_stream, "List of available commands:\n");
    for (int i = 0; i < get_num_commands(); i++) {
        fprintf(output_stream, " %s\n", command_descriptions[i]);
    }

    if (output_file) fclose(output_stream);
    return 1;
}

// Функция для паузы выполнения
int pause_execution() {
    printf("Press <Enter> to continue...");
    while (getchar() != '\n');
    return 1;
}

// Функция для выхода из оболочки
int exit_shell() {
    return 0;
}

// Функция для запуска внешней программы
int launch_process(char **args, int background, char *input_file, char *output_file, int append) {
    pid_t pid, wpid;
    int status;
    pid = fork();

    if (pid == 0) { // дочерний процесс
        if (input_file) {
            freopen(input_file, "r", stdin);
        }
        if (output_file) {
            if (append) {
                freopen(output_file, "a", stdout);
            } else {
                freopen(output_file, "w", stdout);
            }
        }

        if (execvp(args[0], args) == -1) {
            perror("Error");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("Error");
    } else {
        if (!background) {
            do {
                wpid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }
    return 1;
}

// Функция для выполнения команд
int execute_command(char **args) {
    if (args[0] == NULL) {
        return 1;
    }

    char *input_file = NULL;
    char *output_file = NULL;
    int append = 0;

    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "<") == 0) {
            input_file = args[i + 1];
            args[i] = NULL;
        } else if (strcmp(args[i], ">") == 0) {
            output_file = args[i + 1];
            args[i] = NULL;
        } else if (strcmp(args[i], ">>") == 0) {
            output_file = args[i + 1];
            args[i] = NULL;
            append = 1;
        }
    }

    int background = 0;
    int last_arg_index = -1;
    for (int i = 0; args[i] != NULL; i++) {
        last_arg_index = i;
    }
    if (last_arg_index >= 0 && strcmp(args[last_arg_index], "&") == 0) {
        background = 1;
        args[last_arg_index] = NULL;
    }

    for (int i = 0; i < get_num_commands(); i++) {
        if (strcmp(args[0], commands[i]) == 0) {
            return (*command_functions[i])(args, input_file, output_file, append);
        }
    }

    return launch_process(args, background, input_file, output_file, append);
}

// Функция для разбиения строки на аргументы
char **split_line(char *line) {
    int bufsize = LSH_TOK_BUFSIZE;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "Memory allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

// Функция для чтения строки
char *read_line() {
    char *line = NULL;
    ssize_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

// Функция для выполнения оболочки
void shell_loop() {
    char *line;
    char **args;
    int status;
    char path[256];

    do {
        getcwd(path, 256);
        printf("(Shell) [%s] $ ", path);
        line = read_line();
        args = split_line(line);
        status = execute_command(args);

        free(line);
        free(args);
    } while (status);
}

int main() {
    shell_loop();
    return 0;
}
