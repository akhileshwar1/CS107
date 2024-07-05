#ifndef _heap_
#define _heap_

/* The idea here is to simulate the heap as a contiguous block of memory
 * interpreted as a linked list of free blocks for the malloc and free ops.
 * One free block is made up of:
 * next free ptr, no of bytes available, data.
 */

typedef struct {
  int totalBytes;
  void **freePtr; // ** as it is a pointer to a pointer to the next free block.
} heap;

/*
* Function: createHeap
* Usage: createHeap(1024);
* ---------------------------
* Creates the heap with the given size in bytes.
*/
void createHeap(heap *h, int size);

/*
* Function: cmalloc
* Usage: cmalloc(4);
* ----------------------------
* Naive malloc implementation.
* Will search for the first free block that has the available
* bytes and returns the pointer of the data portion. 
* This memory is removed from the free list of blocks by
* disassociating the pointers from and to the prev and next
* free blocks in the linked list.
*/
void *cmalloc(heap* h, int size);

/*
* Function: cfree
* Usage: cfree(ptr);
* ---------------------------
* Naive implementation of free.
* Will move back 8 bytes from the data portion and then
* reinstate it as free block in the free list. 
*/
void cfree(heap *h, void *ptr);

#endif
