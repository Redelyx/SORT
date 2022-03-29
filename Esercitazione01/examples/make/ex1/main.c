#include <stdio.h>
#include "multiply.h"

int main(void)
{
  int first = 3;
  int second = 5;
  printf("multiply(%d,%d) = %d\n", first, second, multiply(first,second));
}
