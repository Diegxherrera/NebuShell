//
// Created by Diego Herrera Redondo on 8/4/24.
//

#ifndef NEBUSHELL_FILE_HANDLER_H
#define NEBUSHELL_FILE_HANDLER_H

int open_file(char *filename, const char *mode, char *optional_input);
char* expand_tilde(const char* filename);

#endif //NEBUSHELL_FILE_HANDLER_H
