#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef char element_t;

struct list {
  size_t capacity;
  size_t size;
  element_t *values;
};

struct list* list_create(size_t capacity)
{
  struct list* list = malloc(sizeof(struct list));
  list->capacity = capacity;
  list->size = 0;
  list->values = malloc(capacity * sizeof(element_t));
  return list;
}

void list_destroy(struct list* list)
{
  free(list->values);
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
  return false;
}

void list_expand(struct list* list, size_t new_capacity)
{
  element_t* new_values = malloc(new_capacity * sizeof(element_t));
  for (size_t i = 0; i < list->size; ++i)
    new_values[i] = list->values[i];
  free(list->values);

  list->values = new_values;
  list->capacity = new_capacity;
}

void list_insert_back(struct list* list, element_t element)
{
  if (list->size == list->capacity)
    list_expand(list, 2 * list->capacity);
  list->values[list->size++] = element;
}

void list_insert_at(struct list* list, size_t position, element_t element)
{
  list_insert_back(list, element);
  for (size_t i = list->size; i > 0; --i)
  {

  }
    list->values[i] = list->values[i - 1];
  list->values[position] = element;

  ++list->size;

  for (size_t )
}

void list_remove_back(struct list* list)
{
  --list->size;
}

void list_remove_at(struct list* list, size_t position)
{
  for (size_t i = position; i < list->size; --i)
    list->values[i] = list->values[i + 1];
  --list->size;
}

