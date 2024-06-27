#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "sortedset.h"

// implementing the sortedset fns.

void SetNew(sortedset *set, int elemSize, int (*cmpfn)(const void *, const void *)) {
  assert(cmpfn != NULL && elemSize > 0 && set != NULL);
  set->elemSize = elemSize;
  set->logLength = 0;
  set->allocatedLength = 4;
  set->cpfn = cmpfn;
  set->elems = malloc(2 + 4*elemSize); // 2 for the first short that indicated if the set is filled?
  short s = 0;
  *(short *)set->elems = s;
}

void addNode(sortedset *set, const void *elemPtr) {
  void *curr = (char *)set->elems + 2; // to the end of the short value.
  curr = curr + set->logLength*(set->elemSize + 8);
  memcpy(curr, elemPtr, set->elemSize);
  int initVal = -1;
  curr = (char *)curr + set->elemSize;
  memcpy(curr, &initVal, sizeof(int)); // setting the left child to -1 offset.
  curr = (char *)curr + set->elemSize;
  memcpy(curr, &initVal, sizeof(int)); // setting the right child to -1 offset.
  set->logLength++;
}

/*
* Function: getNode
* Usage: getNode(&intSet, 4);
* ---------------------------
* Gets the node at the nth position assuming 0 indexing.
*/
void *getNode(sortedset *set, int offset) {
  void *curr = (char *)set->elems + 2;
  curr = curr + offset*(set->elemSize + 8);
  return curr;
}

bool SetAdd(sortedset *set, const void *elemPtr) {
  assert(set != NULL && elemPtr != NULL);

  // double the size if there is no space.
  if (set->logLength == set->allocatedLength) {
    int newLength = 2*set->allocatedLength;
    set->elems = realloc(set->elems, newLength);
    set->allocatedLength = newLength;
  }

  // add the node regardless with nil children.
  addNode(set, elemPtr);

  // return true as there is no linking to be done here. 
  if (*(short *)set->elems == 0) {
    return true;
  }

  void *curr = (char *)set->elems + 2;

  // Making the added node a child of a previously added node.
  int offset = 0;
  int r = 0;
  int l = 0;
  while (offset != -1) {
    r = 0;
    l= 0;
    curr = getNode(set, offset);
    int res = set->cpfn(elemPtr, curr);
    if (res < 0) {
      void *left = (char *)curr + set->elemSize;
      offset = *(int *)left;
      l = 1;
    } else if (res > 0) {
      void *right = (char *)curr + set->elemSize;
      offset = *(int *)right;
      r = 1;
    } else {
      return false; // already present.
    }
  }

  if (l == 1) {
    curr = (char *)curr + set->elemSize; // to the left offset.
    memcpy(curr, &set->logLength, sizeof(int));
  } else {
    curr = (char *)curr + set->elemSize + 4; // to the right offset.
    memcpy(curr, &set->logLength, sizeof(int));
  }

  return true;
}

void *SetSearch(sortedset *set, const void *elemPtr) {
  void *curr;
  int offset = 0;
  while (offset != -1) {
    curr = getNode(set, offset);
    int res = set->cpfn(elemPtr, curr);
    if (res < 0) {
      void *left = (char *)curr + set->elemSize;
      offset = *(int *)left;
    } else if (res > 0) {
      void *right = (char *)curr + set->elemSize;
      offset = *(int *)right;
    } else {
      return curr; // already present.
    }
  }

  return NULL; // not found.
}
