#ifndef NEBUSHELL_NEBULA_TOOLS_H
#define NEBUSHELL_NEBULA_TOOLS_H
#define MAX_DIRECTORY_LENGTH 1024
#define MAX_ARGS_LENGTH 1024
#define MIN_PATH_LENGTH 256

int change_directory(char *path, char *currentDirectory); // Error Codes: 10XX
int list_directory(char *currentDirectory, const char *args); // Error Codes: 20XX
int print_working_directory(); // Error Codes: 30XX
int who_am_i(); // Error Codes: 40XX
int hostname(); // Error Codes: 50XX
int echo(char *args); // Error Codes: 60XX
void clear(); // Error Codes: 70XX
int help_page(char *option, char *page); // Error Codes: 80XX
int close_shell(); // Error Codes: 90XX
int environment_variables(const char *args); // Error Codes: 100XX
int where(const char *args); // Error Codes 110XX

#endif //NEBUSHELL_NEBULA_TOOLS_H
