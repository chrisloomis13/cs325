/* Project 3
    Christopher Loomis
    Benjamin Tullis
    Dalena Pham
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h> /* memset */
#include <time.h>
#include <math.h>

int** buildArrays(FILE* input, int* lineCount) {
    char str1;
    // Look into a way to not have to hard-code this?
    int** biggerArray = (int**)malloc(sizeof(int) * 20000);
    int i = 0;
    while(!feof(input)) {

        // Extract Array
        int* array = malloc(sizeof(int) * 3);
        // Extract City Identifier
        fscanf(input, "%d", &array[0]);
        // Extract x-coordinate
        fscanf(input, "%d", &array[1]);
        // Extract y-coordinate
        fscanf(input, "%d", &array[2]);
        // Strip newline
        fscanf(input, "%c", &str1);

        biggerArray[i] = array;
        i++;
    }

    *lineCount = i - 1;
    return biggerArray;
}

void destroy(int** arrays, int line_count) {
    int i;
    for(i = 0; i < line_count; i++) {
        free(arrays[i]);
    }
    free(arrays);
}

int pythag(int x1, int x2, int y1, int y2){
	return round(sqrt(pow((x1-x2),2) + pow((y1-y2),2)));
}

//takes the biggerArray of the cities and the number of them
//creates and returns a cost matrix of each city to each other city
//costMatrix[0][1] will hold the distance from the first city read in from input to the second read from input
int** buildCostMatrix(int** cities, int* lineCount){
	int i,j;
	int** costMatrix = (int**)malloc(sizeof(int) * 20000);	//copy/paste from buildArrays()
	//for each city
	for(i=0;i<lineCount;i++){
		//call pythag from current city to each other city
		for(j=0;j<lineCount;j++){
			if(i==j)
				costMatrix[i][j] = 0;
			else
				costMatrix[i][j] = pythag(cities[i][1],cities[j][1],cities[i][2],cities[j][2]);
		}
	}
	
	return costMatrix;
}

int main(int argc, char *argv[]){
    int i, j;

    /* File Input/Output Variables */
    FILE *output, *input;
    char* endFile = ".tour";
    char* outputFile;
    char* inputFile = argv[1];
    input = fopen(inputFile, "r");

    /* Creating .tour output file */
    int len = strlen(inputFile);
    char fileName[len];
    for (i=0; i< len; i++) {
        fileName[i] = inputFile[i];
	}
    outputFile = strcat(fileName, endFile);
    output = fopen(outputFile, "w");

    /* Copy Arrays */
    int** arrays;
    int line_count;
    arrays = buildArrays(input, &line_count);

    // Debug Code
    for(i = 0; i < line_count; i++) {
        for(j = 0; j < 3; j++) {
            printf("%d ", arrays[i][j]);
        }
        printf("\n");
    }

    destroy(arrays, line_count);
	close(output);
    close(input);
    return 0;
}