#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "heap.h"

void createHeap(heap *h, int size) {
  h->totalBytes = size;
  h->freePtr = malloc(size);  // metacircular haha.
  
  // create the first block with size as the size of the heap.
  *h->freePtr = NULL;
  void *it = h->freePtr;
  it = (char *)it + 8;
  memcpy(it, &size, sizeof(int));
}

void *cmalloc(heap *h, int size) {
  assert(h != NULL && size < h->totalBytes);

  // heap is full, no free blocks.
  if (h->freePtr == NULL) {
    return NULL;
  }

  // choose the free block.
  void *it = h->freePtr;
  int availableSpace = *(int *)((char *)it + 8);
  while (availableSpace < size) {
    it = (char *)it + 8;
    availableSpace = *(int *)it;
  }

  // update the free list starting by updating the chosen free block.
  // current block with remSize => rem block with size => next block. 
  int remSize = availableSpace - size;
  memcpy(it, &remSize, sizeof(int));
  char *next = (char *)it + 4 + remSize;
  memcpy((char *)it + 4 + remSize, (char *)it - 8, sizeof(char *));
  memcpy((char *)it - 8, &next, sizeof(char *));
  memcpy((char *)it + 4 + remSize + 8, &size, sizeof(int));

  return (char *)it + 4 + remSize + 8 + 4;
}
