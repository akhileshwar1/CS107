#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// helpful comparator fn for descending order on ints.
int compare(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

void inc(void *elemAddr, void *auxData) {
  int a = *(int *)elemAddr + 1;
  memcpy(elemAddr, &a, sizeof(int));
}


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

  // replace the vector at 4th position with 11.
  int elem = 11;
  VectorReplace(&vec, &elem, 4);
  int *nth_ptr = VectorNth(&vec, 4);
  printf("new vector value at the 4th position is %d\n", *nth_ptr);

  // insert at capacity, should resize the vector.
  VectorInsert(&vec, &elem, 4);
  nth_ptr = VectorNth(&vec, 5);
  printf("new vector value at the 5th position is %d\n", *nth_ptr);

  // delete at last position.
  VectorDelete(&vec, 3);
  nth_ptr = VectorNth(&vec, 3);
  printf("new vector value at the 3rd position is %d\n", *nth_ptr);

  // sort the array.
  VectorCompareFunction comparator = compare;
  VectorSort(&vec, comparator);
  nth_ptr = VectorNth(&vec, 2);
  printf("new vector value at the 2nd position is %d\n", *nth_ptr);

  // use the inc map function to increment the ints of vector.
  VectorMapFunction map = inc;
  VectorMap(&vec, map, NULL);
  nth_ptr = VectorNth(&vec, 4);
  printf("new vector value at the 4th position is %d\n", *nth_ptr);


  VectorDispose(&vec);
  if (vec.elems == NULL) printf("vector is null\n");
}
