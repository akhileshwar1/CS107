#include "vector.h"
#include <stdio.h>

int main() {
  printf("in main\n");
  vector vec;
  VectorNew(&vec, sizeof(int), NULL, 4);
  printf("vector length is %x\n", vec.allocatedLength);
}
