// C program for implementation of Bubble sort
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
  
void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
  
// bubble sort
void bubbleSort(int arr[], int n)
{

   #pragma omp parallel for schedule(dynamic,100)
   for (int i = 0; i < n-1; i++)      
       for (int j = 0; j < n-i-1; j++) 
           if (arr[j] > arr[j+1])
              swap(&arr[j], &arr[j+1]);
}
  
// Verify if the array is in ascending order
bool isArraySorted(int arr[], int size)
{
    for (int i=0; i < size - 1; i++)
        if(arr[i] > arr[i+1]) 
            return false;
    
    return true;
}
  
// Driver program to test above functions
int main()
{
    int i,n = 100000;
    int *arr = (int*) malloc(n*sizeof(int));
    
    #pragma omp parallel for
    for(i=0; i < n; i++)
        arr[i] = rand()%n;

    bubbleSort(arr, n);
    printf("Is array sorted?: \n");
    fputs(isArraySorted(arr, n) ? "true\n" : "false\n", stdout);
    return 0;
}