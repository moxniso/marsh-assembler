// File created: December 29 2019
// written by moxniso
// vector.h, prototypes for vector.c

#ifndef VECTOR_H
#define VECTOR_H

typedef struct dynamic_vector {
  void** data;
  size_t limit;
  size_t current_total;
} vector;

vector* vector_init(size_t slimit);
int vector_total(vector* vec);
void vector_resize(vector* vec, size_t resized);
void vector_add(vector* vec, void* newdata);
void vector_set(vector* vec, size_t where, void* data);
void* vector_get(vector* vec, size_t where);
void vector_specific_delete(vector* vec, size_t where);
void vector_range_delete(vector* vec, size_t start, size_t end);
void vector_free(vector* vec);

#endif
