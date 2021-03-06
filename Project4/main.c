/* Project 3 - Traveling Salesman Problem
    Group 9:
    Christopher Loomis
    Benjamin Tullis
    Dalena Pham

    Sources:
    http://www.ccodechamp.com/c-program-of-travelling-salesman-problem/
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

int pythag(int x1, int x2, int y1, int y2){
	return round(sqrt(pow((x1-x2),2) + pow((y1-y2),2)));
}

//takes the biggerArray of the cities and the number of them
//creates and returns a cost matrix of each city to each other city
//costMatrix[0][1] will hold the distance from the first city read in from input to the second read from input
int** buildCostMatrix(int** cities, int lineCount){
	int i,j;
	int** costMatrix = (int**)malloc(sizeof(int) * 20000);	//copy/paste from buildArrays()
	//for each city
	for(i = 0; i < lineCount; i++){
        // allocate space for row
        int* row = malloc(sizeof(int) * lineCount);
		//call pythag from current city to each other city
		for(j = 0; j < lineCount; j++){
			if(i == j)
				row[j] = INT_MAX;
			else
				row[j] = pythag(cities[i][1],cities[j][1],cities[i][2],cities[j][2]);
		}
        costMatrix[i] = row;
	}
	return costMatrix;
}


/* Finds the minimum value in both the row and column of a cost matrix,
subtracts this value from the cost matrix
 */
void subtractMinimum(int** costM, int lineCount) {
    int i, j;
    /* Find the minimum value of each row, subtract
    each value in row by minimum value */
    int minRow;
    for(i = 0; i < lineCount; i++) {
        minRow = costM[i][0];
        /* Find the minimum element in each row */
        for(j = 0; j < lineCount; j++) {
            if (costM[i][j] < minRow)
                minRow = costM[i][j];
        }
        /* Subtract the minimum element from each row */
        for (j = 0; j < lineCount; j++) {
            costM[i][j] = costM[i][j] - minRow;
        }
    }

    /* Find minimum value in each column, subtract
    each value in column by minimum value */
    int minCol;
    for(i = 0; i < lineCount; i++) {
        minCol = costM[0][i];
        /* Find minimum element in each column */
        for(j = 0; j < lineCount; j++) {
            if(costM[j][i] < minCol)
                minCol = costM[j][i];
        }
        /* Subtract the minimum element from each column */
        for(j = 0; j < lineCount; j++) {
            costM[j][i] = costM[j][i] - minCol;
        }
    }
}

int tsp_dp (int** c, int* tour, int start, int n) {
    int i, j, k; /* Loop counters. */
    int* temp = malloc(sizeof(int) * n); /* Temporary during calculations. */
    int* mintour = malloc(sizeof(int) * n); /* Minimal tour array. */
    int mincost; /* Minimal cost. */
    int ccost; /* Current cost. */

    /* End of recursion condition. */
    if (start == n - 2)
    return c[tour[n-2]][tour[n-1]] + c[tour[n-1]][0];

    /* Compute the tour starting from the current city. */
    mincost = INT_MAX;
        for (i = start+1; i<n; i++){
        for (j=0; j<n; j++)
        temp[j] = tour[j];

        /* Adjust positions. */
        temp[start+1] = tour[i];
        temp[i] = tour[start+1];

        /* Found a better cycle? (Recurrence derivable.) */
        if (c[tour[start]][tour[i]] +
            (ccost = tsp_dp (c, temp, start+1, n)) < mincost) {
            mincost = c[tour[start]][tour[i]] + ccost;
            for (k=0; k<n; k++)
                mintour[k] = temp[k];
            }
    }

    /* Set the minimum-tour array. */
    for (i=0; i<n; i++)
        tour[i] = mintour[i];


    free(temp);
    free(mintour);

    return mincost;
}

