// File created: December 27 2019
// written by moxniso
// vector.c, implementation of vectors for the C programming language.

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

int vector_total(vector* vec) 
{
  return vec->current_total;
}

vector* vector_init(size_t slimit)
{
  vector* vec = malloc(sizeof(vector));
  if (slimit < 0) vec->limit = 100;
  else vec->limit = slimit;
  
  vec->current_total = 0;
  vec->data = malloc(sizeof(void*) * vec->limit);
  return vec;
}

void vector_add(vector* vec, void* newdata) 
{
  if (vec->limit == vec->current_total) vector_resize(vec, vec->limit * 2);
  vec->data[vec->current_total] = newdata;
  vec->current_total++;
}

void vector_specific_delete(vector* vec, size_t where) 
{
  if (where < 0 || where > vec->limit) printf("Invalid index for vector data deletion!\n");
  else {
    vec->data[where] = NULL;
    vec->current_total--;
  }
}

void vector_range_delete(vector* vec, size_t start, size_t end) 
{
  for (int i = start;i <= end;i++)
                 vec->data[i] = NULL; 
                 
  vec->current_total -= (end-start);
}

void* vector_get(vector* vec, size_t where) 
{
  if (where < 0 || where > vec->limit) {
    printf("Invalid index for vector_get!\n");
    return NULL; 
  }
  else return vec->data[where];
}

void vector_set(vector* vec, size_t where, void* data) 
{
  if (where < 0 || where > vec->limit) printf("Invalid index for vector_set!\n");
  else {
     vec->data[where] = data;
     vec->current_total++;
  }
}
   
void vector_free(vector* vec) 
{
   if (vec) free(vec->data);
}

void vector_resize(vector* vec, size_t resized) 
{
  void** data = realloc(vec->data, sizeof(void*) * resized);
  if (data) {
    vec->data = data;
    vec->limit = resized;
  }
  else printf("Failed to resize vector!\n");
}
