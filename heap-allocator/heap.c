#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "heap.h"

void createHeap(heap *h, int size) {
  h->totalBytes = size;
  h->freePtr = malloc(size);  // metacircular.
  
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
    if (*(char **)it == NULL) {
      printf("insufficient memory");
      return NULL;
    }

    it = *(char **)it;
    availableSpace = *(int *)((char *)it + 8);
  }

  // detach the new block from the chosen block and update the chosen block with rem size.
  int remSize = availableSpace - size;
  memcpy((char *)it + 8 , &remSize, sizeof(int));

  // initialize the new block.
  char *newPtr = NULL;
  memcpy((char *)it + 8 + 4 +remSize, &newPtr, sizeof(char *));
  memcpy((char *)it + 8 + 4 + remSize + 8, &size, sizeof(int));

  // return the pointer to the data portion of the new block.
  return (char *)it + 8 + 4 + remSize + 8 + 4;
}

void cfree(heap *h, void *ptr) {
  assert(h != NULL && ptr != NULL);

  int size = *(int *)((char *)ptr - 4);
  ptr = ptr - 4 - 8; // go back to the pointer portion of the block.
  if (size < 0 || size > h->totalBytes) {
    printf("invalid pointer");
  }

  // find the pointers to the right and left of the malloced block. 
  void *current = h->freePtr;
  printf("next free ptr is %p\n", *h->freePtr);
  void *prev;
  int flag = 0;
  while (ptr - current > 0) {
    printf("current is %p\n", *(char **)current);
    if(*(char **)current == NULL) {
      flag = 1;
      break;
    }

    prev = current;
    current = *(char **)current;
  }

  if (flag == 1) {
    memcpy(current, &ptr, sizeof(char *)); // add to the right of the last free block.
  } else {
    // insert the malloced block in between the left and right free blocks.
    // No need to zero the malloced memory while freeing.
    memcpy(prev, &ptr, sizeof(char *));
    memcpy(ptr, &current, sizeof(char *));
  }

  printf("Block freed: %p\n", ptr);
}
