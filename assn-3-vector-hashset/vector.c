#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation)
{
  v->allocatedLength = initialAllocation;
  v->elemSize = elemSize;
  v->logLength = 0;
  v->freeFn = freeFn;
  v->elems = malloc(initialAllocation*elemSize);
}

void VectorDispose(vector *v)
{
  if (v->freeFn != NULL) {
    for (int i = 0; i < v->allocatedLength; i++) {
      void *elemAddr = (char *)v->elems + i*v->elemSize;
      v->freeFn(elemAddr);
    }
  }

  // embedded memory pointers freed, now we can free the actual elems memory.
  free(v->elems);
  v->elems = NULL;
}

int VectorLength(const vector *v)
{
  return v->logLength;
}

void *VectorNth(const vector *v, int position)
{
  assert(position>=0 && position<v->logLength);

  void *ptr = (char *)v->elems + position*v->elemSize;
  return ptr;
}

void VectorReplace(vector *v, const void *elemAddr, int position)
{
  assert(position>=0 && position<v->logLength);
  void *ptr = (char *)v->elems + position*v->elemSize;

  if (v->freeFn != NULL) {
    v->freeFn(ptr);
  }

  memcpy(ptr, elemAddr, v->elemSize);
}

void VectorInsert(vector *v, const void *elemAddr, int position)
{
  assert(position>=0 && position<=v->logLength);
  if (v->logLength == v->allocatedLength) {
    // resize it to double of the previous size.
    v->elems = realloc(v->elems, 2*v->allocatedLength*v->elemSize);
    v->allocatedLength = 2*v->allocatedLength;
  }

  void *src = (char *)v->elems + position*v->elemSize;
  void *dest = (char *)v->elems + (position+1)*v->elemSize;
  memmove(dest, src, (void *)((char *)v->elems + v->allocatedLength*v->elemSize) - src);
  memcpy(src, elemAddr, v->elemSize);
  v->logLength++;

}

void VectorAppend(vector *v, const void *elemAddr)
{
  if (v->logLength == v->allocatedLength) {
    // resize it to double of the previous size.
    v->elems = realloc(v->elems, 2*v->allocatedLength*v->elemSize);
    v->allocatedLength = 2*v->allocatedLength;
  }

  // all clear.
  memcpy((char *)v->elems + v->logLength*v->elemSize, elemAddr, v->elemSize);  
  v->logLength++;
}

void VectorDelete(vector *v, int position)
{
  assert(position>=0 && position<=v->logLength);
  if (v->freeFn != NULL) {
    void *ptr = (char *)v->elems + position*v->elemSize;
    v->freeFn(ptr);
  }

  void *dest = (char *)v->elems + position*v->elemSize;
  void *src = (char *)v->elems + (position+1)*v->elemSize;
  // there is an overlap in source and dest memory area, but not concerning while copying elems.
  memcpy(dest, src, (void *)((char *)v->elems + v->allocatedLength*v->elemSize) - src);
  v->logLength--;
}

void VectorSort(vector *v, VectorCompareFunction compare)
{
  assert(compare != NULL);
  qsort(v->elems, v->allocatedLength, v->elemSize, compare);
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)
{
  assert(mapFn != NULL);
  for (int i = 0; i< v->allocatedLength; i++) {
    mapFn((char *)v->elems + i*v->elemSize, auxData);
  }
}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{
  assert(key != NULL && searchFn != NULL);
  void *ptr;
  if (isSorted) {
    ptr = bsearch(key, (char *)v->elems + startIndex*v->elemSize, v->allocatedLength, 
                        v->elemSize, searchFn);
    
  } else {
    ptr = bsearch(key, (char *)v->elems + startIndex*v->elemSize, v->allocatedLength, 
                        v->elemSize, searchFn);
  }
  if (ptr == NULL) {
    return -1;
  } else {
    return (ptr - v->elems)/v->elemSize;
  }
}
