#include <stdio.h>

/* Located in the file multiply.c */
int multiply(int a, int b);

int main(void)
{
  int a = 3;
  int b = 5;
  printf("multiply(%d,%d) = %d\n", a, b, multiply(a,b));
}
