#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef char element_t;

struct list {
  size_t capacity;
  size_t size;
  element_t values[];
};

struct list* list_create(size_t capacity)
{
  struct list* list = malloc(sizeof(struct list) + sizeof(element_t) * capacity);
  list->capacity = capacity;
  list->size = 0;
  return list;
}

void list_destroy(struct list* list)
{
  free(list);
}

void list_empty(struct list* list)
{
  list->size = 0;
}

bool list_is_empty(const struct list* list)
{
  return list->size == 0;
}

bool list_is_full(const struct list* list)
{
  return list->size == list->capacity;
}

void list_expand(struct list*, size_t);

void list_insert_back(struct list* list, element_t element)
{
  list->values[list->size++] = element;
}

void list_insert_at(struct list* list, size_t position, element_t element)
{
  for (int i = list->size; i > 0; --i)
    list->values[i] = list->values[i - 1];
  list->values[position] = element;
  ++list->size;
}

void list_remove_back(struct list* list)
{
  --list->size;
}

void list_remove_at(struct list* list, int position)
{
  for (int i = position; i < list->size; --i)
    list->values[i] = list->values[i + 1];
  --list->size;
}

