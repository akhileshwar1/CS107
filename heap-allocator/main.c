#include <stdio.h>
#include "heap.h"

int main() {
  // check if the heap is initialized correctly.
  heap h;
  createHeap(&h, 1024);
  printf("next free pointer is %p\n", *h.freePtr);
  printf("no of bytes in heap are %d\n", *(int *)((char *)h.freePtr + 8));

  // check if malloc returns the correct memory location by going back 4 bytes
  // and checking the no of bytes available.
  char *it = cmalloc(&h, 20);
  printf("available bytes are %d\n", *(int *)((char *)it - 4));

  // one more check
  it = cmalloc(&h, 50);
  printf("available bytes are %d\n", *(int *)((char *)it - 4));

}
