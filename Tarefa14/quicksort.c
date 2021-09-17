/*
Speedup = 1.21875


Tempo sequencial

real	0m3.925s
user	0m3.887s
sys	0m0.036s

real	0m3.952s
user	0m3.924s
sys	0m0.024s

real	0m3.930s
user	0m3.898s
sys	0m0.028s

real	0m3.930s
user	0m3.885s
sys	0m0.040s

real	0m3.931s
user	0m3.894s
sys	0m0.032s

real	0m3.933s
user	0m3.902s
sys	0m0.028s

Tempo paralelo

real	0m3.216s
user	0m4.382s
sys	0m0.320s

real	0m3.201s
user	0m4.373s
sys	0m0.315s

real	0m3.209s
user	0m4.436s
sys	0m0.256s

real	0m3.242s
user	0m4.423s
sys	0m0.295s

real	0m3.207s
user	0m4.405s
sys	0m0.288s

*/

/* C implementation QuickSort from  http://w...content-available-to-author-only...s.org/quick-sort/ */
    #include<stdio.h>
    #include<stdlib.h>
    #include<omp.h>

    // A utility function to swap two elements
    void swap(int* a, int* b)
    {
      int t = *a;
      *a = *b;
      *b = t;
    }
     
    /* This function takes last element as pivot, places
       the pivot element at its correct position in sorted
        array, and places all smaller (smaller than pivot)
       to left of pivot and all greater elements to right
       of pivot */
    int partition (int arr[], int low, int high)
    {
      int pivot = arr[high];    // pivot
      int i = (low - 1);  // Index of smaller element
     

      for (int j = low; j <= high- 1; j++)
        {
          // If current element is smaller than or
          // equal to pivot
          if (arr[j] <= pivot)
            {
    	  i++;    // increment index of smaller element
    	  swap(&arr[i], &arr[j]);
            }
        }
      swap(&arr[i + 1], &arr[high]);
      return (i + 1);
    }
     
    /* The main function that implements QuickSort
     arr[] --> Array to be sorted,
      low  --> Starting index,
      high  --> Ending index */
void quickSort(int arr[], int low, int high, int aux)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
        at right place */
        int pi = partition(arr, low, high);

        if(aux > 20)
        {
          quickSort(arr, low, pi - 1, aux);
          quickSort(arr, pi + 1, high, aux);

          return;
        }

        // Separately sort elements before
        // partition and after partition
        #pragma omp parallel sections
        {
            #pragma omp section
            {
              quickSort(arr, low, pi - 1, aux+1);
            }
            
            #pragma omp section
            {
              quickSort(arr, pi + 1, high, aux+1);
            }
        }
    }
}
     
    /* Function to print an array */
    void printArray(int arr[], int size)
    {
      int i;
      for (i=0; i < size; i++)
        printf("%d ", arr[i]);
      printf("\n");
    }
     
    // Driver program to test above functions
    int main()
    {
      int i,n = 10000000;
      int *arr = (int*) malloc(n*sizeof(int));
    
      for(i=0; i < n; i++)
        arr[i] = rand()%n;
    
      //omp_set_nested(4);
      quickSort(arr, 0, n-1, 0);
      //printf("Sorted array: \n");
      //printArray(arr, n);
      return 0;
    }
