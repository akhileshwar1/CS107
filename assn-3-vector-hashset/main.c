#include "vector.h"
#include <stdio.h>

int main() {
  printf("in main\n");
  vector vec;
  VectorNew(&vec, sizeof(int), NULL, 4);
  printf("vector length is %x\n", vec.allocatedLength);

  // append an int to the
  int n = 10;
  VectorAppend(&vec, &n);
  int *ptr = VectorNth(&vec, 0);
  printf("vector at 0th position is %d\n", *ptr);

  // go beyond the current length of the vector and check if it resizes.
  for (int i = 0; i < 4 ; i++) {
    VectorAppend(&vec, &i);
  }

  int *ptr_end = VectorNth(&vec, 4);
  printf("vector value at the 4th position is %d\n", *ptr_end);
  printf("resized vector length is %d\n", VectorLength(&vec));
  VectorDispose(&vec);
  if (vec.elems == NULL) printf("vector is null\n");
}
