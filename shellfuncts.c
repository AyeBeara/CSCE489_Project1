/*************************************************************************************
 * shellfuncts.c - code definitions for your functions
 *
 *************************************************************************************/

#include <dirent.h>
#include <sys/types.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>
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

	int pid = getpid();

	// Always display this message when the shell is opened, then block for input
	printf("Hello there! Welcome to my shell. PID:%d\n", pid);
	do {
		printf("Enter command: ");
		
		line = get_line();
		args = get_args(line);
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
			fprintf(stderr, "Error reading stdin: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	return line;
}

#define TOK_BUFSIZE 64
#define DELIMS " \t\r\n\a"
char **get_args(char *line) {

	int bufsize = TOK_BUFSIZE, position = 0;
	char **tokens = malloc(sizeof(char*) * bufsize);
	char *token;

	if (!tokens) { // error allocating memory
		fprintf(stderr, "Error allocating memory for token array");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, DELIMS);
	while (token != NULL) {

		// Handle quoted strings
		if (token[0] == '\"' || token[0] == '\'') {
			char *quoted_string = malloc(256); // allocate buffer for quoted string
			if (!quoted_string) {
				fprintf(stderr, "Error allocating memory for quoted string");
				free(tokens);
				free(token);
				exit(EXIT_FAILURE);
			}
			
			strcpy(quoted_string, token + 1); // copy without the leading quote
			
			char *next_token;
			while ((next_token = strtok(NULL, DELIMS)) != NULL) {
				if (next_token[strlen(next_token) - 1] == '\"') {
					// Found closing quote
					next_token[strlen(next_token) - 1] = '\0'; // remove trailing quote
					strcat(quoted_string, " ");
					strcat(quoted_string, next_token);
					break;
				} else {
					// Continue collecting tokens
					strcat(quoted_string, " ");
					strcat(quoted_string, next_token);
				}
			}

			if (next_token == NULL) {
					fprintf(stderr, "Error: unmatched quote in input");
					free(quoted_string);
					free(tokens);
					exit(EXIT_FAILURE);
				}
			
			tokens[position] = quoted_string;
		} else {
			// Regular token without quotes
			tokens[position] = token;
		}
		position++;

		if (position >= bufsize) { // increase buffer size if needed
			bufsize += TOK_BUFSIZE;
			tokens = realloc(tokens, sizeof(char*) * bufsize);
			if (!tokens) {
				fprintf(stderr, "Error reallocating memory for token array");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, DELIMS);
	}
	tokens[position] = NULL; // null-terminate the array of tokens

	return tokens;
}

int execute(char **args) {
	char *valid_commands[] = {"create", "update", "list", "dir", "halt"};
	
	for (size_t i = 0; i < sizeof(valid_commands) / sizeof(valid_commands[0]); i++) {
		if (args[0] != NULL && strcmp(args[0], valid_commands[i]) == 0) {
			switch (i) {
				case 0: return create(args);
				case 1: return update(args);
				case 2: return list(args);
				case 3: return dir();
				case 4: return halt();
			}
		}
	}

	return 1;
}

int create(char **args) {
	if (args[1] == NULL) {
		fprintf(stderr, "Error: No filename provided for create command\n");
		return 1;
	}

	FILE *fptr;

	fptr = fopen(args[1], "r");
	if (fptr != NULL) {
		fprintf(stderr, "Error: File '%s' already exists\n", args[1]);
		fclose(fptr);
		return 1;
	}
	fptr = fopen(args[1], "w");
	if (fptr == NULL) {
		fprintf(stderr, "Error: Could not create file '%s': %s\n", args[1], strerror(errno));
		return 1;
	}

	fclose(fptr);
	return 1;
}

int update(char **args) {
	if (args[1] == NULL || args[2] == NULL || args[3] == NULL) {
		fprintf(stderr, "Error: Insufficient arguments for update command\n");
		return 1;
	}

	FILE *fptr = fopen(args[1], "a");

	if (fptr == NULL) {
		fprintf(stderr, "Error: Could not open file '%s' for updating: %s\n", args[1], strerror(errno));
		return 1;
	}

	int write_counter = atoi(args[2]);

	if (write_counter <= 0) {
		fprintf(stderr, "Error: Invalid write count '%s'\n", args[2]);
		fclose(fptr);
		return 1;
	}

	for (int writes = 0; writes < write_counter; writes++) {
		if (fprintf(fptr, "%s\n", args[3]) < 0) {
			fprintf(stderr, "Error: Could not write to file '%s': %s\n", args[1], strerror(errno));
			fclose(fptr);
			sleep(3);
			return 1;
		}
	}
	
	fclose(fptr);
	return 1;
}

int list(char **args) {
	if (args[1] == NULL) {
		fprintf(stderr, "Error: No file provided for list command\n");
		return 1;
	}

	FILE *fptr = fopen(args[1], "r");
	if (fptr == NULL) {
		fprintf(stderr, "Error: Could not open file '%s' for listing: %s", args[1], strerror(errno));
		return 1;
	}

	char *line = NULL;
	size_t line_size = 0; // let getline() allocate the buffer
	ssize_t num_chars = 0;

	while (num_chars != -1) {
		num_chars = getline(&line, &line_size, fptr);
		if (num_chars == -1) {
			if (feof(fptr)) {
				break; // End of file reached
			} else {
				fprintf(stderr, "Error reading file '%s': %s\n", args[1], strerror(errno));
				free(line);
				fclose(fptr);
				return 1;
			}
		}
		printf("%s", line); // Print the line read from the file
	}

	return 1;
}

int dir() {
	
	if( execl("/bin/ls", "ls", NULL) == -1) {
		fprintf(stderr, "Error getting directory contents: %s\n", strerror(errno));
		return 1;
	};
	return 1;
}

int halt() {
	return 0; // Return 0 to indicate the shell should exit
}