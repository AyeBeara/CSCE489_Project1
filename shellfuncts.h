/*************************************************************************************
 * shellfuncts.h - header file for your shell functions
 *
 *************************************************************************************/

void my_shell();
char *get_line();
char **parse_line(char *line);
int execute(char **args);

int create(char **args);
int update(char **args);
int list(char **args);
int dir(char **args);
int halt(char **args);