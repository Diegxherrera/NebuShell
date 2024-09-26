#ifndef NEBUSHELL_ALIAS_H
#define NEBUSHELL_ALIAS_H

char *find_alias(char *alias_name);
int show_aliases();
int alias(char *alias_name, char *aliased);
int set_up_aliases();
int handle_alias(char *arg1, char *optional_arg2);

#endif //NEBUSHELL_ALIAS_H
