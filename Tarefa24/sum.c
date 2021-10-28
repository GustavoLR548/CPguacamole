#include <stdio.h>
#include <stdlib.h>

int main()
{
  int width = 40000000;
  double sum = 0;


  for(int i = 0; i < width; i++)
  {
     sum += i;
  }

  printf("\nSum = %f\n",sum);
}