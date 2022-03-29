#include <stdio.h>

/* Prototipo della funzione */

int multiply(int a, int b);




int main(void)
{
  int first = 3;
  int second = 5;
  printf("multiply(%d,%d) = %d\n", first, second, multiply(first,second));
}




/* Definizione della funzione */

int multiply(int a, int b)
{
  return(a*b);
}
