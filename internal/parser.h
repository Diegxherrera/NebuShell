#ifndef NEBUSHELL_PARSER_H
#define NEBUSHELL_PARSER_H

#define MAX_ARGS 1024
#define MAX_PIPES 1024

typedef struct {
    int option_l;
    int option_a;
    int option_f;
} Options;

int parse_options(int argc, char *argv[], Options *opts, char **directory);
int command_tokenizer(char *command, char *currentDirectory);

#endif // NEBUSHELL_PARSER_H