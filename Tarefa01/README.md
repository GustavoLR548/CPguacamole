# Modifique o seguinte código, utilizando o padrão MAP (e outros comandos do OpenMP),

#include <stdio.h>
#include <omp.h>

```c
int main()
{
    int i;

    #pragma omp parallel num_threads(2) // seta o número de threads em 2 
    {
        int tid = omp_get_thread_num(); // lê o identificador da thread 
        for(i = 1; i <= 3; i++) 
        {
           printf("[PRINT1] T%d = %d \n",tid,i);
           printf("[PRINT2] T%d = %d \n",tid,i);
        }
    }
}
```

para que a saída do programa seja:

```
[PRINT1] T0 = 1 
[PRINT2] T0 = 1 
[PRINT1] T0 = 2 
[PRINT2] T0 = 2 
[PRINT1] T1 = 3 
[PRINT2] T1 = 3
```