/* Deallocates reserved memory */
void destroy(int** cities, int** costM, int line_count) {
    int i;
    for(i = 0; i < line_count; i++) {
        free(cities[i]);
        free(costM[i]);
    }
    free(cities);
    free(costM);
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

    /* Copy Cities */
    int** cities;
    int lineCount;
    cities = buildArrays(input, &lineCount);

    // Debug Code - print cities
    /*for(i = 0; i < lineCount; i++) {
        for(j = 0; j < 3; j++) {
            printf("%d ", cities[i][j]);
        }
        printf("\n");
    }*/

    /* Create Cost Matrix */
    int** costM;
    costM = buildCostMatrix(cities, lineCount);

    // Debug Code - print Cost Matrix
    /*for(i = 0; i < lineCount; i++) {
        for(j = 0; j < lineCount; j++) {
            printf("%d ", costM[i][j]);
        }
        printf("\n");
    }*/

    /* Zero out rows and columns */
    subtractMinimum(costM, lineCount);

    /* To-do, put the following into function and figure what it all means */
    int flag[lineCount][lineCount];
    int nrz[lineCount];
    int ncz[lineCount];

    int nrz1[lineCount];
    int ncz1[lineCount];


    int min = INT_MAX;
    // a = minimum number of lines
    int a = 0, noz = 0;
    // Initializing all flag spots to 0
    for(i = 0; i < lineCount; i++) {
        for(j = 0; j < lineCount; j++) {
            flag[i][j] = 0;
        }
    }

    int cn;
    // Count and mark number of zeros in rows
    for(i = 0; i < lineCount; i++) {
        cn = 0;
        for(j = 0; j < lineCount; j++) {
            if(costM[i][j] == 0) {
                cn++;
                flag[i][j] = 1;
            }
        }
        // Record number of zeros in specific row
        nrz[i] = cn;
        // Record overall number of zeros
        noz = noz + cn;
    }

    // Count and mark number of zeros in columns
    for(i = 0; i < lineCount; i++) {
        cn = 0;
        for(j = 0; j < lineCount; j++) {
            if(costM[j][i] == 0) {
                cn++;
                flag[j][i] = 1;
            }
        }
        // Record number of zeros in specific column
        ncz[i] = cn;
        // Record overall number of zeros
        noz = noz + cn;
    }

    // Copy arrays recording number of zeros in rows and columns
    for(i = 0; i < lineCount; i++) {
        nrz1[i] = nrz[i];
        ncz1[i] = ncz[i];
    }

    int k = 0;
    // while there is row or column that has a zero in it
    while(nrz[k] != 0 || ncz[k] != 0) {

        for(i = 0; i < lineCount; i++) {
            // Count number of flagged spots in row, record in nrz
            cn = 0;
            for(j = 0; j < lineCount; j++) {
                if(flag[i][j] == 1)
                    cn++;
                nrz[i] = cn;
            }

            // If there is only 1 flag in row
            if(nrz[i] == 1) {
                for(j = 0; j < lineCount; j++) {
                    // Find the flag
                    if(flag[i][j] == 1) {
                        // Reassign it
                        flag[i][j] = 2;
                        // Find other flags in column
                        for(k = 0; k < lineCount; k++) {
                            // Reassign flag
                            if(flag[k][j] == 1) {
                                flag[k][j] = 0;
                            }
                        }
                    }
                }
            }
        }

        // Traverse through columns
        for(i = 0; i < lineCount; i++) {
            cn = 0;
            // Count number of flags in column
            for(j = 0; j < lineCount; j++) {
                if(flag[j][i] == 1) {
                    cn++;
                    ncz[i] = cn;
                }
            }
            // If there was only 1 flag in column
            if(ncz[i] == 1) {
                for(j = 0; j < lineCount; j++) {
                    // Search for that flag
                    if(flag[j][i] == 1) {
                        // Reassign it
                        flag[j][i] = 2;
                        // Find all other flags in row, reassign to 0
                        for(k = 0; k < lineCount; k++) {
                            if(flag[j][k] == 1) {
                                flag[j][k] = 0;
                            }
                        }
                    }
                }
            }
        }
        k++;
    }

    a = 0;
    for(i = 0; i < lineCount; i++) {
        for(j = 0; j < lineCount; j++) {
            if(flag[i][j] == 2) {
                a++;
            }
        }
    }

    if (a == lineCount) {
        fprintf(output, "%d\n", a);
        for(i = 0; i < lineCount; i++) {
            for(j = 0; j < lineCount; j++) {
                if(flag[i][j] == 2) {
                    fprintf(output, "%d\n", i);
                }
            }
        }
    }

    else {
        int rf[lineCount];
        int sr[lineCount];
        int cf[lineCount];
        int sc[lineCount];

        // Initialize all to zero
        for(i = 0; i < lineCount; i++) {
            rf[i] = 0;
            sr[i] = 0;
            cf[i] = 0;
            sc[i] = 0;
        }

        int m;
        for(k = lineCount; (k > 0 && noz != 0); k--) {
            for(i = 0; i < lineCount; i++) {
               m = 0;
                for(j = 0; j < lineCount; j++) {
                    if((flag[i][j] == 4)&&(costM[i][j] == 0))
                        m++;
                }
                sr[i] = m;
            }

            for(i = 0; i < lineCount; i++) {
                if(nrz1[i] == k && nrz1[i] != sr[i]) {
                    rf[i] = 1;
                    for(j = 0; j < lineCount; j++) {
                        if(costM[i][j] == 0)
                            flag[i][j] = 4;
                    }
                    noz = noz - k;
                }
            }
            int l;
            for(i = 0; i < lineCount; i++) {
                l = 0;
                for(j = 0; j < lineCount; j++) {
                    if((flag[j][i] == 4) && (costM[j][i] == 0))
                        l++;
                }
                sc[i] = l;
            }

            for(i = 0; i < lineCount; i++) {
                if(ncz1[i] == k && ncz1[i] != sc[i]) {
                    cf[i] = 1;
                    for(j = 0; j < lineCount; j++) {
                        if(costM[j][i] == 0)
                            flag[j][i] = 4;
                    }
                    noz = noz - k;
                }
            }

            for(i = 0; i < lineCount; i++) {
                for (j = 0; j < lineCount; j++) {
                    if(flag[i][j] != 3) {
                        if(rf[i] == 1 && cf[j] == 1) {
                            flag[i][j] = 3;
                            if(cost[i][j] == 0)
                                noz = noz + 1;
                        }
                    }
                }

            }
        }

        for(i = 0; i < lineCount; i++) {
            for(j = 0; j < lineCount; j++) {
                if(rf[i] != 1 && cf[j] != 1) {
                    if(min > costM[i][j])
                        min = cost[i][j];
                }
            }
        }

        for(i = 0; i < lineCount; i++) {
            for(j = 0; j < lineCount; j++) {
                if(rf[i] != 1 && cf[j] != 1)
                    cost[i][j] = cost[i][j] - min;
            }
        }

        for(i = 0; i < lineCount; i++) {
            for(j = 0; j < lineCount; j++) {
                if(flag[i][j] == 3)
                    cost[i][j] = cost[i][j] + min;
            }
        }
    }


    destroy(cities, costM, lineCount);
	close(output);
    close(input);
    return 0;
}
