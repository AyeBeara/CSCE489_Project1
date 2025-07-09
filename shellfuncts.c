/*************************************************************************************
 * shellfuncts.c - code definitions for your functions
 *
 *************************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "shellfuncts.h"

/*************************************************************************************
 * my_shell - process loop for my shell
 *
 *
 *************************************************************************************/

void my_shell() {
	char *line; 
	char **args;
	int status;

	// Always display this message when the shell is opened, then block for input
	char welcome[] = "Hello there! Welcome to my shell";
	printf("%s", welcome);
	do {
		printf("# ");
		
		line = get_line();
		args = parse_line();
		status = execute(args);

	}
	while (status);

}

/*************************************************************************************
 * get_line - get single line inut for shell
 *
 *
 *************************************************************************************/

char *get_line() {
	char *line = NULL;
	size_t bufsize = 0; // let getline() allocate the buffer
	ssize_t num_chars;

	num_chars = getline(&line, &bufsize, stdin);

	if (num_chars == -1) {
		if (feof(stdin)) {
			exit(EXIT_SUCCESS);
		} else {
			perror("Error reading stdin");
			exit(EXIT_FAILURE);
		}
	}

	return line;
}

#define TOK_BUFSIZE 64
#define DELIMS " \t\r\n\a"
char **parse_line(char *line) {

	int bufsize = TOK_BUFSIZE, position = 0;
	char **tokens = malloc(sizeof(char*) * bufsize);
	char *token;

	if (!tokens) { // error allocating memory
		fprintf(stderr, "Error allocating memory for token array");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, DELIMS);
}

int execute(char **args) {
	(void) args;
	return 0;
}