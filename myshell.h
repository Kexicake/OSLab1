#ifndef MYSHELL_H
#define MYSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>

// Максимальная длина строки для ввода
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

// Прототипы функций
int change_directory(char **args, char *input_file, char *output_file, int append);
int clear_screen();
int list_directory(char **args, char *input_file, char *output_file, int append);
int print_environment(char **args, char *input_file, char *output_file, int append);
int print_echo(char **args, char *input_file, char *output_file, int append);
int show_help(char **args, char *input_file, char *output_file, int append);
int pause_execution();
int exit_shell();
int launch_process(char **args, int background, char *input_file, char *output_file, int append);
int execute_command(char **args);
char **split_line(char *line);
char *read_line();
void shell_loop();
int get_num_commands();

extern char *commands[];
extern char *command_descriptions[];
extern int (*command_functions[])(char **, char *, char *, int);
extern char **environ;

#endif // MYSHELL_H
