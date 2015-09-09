#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#include<readline/readline.h>

#include "command.h"
#include "matrix.h"

void run_commands (Commands_t* cmd, Matrix_t** mats, unsigned int num_mats);
unsigned int find_matrix_given_name (Matrix_t** mats, unsigned int num_mats, 
			const char* target);

// TODO complete the defintion of this function. 
void destroy_remaining_heap_allocations(Matrix_t **mats, unsigned int num_mats);

//TODO FUNCTION COMMENT
/*
 * PURPOSE: instantiates the whole program, generate the matrix array with the first element in the array as a ramdon matrix with the size of 5X5 and value between 10 to 15
the program also allocate the memory space for the matrix array, while it will release the memory that it took for running when the user type "exit"
the program reads users' command and calls the corresponding functions 
 * INPUTS: 
 * argc :argument count
 * argv:argument vector
 * RETURN: -1 if the program failed to instantiate the matrix array
 return 0 if the program is excuted 
 **/
int main (int argc, char **argv) {
	srand(time(NULL));		
	char *line = NULL;
	Commands_t* cmd;

	Matrix_t *mats[10];
	memset(&mats,0, sizeof(Matrix_t*) * 10); // IMPORTANT C FUNCTION TO LEARN

	Matrix_t *temp = NULL;
	create_matrix (&temp,"temp_mat", 5, 5); // TODO ERROR CHECK
	add_matrix_to_array(mats,temp, 10); //TODO ERROR CHECK NEEDED
	int mat_idx = find_matrix_given_name(mats,10,"temp_mat");

	if (mat_idx < 0) {
		perror("PROGRAM FAILED TO INIT\n");
		return -1;
	}
	random_matrix(mats[mat_idx], 10, 15);
	write_matrix("temp_mat", mats[mat_idx]); // TODO ERROR CHECK

	line = readline("> ");
	while (strncmp(line,"exit", strlen("exit")  + 1) != 0) {
		
		if (!parse_user_input(line,&cmd)) {
			printf("Failed at parsing command\n\n");
		}
		
		if (cmd->num_cmds > 1) {	
			run_commands(cmd,mats,10);
		}
		if (line) {
			free(line);
		}
		destroy_commands(&cmd);
		line = readline("> ");
	}
	free(line);
	destroy_remaining_heap_allocations(mats,10);
	return 0;	
}

	//TODO FUNCTION COMMENT
		/*
 * PURPOSE: read the command from the user and call the corresponding functions to do the job.
 * INPUTS:
 *cmd: the point of the command structure(Commands array)
 *mats:the matrix array 
 *num_mats: the size of the matrix array
 * RETURN: NULL
 **/
