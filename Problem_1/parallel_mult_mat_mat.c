#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <omp.h>

#define DEBUG 0

/* ----------- Project 2 - Problem 1 - Matrix Mult -----------

    This file will multiply two matricies.
    Complete the TODOs in order to complete this program.
    Remember to make it parallelized!
*/ // ------------------------------------------------------ //

int main(int argc, char* argv[])
{
    // Catch console errors
    if (argc != 10)
    {
        printf("USE LIKE THIS: parallel_mult_mat_mat   mat_1.csv n_row_1 n_col_1   mat_2.csv n_row_2 n_col_2   num_threads   results_matrix.csv   time.csv\n");
        return EXIT_FAILURE;
    }

    // Get the input files
    FILE* inputMatrix1 = fopen(argv[1], "r");
    FILE* inputMatrix2 = fopen(argv[4], "r");

    char* p1;
    char* p2;

    // Get matrix 1's dims
    int n_row1 = strtol(argv[2], &p1, 10);
    int n_col1 = strtol(argv[3], &p2, 10);

    // Get matrix 2's dims
    int n_row2 = strtol(argv[5], &p1, 10);
    int n_col2 = strtol(argv[6], &p2, 10);


    // Get num threads
    int thread_count = strtol(argv[7], NULL, 10);

    // Get output files
    FILE* outputFile = fopen(argv[8], "w");
    FILE* outputTime = fopen(argv[9], "w");


    // TODO: malloc the two input matrices and the output matrix
    // Please use long int as the variable type
	long int* mat1 = malloc(n_col1* n_row1* sizeof(long int));
	long int* mat2 = malloc(n_col2 * n_row2 * sizeof(long int));
	long int* omat = malloc(n_row1 * n_col2 * sizeof(long int));
	printf("Allocation Finished\n");
    // TODO: Parse the input csv files and fill in the input matrices
	long int temp = 0;
	for(int i = 0; i < n_row1*n_col1; ++i) {
		fscanf(inputMatrix1, "%li, ", &temp);
		(mat1)[i] = temp;
	}
	temp = 0;
	for(int i = 0; i < n_row2*n_col2; ++i) {
                fscanf(inputMatrix2, "%li, ", &temp);
                //printf("Element: %li, Idx: %i\n", temp, i);
		(mat2)[i] = temp;
        }
	//printf("\n");
	printf("Reading Finished\n");
    // We are interesting in timing the matrix-matrix multiplication only
    // Record the start time
    double start = omp_get_wtime();
    
    // TODO: Parallelize the matrix-matrix multiplication
# pragma omp parallel for num_threads(8)
	for(int i = 0; i < n_row1; i++) {
		for(int j = 0; j < n_col2; j++) {
			int sum = 0;
			for(int k = 0; k < n_col1; k++) {
				int a = mat1[i * n_col1 + k];
				int b = mat2[k * n_col2 + j];
				sum = sum + a * b;
			}
			omat[i * n_col2 + j]=sum;
		}
	}
	printf("Multiplication finished\n");
    // Record the finish time
    double end = omp_get_wtime();

    // Time calculation (in seconds)
    double time_passed = end - start;

    // Save time to file
    fprintf(outputTime, "%f", time_passed);

    // TODO: save the output matrix to the output csv file
	for(int i = 0; i < n_row1 * n_col2; i++) {
		//printf("i: %d\n", i%n_col2<n_col2-1);
		if(i % n_col2 == n_col2-1){
			fprintf(outputFile, "%li\n", omat[i]);
		}
		else{
			fprintf(outputFile, "%li, ", omat[i]);
		}
	}

	printf("File output finished\n");
    // Cleanup
    fclose(inputMatrix1);
    fclose(inputMatrix2);
    fclose(outputFile);
    fclose(outputTime);
    // Remember to free your buffers!
    free(mat1);
    free(mat2);
    free(omat);

    return 0;
}

