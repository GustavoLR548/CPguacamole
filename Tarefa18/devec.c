#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
devec.c:42:15: missed: not vectorized: multiple exits.
devec.c:31:11: missed: not vectorized: number of iterations cannot be computed.
devec.c:20:18: missed: not vectorized: control flow in loop.
devec.c:10:19: missed: not vectorized: latch block not empty.
*/

int main()
{
    int a[2048], b[2048], c[2048];
    char cp_a[50];
    char cp_b[50];
    int i;

    for (i = 0; i < rand() % 2048; i++)
    {
        b[i] = 0;
        c[i] = 0;
    }

    for (i = 0; i < 2048; i++)
    {
        a[i] = b[i] + c[i];

        if (i == rand() % 20)
            break;
    }

    i = 0;

    while (i < 2048)
    {
        a[i] = b[i] + c[i];
        i = rand() % 2048;

        if(i == 3)
            break;
    }

    int aux = rand() % i;
    
    for(i = 0; i < 2048; i++)
    {
        a[i] = b[i] + c[i];

        i = aux;
        aux = rand() % i;
    }

    for(i = 0; i < 2048; i++)
    {
        a[i] = b[i] + c[i];

        if(a[i] > 0) { 
            b[i] = a[i] * (i++ * ++i * c[++i]);
        }
    }
    
    return 0;
}