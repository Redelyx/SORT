#include <stdio.h>
#include "multiply.h"
#include "sum.h"

int main()
{
  int a = 3;
  int b = 5;
  printf("multiply(%d,%d) = %d\n", a, b, multiply(a,b));
  printf("sum(%d,%d) = %d\n", a, b, sum(a,b));

  return 0;
}