void run_commands (Commands_t* cmd, Matrix_t** mats, unsigned int num_mats) {
	//TODO ERROR CHECK INCOMING PARAMETERS
if (!cmd){
	printf("No commands exist\n!");
	return;
}
if (!mats||!(*mats)){
	printf("No matrix exist\n!");
    return; 
}

if (num_mats<0){
	printf("Please double check the number of the matrics\n");
	return;
}


	/*Parsing and calling of commands*/
	if (strncmp(cmd->cmds[0],"display",strlen("display") + 1) == 0
		&& cmd->num_cmds == 2) {
			/*find the requested matrix*/
			int idx = find_matrix_given_name(mats,num_mats,cmd->cmds[1]);
			if (idx >= 0) {
				display_matrix (mats[idx]);
			}
			else {
				printf("Matrix (%s) doesn't exist\n", cmd->cmds[1]);
				return;
			}
	}
	else if (strncmp(cmd->cmds[0],"add",strlen("add") + 1) == 0
		&& cmd->num_cmds == 4) {
			int mat1_idx = find_matrix_given_name(mats,num_mats,cmd->cmds[1]);
			int mat2_idx = find_matrix_given_name(mats,num_mats,cmd->cmds[2]);
			if (mat1_idx >= 0 && mat2_idx >= 0) {
				Matrix_t* c = NULL;
				if( !create_matrix (&c,cmd->cmds[3], mats[mat1_idx]->rows, 
						mats[mat1_idx]->cols)) {
					printf("Failure to create the result Matrix (%s)\n", cmd->cmds[3]);
					return;
				}
			
				add_matrix_to_array(mats,c, num_mats); //TODO ERROR CHECK NEEDED


				if (! add_matrices(mats[mat1_idx], mats[mat2_idx],c) ) {
					printf("Failure to add %s with %s into %s\n", mats[mat1_idx]->name, mats[mat2_idx]->name,c->name);
					return;	
				}
			}
	}
	else if (strncmp(cmd->cmds[0],"duplicate",strlen("duplicate") + 1) == 0
		&& cmd->num_cmds == 3 && strlen(cmd->cmds[1]) + 1 <= MATRIX_NAME_LEN) {
		int mat1_idx = find_matrix_given_name(mats,num_mats,cmd->cmds[1]);
		if (mat1_idx >= 0 ) {
				Matrix_t* dup_mat = NULL;
				if( !create_matrix (&dup_mat,cmd->cmds[2], mats[mat1_idx]->rows, 
						mats[mat1_idx]->cols)) {
					return;
				}
				duplicate_matrix (mats[mat1_idx], dup_mat); //TODO ERROR CHECK NEEDED
				add_matrix_to_array(mats,dup_mat,num_mats); //TODO ERROR CHECK NEEDED
				printf ("Duplication of %s into %s finished\n", mats[mat1_idx]->name, cmd->cmds[2]);
		}
		else {
			printf("Duplication Failed\n");
			return;
		}
	}
	else if (strncmp(cmd->cmds[0],"equal",strlen("equal") + 1) == 0
		&& cmd->num_cmds == 2) {
			int mat1_idx = find_matrix_given_name(mats,num_mats,cmd->cmds[1]);
			int mat2_idx = find_matrix_given_name(mats,num_mats,cmd->cmds[2]);
			if (mat1_idx >= 0 && mat2_idx >= 0) {
				if ( equal_matrices(mats[mat1_idx],mats[mat2_idx]) ) {
					printf("SAME DATA IN BOTH\n");
				}
				else {
					printf("DIFFERENT DATA IN BOTH\n");
				}
			}
			else {
				printf("Equal Failed\n");
				return;
			}
	}
	else if (strncmp(cmd->cmds[0],"shift",strlen("shift") + 1) == 0
		&& cmd->num_cmds == 4) {
		int mat1_idx = find_matrix_given_name(mats,num_mats,cmd->cmds[1]);
		const int shift_value = atoi(cmd->cmds[3]);
		if (mat1_idx >= 0 ) {
			bitwise_shift_matrix(mats[mat1_idx],cmd->cmds[2][0], shift_value); //TODO ERROR CHECK NEEDED
			printf("Matrix (%s) has been shifted by %d\n", mats[mat1_idx]->name, shift_value);
		}
		else {
			printf("Matrix shift failed\n");
			return;
		}

	}
	else if (strncmp(cmd->cmds[0],"read",strlen("read") + 1) == 0
		&& cmd->num_cmds == 2) {
		Matrix_t* new_matrix = NULL;
		if(! read_matrix(cmd->cmds[1],&new_matrix)) {
			printf("Read Failed\n");
			return;
		}	
		
		add_matrix_to_array(mats,new_matrix, num_mats); //TODO ERROR CHECK NEEDED
		printf("Matrix (%s) is read from the filesystem\n", cmd->cmds[1]);	
	}
	else if (strncmp(cmd->cmds[0],"write",strlen("write") + 1) == 0
		&& cmd->num_cmds == 2) {
		int mat1_idx = find_matrix_given_name(mats,num_mats,cmd->cmds[1]);
		if(! write_matrix(mats[mat1_idx]->name,mats[mat1_idx])) {
			printf("Write Failed\n");
			return;
		}
		else {
			printf("Matrix (%s) is wrote out to the filesystem\n", mats[mat1_idx]->name);
		}
	}
	else if (strncmp(cmd->cmds[0], "create", strlen("create") + 1) == 0
		&& strlen(cmd->cmds[1]) + 1 <= MATRIX_NAME_LEN && cmd->num_cmds == 4) {
		Matrix_t* new_mat = NULL;
		const unsigned int rows = atoi(cmd->cmds[2]);
		const unsigned int cols = atoi(cmd->cmds[3]);

		create_matrix(&new_mat,cmd->cmds[1],rows, cols); //TODO ERROR CHECK NEEDED
		add_matrix_to_array(mats,new_mat,num_mats); // TODO ERROR CHECK NEEDED
		printf("Created Matrix (%s,%u,%u)\n", new_mat->name, new_mat->rows, new_mat->cols);
	}
	else if (strncmp(cmd->cmds[0], "random", strlen("random") + 1) == 0
		&& cmd->num_cmds == 4) {
		int mat1_idx = find_matrix_given_name(mats,num_mats,cmd->cmds[1]);
		const unsigned int start_range = atoi(cmd->cmds[2]);
		const unsigned int end_range = atoi(cmd->cmds[3]);
		random_matrix(mats[mat1_idx],start_range, end_range); //TODO ERROR CHECK NEEDED

		printf("Matrix (%s) is randomized between %u %u\n", mats[mat1_idx]->name, start_range, end_range);
	}
	else {
		printf("Not a command in this application\n");
	}

}

	//TODO FUNCTION COMMENT
/*
 * PURPOSE:find the the index of a matrix which was stored in the matrix array by matching its name
 * INPUTS:
 * mats: the  matrix array
 * num_mats: the size of the matrixarray
 * targer ： the name of the targeted matrix  
 * RETURN:
 * If find match return the index of the targeted matrix
 * If did not find any matrix with the given name, return -1 
 **/
unsigned int find_matrix_given_name (Matrix_t** mats, unsigned int num_mats, const char* target) {
	//TODO ERROR CHECK INCOMING PARAMETERS
if (!mats||!(*mats)){
	printf("No matrix exist\n!");
    return; 
}

if (num_mats<0){
	printf("Please double check the number of the matrics\n");
	return;
}

if (!target){
	printf("The matrix name is missing!\n");
	return;
	
}
	for (int i = 0; i < num_mats; ++i) {
		if (strncmp(mats[i]->name,target,strlen(mats[i]->name)) == 0) {
			return i;
		}
	}
	return -1;
}

	//TODO FUNCTION COMMENT
		/*
 * PURPOSE:relase the memory location that was taken to store the matrix
 * INPUTS:
 * mats: the matrix array
 * num_mats: the size of the Matrix array
 * RETURN:NULL
 **/
void destroy_remaining_heap_allocations(Matrix_t** mats, unsigned int num_mats) {
	//TODO ERROR CHECK INCOMING PARAMETERS
if(!mats||!(*mats)){
	printf("Can not locate the matrix array!\n");
	return;
}
	
	
	// COMPLETE MISSING MEMORY CLEARING HERE
	
	for (int i = 0; i < num_cmds; ++i) {
		free((*mats)->name);
		free((*mates)->rows);
		free((*mates)->cols);
		free((*mates)->data);
	}
	free(*mates);
	free(num_mats)
	*mates = NULL;
	
}
