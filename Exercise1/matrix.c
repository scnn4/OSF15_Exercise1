#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>


#include "matrix.h"


#define MAX_CMD_COUNT 50

/*protected functions*/
void load_matrix (Matrix_t* m, unsigned int* data);

/*
 * PURPOSE: instantiates a new matrix with the passed name, rows, cols
 * INPUTS:
 *	name the name of the matrix limited to 50 characters
 *  rows the number of rows the matrix
 *  cols the number of cols the matrix
 * RETURN:
 *  If no errors occurred during instantiation then true
 *  else false for an error in the process.
 *
 **/

bool create_matrix (Matrix_t** new_matrix, const char* name, const unsigned int rows,
						const unsigned int cols) {

	//TODO ERROR CHECK INCOMING PARAMETERS
    if((*new_matrix) != NULL){
		printf("the matrix is not empty!\n");
		return false;
	}

	 if(name== NULL){
		printf("no matrix name detected!\n");
		return false;
	}

	 if( rows<0){
		printf("Invalid row variable!\n");
		return false;
	}

	 if( cols<0){
		printf("Invalid cols variable!\n");
		return false;
	}


	*new_matrix = calloc(1,sizeof(Matrix_t));
	if (!(*new_matrix)) {
		return false;
	}
	(*new_matrix)->data = calloc(rows * cols,sizeof(unsigned int));
	if (!(*new_matrix)->data) {
		return false;
	}
	(*new_matrix)->rows = rows;
	(*new_matrix)->cols = cols;
	unsigned int len = strlen(name) + 1;
	if (len > MATRIX_NAME_LEN) {
		return false;
	}
	strncpy((*new_matrix)->name,name,len);
	return true;

}

	//TODO FUNCTION COMMENT
/*
 * PURPOSE:release the given memory that was taken to store the matrix's data array
 * INPUTS:the pointer of the matrix array
 * RETURN:NULL
 **/
void destroy_matrix (Matrix_t** m) {

    if (*m==NULL){
		printf("Missing source matrix array!\n");
		return;
	}


	//TODO ERROR CHECK INCOMING PARAMETERS

	free((*m)->data);
	free(*m);
	*m = NULL;
}



	//TODO FUNCTION COMMENT
	/*
 * PURPOSE:compare the two matrix by looking into it's data variable and the  number of bytes to compare
 * INPUTS:
 the pointer that points to a mastrix structure
 the pointer that points to another mastrix structure
 * RETURN: Ture if the contents of both memory blocks are equal
 false if they are not equal or any matrix is empty or the any data variable of the structures is empty
 **/
bool equal_matrices (Matrix_t* a, Matrix_t* b) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	//it was done....
	if (!a || !b || !a->data || !b->data) {
		printf("some data was missing please double check/n");
		return false;

	}

	int result = memcmp(a->data,b->data, sizeof(unsigned int) * a->rows * a->cols);
	if (result == 0) {
		return true;
	}
	return false;
}

//TODO FUNCTION COMMENT
/*
 * PURPOSE:copy a matrix from one memory location to another location
 * INPUTS: the pointer of source Matrix structure
 the pointer of the destination Matrix structure
 * RETURN: ture if successed copied the matrix to the new memory location.
 false if the pointer of the sourse matrix is empty.
 **/
bool duplicate_matrix (Matrix_t* src, Matrix_t* dest) {


	//TODO ERROR CHECK INCOMING PARAMETERS

	if (!src || !src-> data) {
		printf("Missing source matrix!");
		return false;
	}
	/*
	 * copy over data
	 */
	unsigned int bytesToCopy = sizeof(unsigned int) * src->rows * src->cols;
	memcpy(dest->data,src->data, bytesToCopy);
	return equal_matrices (src,dest);
}

//TODO FUNCTION COMMENT
/*
 * PURPOSE: operate the bitwise shift to the whole matrix
 * INPUTS:the pointer of the matrix
 the shift direction which is a char variable('L' for left , otherwise all counts as right)
 the unsighned interger number of places(bit) of the shift operation
 * RETURN: ture if successed
 False if the pointer of the matrix is empty.
 **/
