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
{ return 0; }

void *VectorNth(const vector *v, int position)
{ return NULL; }

void VectorReplace(vector *v, const void *elemAddr, int position)
{}

void VectorInsert(vector *v, const void *elemAddr, int position)
{}

void VectorAppend(vector *v, const void *elemAddr)
{}

void VectorDelete(vector *v, int position)
{}

void VectorSort(vector *v, VectorCompareFunction compare)
{}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)
{}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{ return -1; } 
