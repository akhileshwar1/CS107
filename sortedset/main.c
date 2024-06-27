#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sortedset.h"

int cmp(const void *a, const void *b) {
  return *(int *)a - *(int *)b;
}

int main() {
  // make the new integer sorted set.
  sortedset intSet;
  cmpfn icmp = cmp;
  SetNew(&intSet, sizeof(int), icmp);
  printf("sorted set size is %d\n", intSet.allocatedLength);
  printf("is sortedset filled? %d\n", *(short *)intSet.elems);

  // add a node.
  int a = 2;
  bool add = SetAdd(&intSet, &a);
  void *searchPtr = SetSearch(&intSet, &a);
  printf("searched value is %d\n", *(int *)searchPtr);
  return 0;
}
