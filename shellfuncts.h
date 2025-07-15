/*************************************************************************************
 * shellfuncts.h - header file for your shell functions
 *
 *************************************************************************************/

void my_shell();
char *get_line();
char **get_args(char *line);

void create(char **args);
void update(char **args);
void list(char **args);
void dir();