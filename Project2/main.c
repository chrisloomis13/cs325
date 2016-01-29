#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h> /* memset */


struct Array {
    int min;
    int size;
    int* V;
    int* C;
};

/* Adapted from http://www.geeksforgeeks.org/find-minimum-number-of-coins-that-make-a-change/*/
struct Array changeSlowHelper(struct Array a, int value) {
    struct Array subProblem;
    int i, minCoins, numCoins;
    // base case, ya broke
    if (value == 0) {
        a.min = 0;
        return a;
    }
    // Initialize result
    minCoins = value;
    // Try every coin that has smaller value than target Value
    for (i = 0; i< a.size; i++) {
        if (a.V[i] <= value) {
            // Reset coin counts
            memset(a.C, 0, sizeof(int)*a.size);
            subProblem = changeSlowHelper(a, value-a.V[i]);
            // Incrementing the coin
            subProblem.C[i]++;
            numCoins = subProblem.min;
            // Check for INT_MAX to avoid overflow and see if result can minimized
            if ((numCoins != INT_MAX) && (numCoins + 1 < minCoins)) {
                minCoins = numCoins + 1;
                a.C = subProblem.C;
            }
        }
    }
    a.min = minCoins;
    return a;
}

void changeSlow(int V[], int size, int value){
    // Create Struct to hold Array Information
    struct Array a;
    a.V = V;
    a.size = size;
    int* coinArr = malloc(sizeof(int) * size);
    memset(coinArr, 0, sizeof(int)*size);
    a.C = coinArr;

    a = changeSlowHelper(a, value);

	//print to console the values in our output array
	printf("\n***changeslow***\n");
	printf("C = [");
	int i;
	for(i = 0; i < size; i++)
	{
		printf("%d", a.C[i]);
		if(i < size-1)
		{
			printf(", ");
		}
	}
	printf("]\n");
    printf("m = %d\n", a.min);
    free(coinArr);
}

/*Adapted from http://www.geeksforgeeks.org/find-minimum-number-of-coins-that-make-a-change/ */
void changedp(int V[], int size, int value){
    int i, j;
    // table[i] will be storing the minimum number of coins
    // required for i value.  So table[value] will have result
    int table[value+1];

    // Base case (If given value is 0)
    table[0] = 0;

    // Initialize all table values as Infinite
    for (i = 1; i <= value; i++)
        table[i] = INT_MAX;

    // Compute minimum coins required for all
    // values from 1 to value
    for (i = 1; i <= value; i++){
        // Go through all coins smaller than i
        for (j = 0; j < size; j++)
          if (V[j] <= i){
              int sub_res = table[i - V[j]];
              if ((sub_res != INT_MAX) && (sub_res + 1 < table[i]))
                  table[i] = sub_res + 1;
          }
    }

    int countBack = table[value]; //gets number of coins
    int n;

    int combo[size];    //holds combination of coints
    for(i = 0; i < size; i++)
        combo[i] = 0;   //put 0s in combo

    j = value;  //gets value which is used as index

    while(countBack > 0){
        for(i = size-1; i >=0; i--){
            n = j - V[i];
            if(table[n] == table[j]-1){
                combo[i]++;
                j = n;
                countBack--;
            }
        }
    }

	//print to console the values in our output array
	printf("\n***changedp***\n");
	printf("C = [");
	for(i = 0; i < size; i++)
	{
		printf("%d", combo[i]);
		if(i < size-1)
		{
			printf(", ");
		}
	}
	printf("]\n");
    printf("m = %d\n", table[value]);
}

void changegreedy(int V[], int size, int value){
    int i, currentValue;
    int count = 0, amount = 0;
    int combo[size];    //holds combination of coins used

    for(i = 0; i < size; i++)
        combo[i] = 0;   //put 0s in combo

    for(i = size - 1; i >= 0; i--) {
        currentValue = V[i];
        while(amount < value ) {
            amount = amount + V[i];
            count++;
            combo[i]++;
        }
        if(amount > value) {
            amount = amount - V[i];
            count--;
            combo[i]--;
        }
    }

	//print to console the values in our output array
	printf("\n***changegreedy***\n");
	printf("C = [");
	for(i = 0; i < size; i++)
	{
		printf("%d", combo[i]);
		if(i < size-1)
		{
			printf(", ");
		}
	}
	printf("]\n");
    printf("m = %d\n", count);
}

int main()
{
    int arr[] = {1, 2, 4, 8};
	int value = 15;
    int size = sizeof(arr)/sizeof(arr[0]);
    //printf("%d \n", changeSlow(arr, size, value));
    changeSlow(arr, size, value);
    changedp(arr, size, value);
	changegreedy(arr, size, value);
    return 0;
}
