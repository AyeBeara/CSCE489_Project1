/*************************************************************************************
 * myshell - student code for Project 1 of CSCE 489 
 *
 *************************************************************************************/

#include <stdio.h>
#include "shellfuncts.h"

int main(int argv, const char *argc[]) {
//	(void) argv; // Make compile warnings go away - be sure to delete this line if you use the param
	(void) argc; // Make compile warnings go away - be sure to delete this line if you use the param

	// If they just ran myshell, say Hello World--if they included a parameter, speak Australian
	if (argv == 1)
		hello(1);
	else
		hello(0);

}
