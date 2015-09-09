#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "command.h"

#define MAX_CMD_COUNT 50
#define MAX_CMD_LEN 25


//TODO FUNCTION COMMENT

/*
 * PURPOSE:
   store the user input information into the memory location under the commands_t sturcture.
 * INPUTS:
   the pointer that points to the character array which store the user input
   the address of the pointer which points to a commands_t sturcture
 * RETURN:
   if success reture ture else
	   return false for an error in the process.
 **/
bool parse_user_input (const char* input, Commands_t** cmd){

	//TODO ERROR CHECK INCOMING PARAMETERS

	if (! input ){
		printf("no input detected!\n");
		return false;
	}

	if(! cmd){
		printf("No commend array detected!\n");
		return false;
	}



	char *string = strdup(input);

	*cmd = calloc (1,sizeof(Commands_t));
	(*cmd)->cmds = calloc(MAX_CMD_COUNT,sizeof(char*));

	unsigned int i = 0;
	char *token;
	token = strtok(string, " \n");
	for (; token != NULL && i < MAX_CMD_COUNT; ++i) {
		(*cmd)->cmds[i] = calloc(MAX_CMD_LEN,sizeof(char));
		if (!(*cmd)->cmds[i]) {
			perror("Allocation Error\n");
			return false;
		}
		strncpy((*cmd)->cmds[i],token, strlen(token) + 1);
		(*cmd)->num_cmds++;
		token = strtok(NULL, " \n");
	}
	free(string);
	return true;
}

	//TODO FUNCTION COMMENT

/*
 * PURPOSE: release the memory that was used to store the commands_t structure and the pointers that associated with it.
 * INPUTS: the the address of the pointer which points to a commands_t sturcture
 * RETURN: null
 **/

void destroy_commands(Commands_t** cmd) {

	//TODO ERROR CHECK INCOMING PARAMETERS
		if(! cmd){
		printf("No commend array detected!\n");
		return;
	}


	for (int i = 0; i < (*cmd)->num_cmds; ++i) {
		free((*cmd)->cmds[i]);
	}
	free((*cmd)->cmds);
	free((*cmd));
	*cmd = NULL;
}