bool bitwise_shift_matrix (Matrix_t* a, char direction, unsigned int shift) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if (!a||!a->data|| ! shift) {
		printf("Missing source matrix or shift commands!\n");
		return false;
	}

	if (direction == 'l') {
		unsigned int i = 0;
		for (; i < a->rows; ++i) {
			unsigned int j = 0;
			for (; j < a->cols; ++j) {
				a->data[i * a->cols + j] = a->data[i * a->cols + j] << shift;
			}
		}

	}
	else {
		unsigned int i = 0;
		for (; i < a->rows; ++i) {
			unsigned int j = 0;
			for (; j < a->cols; ++j) {
				a->data[i * a->cols + j] = a->data[i * a->cols + j] >> shift;
			}
		}
	}

	return true;
}

	//TODO FUNCTION COMMENT
/*
 * PURPOSE:combine two matrix into one matrix
 * INPUTS: first sourse matrix pointer
 second sourse matrix pointer
 a pointer of the destination matrix for the combination process
 * RETURN: ture if successed
 false if the size of the two sourse matrix is not equal.
 **/
bool add_matrices (Matrix_t* a, Matrix_t* b, Matrix_t* c) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(!a||!b){
		printf("one of the matrix is not exist! please create it first!\n");
		return false;
	}


	if (a->rows != b->rows && a->cols != b->cols) {
		printf("The two matrix does not have the same size!/n");
		return false;
	}

	for (int i = 0; i < a->rows; ++i) {
		for (int j = 0; j < b->cols; ++j) {
			c->data[i * a->cols +j] = a->data[i * a->cols + j] + b->data[i * a->cols + j];
		}
	}
	return true;
}

//TODO FUNCTION COMMENT
/*
 * PURPOSE: print out the matrix
 * INPUTS: the pointer of the matrix that is ready to be print out
 * RETURN:null
 **/
void display_matrix (Matrix_t* m) {

	//TODO ERROR CHECK INCOMING PARAMETERS
if(!m){
	printf("Matrix can not be displayed, because it does not exist!");
	return;

}

	printf("\nMatrix Contents (%s):\n", m->name);
	printf("DIM = (%u,%u)\n", m->rows, m->cols);
	for (int i = 0; i < m->rows; ++i) {
		for (int j = 0; j < m->cols; ++j) {
			printf("%u ", m->data[i * m->cols + j]);
		}
		printf("\n");
	}
	printf("\n");

}

	//TODO FUNCTION COMMENT
	/*
 * PURPOSE:open a file in readonly mode and load the matrix from the file into the destination matrix
 * INPUTS:
Name of the file which contains a matrix
the pointer of the matrix array
 * RETURN: return ture if  successed load the matrix from the sourse file into the memory location of the destination matrix
 return false for an error in the process.
 **/
