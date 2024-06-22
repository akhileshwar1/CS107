#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vector.h"

void HashSetNew(hashset *h, int elemSize, int numBuckets,
		HashSetHashFunction hashfn, HashSetCompareFunction comparefn, HashSetFreeFunction freefn)
{
  assert(elemSize > 0 && numBuckets > 0 && hashfn != NULL && comparefn != NULL);
  h->numBuckets = numBuckets;
  h->elemSize = elemSize;
  h->hashfn = hashfn;
  h->comparefn = comparefn;
  h->freefn = freefn;
  for (int i = 0; i< numBuckets; i++) {
    vector vec;
    VectorNew(&vec, elemSize, freefn, 4);
    h->buckets[i] = vec;
  }
}

void HashSetDispose(hashset *h)
{
  if (h->freefn != NULL) {
    for (int i = 0; i < h->numBuckets; i++) {
      for (int j = 0; j < h->buckets[i].logLength; j++) {
        h->freefn((char *)h->buckets[i].elems + j*h->elemSize);
      }
    }
  }

  for (int i = 0; i < h->numBuckets; i++) {
    VectorDispose(&h->buckets[i]); // since the individual buckets were dynamically allocated.
  }
}

int HashSetCount(const hashset *h)
{
  int count = 0;
  for (int i = 0; i < h->numBuckets; i++) {
    count = count + h->buckets[i].logLength;
  }
  
  return count;
}

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData)
{
  assert(mapfn != NULL);
  for (int i = 0; i < h->numBuckets; i++) {
    for (int j = 0; j < h->buckets[i].logLength; j++) {
      mapfn((char *)h->buckets[i].elems + j*h->elemSize, auxData);
    }
  }
}

void HashSetEnter(hashset *h, const void *elemAddr)
{
  int bucketNo = h->hashfn(elemAddr, h->numBuckets);
  assert(elemAddr != NULL && bucketNo >= 0 && bucketNo < h->numBuckets);

  int search = VectorSearch(&h->buckets[bucketNo], elemAddr, h->comparefn, 0, 0);
  if (search != -1) {
    VectorReplace(&h->buckets[bucketNo], elemAddr, search);
  } else {
    VectorAppend(&h->buckets[bucketNo], elemAddr);
  }
}

void *HashSetLookup(const hashset *h, const void *elemAddr)
{
  int bucketNo = h->hashfn(elemAddr, h->numBuckets);
  assert(elemAddr != NULL && bucketNo >= 0 && bucketNo < h->numBuckets);

  int search = VectorSearch(&h->buckets[bucketNo], elemAddr, h->comparefn, 0, 0);
  if (search != -1) {
    return (char *)(h->buckets[bucketNo].elems) + search*h->elemSize;
  }

  return NULL;
}
