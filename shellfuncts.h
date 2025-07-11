/*************************************************************************************
 * shellfuncts.h - header file for your shell functions
 *
 *************************************************************************************/

void my_shell();
char *get_line();
char **get_args(char *line);
int execute(char **args);

int create(char **args);
int update(char **args);
int list(char **args);
int dir();
int halt();