bool read_matrix (const char* matrix_input_filename, Matrix_t** m) {

	//TODO ERROR CHECK INCOMING PARAMETERS

	if(! matrix_input_filename){
		Printf("Missing source file name!");
		return false;
	}

	if(! m){
		printf("This matrix does not exist!");
		return false;
	}



	int fd = open(matrix_input_filename,O_RDONLY);
	if (fd < 0) {
		printf("FAILED TO OPEN FOR READING\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}

	/*read the wrote dimensions and name length*/
	unsigned int name_len = 0;
	unsigned int rows = 0;
	unsigned int cols = 0;

	if (read(fd,&name_len,sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READING FILE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}
	char name_buffer[50];
	if (read (fd,name_buffer,sizeof(char) * name_len) != sizeof(char) * name_len) {
		printf("FAILED TO READ MATRIX NAME\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	if (read (fd,&rows, sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READ MATRIX ROW SIZE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	if (read(fd,&cols,sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READ MATRIX COLUMN SIZE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	unsigned int numberOfDataBytes = rows * cols * sizeof(unsigned int);
	unsigned int *data = calloc(rows * cols, sizeof(unsigned int));
	if (read(fd,data,numberOfDataBytes) != numberOfDataBytes) {
		printf("FAILED TO READ MATRIX DATA\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	if (!create_matrix(m,name_buffer,rows,cols)) {
		return false;
	}

	load_matrix(*m,data);
	free(data);
	if (close(fd)) {
		return false;

	}
	return true;
}

	//TODO FUNCTION COMMENT
	/*
 * PURPOSE: write a matrix into a file
 * INPUTS:
 the filename of the destination file
 the pointer that points to the source matrix
 * RETURN: return ture is successed
 return false for an error in the process.
 **/
bool write_matrix (const char* matrix_output_filename, Matrix_t* m) {

	//TODO ERROR CHECK INCOMING PARAMETERS

	if(! matrix_output_filename){
		Printf("Missing dest file name!");
		return false;
	}

	if(! m){
		printf("This matrix does not exist!");
		return false;
	}


	int fd = open (matrix_output_filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	/* ERROR HANDLING USING errorno*/
	if (fd < 0) {
		printf("FAILED TO CREATE/OPEN FILE FOR WRITING\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXISTS\n");
		}
		return false;
	}
	/* Calculate the needed buffer for our matrix */
	unsigned int name_len = strlen(m->name) + 1;
	unsigned int numberOfBytes = sizeof(unsigned int) + (sizeof(unsigned int)  * 2) + name_len + sizeof(unsigned int) * m->rows * m->cols + 1;
	/* Allocate the output_buffer in bytes
	 * IMPORTANT TO UNDERSTAND THIS WAY OF MOVING MEMORY
	 */
	unsigned char* output_buffer = calloc(numberOfBytes,sizeof(unsigned char));
	unsigned int offset = 0;
	memcpy(&output_buffer[offset], &name_len, sizeof(unsigned int)); // IMPORTANT C FUNCTION TO KNOW
	offset += sizeof(unsigned int);
	memcpy(&output_buffer[offset], m->name,name_len);
	offset += name_len;
	memcpy(&output_buffer[offset],&m->rows,sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy(&output_buffer[offset],&m->cols,sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy (&output_buffer[offset],m->data,m->rows * m->cols * sizeof(unsigned int));
	offset += (m->rows * m->cols * sizeof(unsigned int));
	output_buffer[numberOfBytes - 1] = EOF;

	if (write(fd,output_buffer,numberOfBytes) != numberOfBytes) {
		printf("FAILED TO WRITE MATRIX TO FILE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}

	if (close(fd)) {
		return false;
	}
	free(output_buffer);

	return true;
}

	//TODO FUNCTION COMMENT
	/*
 * PURPOSE: generate a matrix with random data in the given maxtix memory location
 * INPUTS: pointer that points to a matrix structure
 the minimon number of the random matrix data variable
 the maximon number of the random matrix data variable
 * RETURN: return ture if no error occurred
 **/
bool random_matrix(Matrix_t* m, unsigned int start_range, unsigned int end_range) {

	//TODO ERROR CHECK INCOMING PARAMETERS

	if(!m){
		printf("Matrix does not exist !\n");
		return false;

	}

	if(start_range<0 || end_range <0 || start_range > end_range){
		printf("please double check the range settings");
		return false;
	}


	for (unsigned int i = 0; i < m->rows; ++i) {
		for (unsigned int j = 0; j < m->cols; ++j) {
			m->data[i * m->cols + j] = rand() % (end_range + 1 - start_range) + start_range;
		}
	}
	return true;
}

/*Protected Functions in C*/

//TODO FUNCTION COMMENT
/*
 * PURPOSE:copy data array from one memory location into a matrix memory area
 * INPUTS:
 the pointer that points to the destination matrix
 the pointer that points to the source data
 * RETURN:Null
 **/
void load_matrix (Matrix_t* m, unsigned int* data) {

	//TODO ERROR CHECK INCOMING PARAMETERS

	if(!m){
		printf("Matrix does not exist!\n");
		return;
	}

	if(!data){
		printf("There is no data to load\n");
		return;
	}

	memcpy(m->data,data,m->rows * m->cols * sizeof(unsigned int));
}

//TODO FUNCTION COMMENT
/*
 * PURPOSE: store the source matrix into the matrix array
 * INPUTS:
   the pointer that points to the pointer of the matrix(the pointer array)
   the pointer that points to the matrix which is going to be added to the matrix array
   the size of the matrix array(which is 10 according to the main)
 * RETURN:
  the current position of the matrix array(shows how many matrix being stored in the matrix array so far)
 **/
unsigned int add_matrix_to_array (Matrix_t** mats, Matrix_t* new_matrix, unsigned int num_mats) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if( !(*mats) ||!new_matrix||0>num_mats|| num_mats >10 )
	{
		printf("Please double check the input variables!");
		return -1;
	}




	static long int current_position = 0;
	const long int pos = current_position % num_mats;
	if ( mats[pos] ) {
		destroy_matrix(&mats[pos]);
	}
	mats[pos] = new_matrix;
	current_position++;
	return pos;
}
