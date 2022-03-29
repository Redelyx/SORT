#include <stdio.h>

#include "multiply.h"
#include "square.h"

int main(void)
{
  printf("multiply(%d, %d) = %d\n", FIRST_NUM, SECOND_NUM, multiply(FIRST_NUM, SECOND_NUM));
  printf("square(%d) = %d\n", FIRST_NUM, square(FIRST_NUM));
}
