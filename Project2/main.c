#include <stdio.h>
#include <limits.h>

/* Adapted from http://www.geeksforgeeks.org/find-minimum-number-of-coins-that-make-a-change/*/
int changeSlow(int V[], int size, int value) {
    int i, res, sub_res;
   // base case, ya broke
   if (value == 0) return 0;
   // Initialize result
   res = INT_MAX;
   // Try every coin that has smaller value than V
   for (i = 0; i< size; i++) {
     if (V[i] <= value) {
         sub_res = changeSlow(V, size, value-V[i]);
         // Check for INT_MAX to avoid overflow and see if result can minimized
         if ((sub_res != INT_MAX) && (sub_res + 1 < res))
            res = sub_res + 1;
     }
   }
   return res;
}

/*Adapted from http://www.geeksforgeeks.org/find-minimum-number-of-coins-that-make-a-change/ */
int changedp(int V[], int size, int value){
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
                i = -1;      //break out of for loop
            }
        }
    }
    
    return table[value];
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
	//print to consol the values in our output array
	printf("outputarr:\n");
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


// Driver program to test above function
int main()
{
    int arr[] = {1, 2, 4, 8};
	int value = 15;
    int size = sizeof(arr)/sizeof(arr[0]);
    //printf("%d \n", changedp(arr, size, value));
    //printf("%d \n", changeSlow(arr, size, value));
	changegreedy(arr, size, value);
    return 0;
}
