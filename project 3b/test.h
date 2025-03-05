//
// Created by Yotam Gaash on 20/02/2024.
//

#ifndef SNAKES_TEST_H
#define SNAKES_TEST_H
#include "markov_chain.h"
/**
 * Pointer to a function that deep-copies the object pointed to by src to the memory address pointed to by dest.
 * Assumes dest is already allocated with at least of size sizeof(*src);
 * On fail - returns 0, on success - returns 1.
 */
typedef int (*elem_copy_func_t) (void *dest, const void *src);

/**
 * Pointer to a function that frees all memory allocations performed by an elem_copy_func_t.
 */
typedef void (*elem_free_func_t) (void *to_free);

/**
 * This enum represents the possible direction for the list_to_array function you will implement.
 * The meaning of the two values will be explained in that function.
 */
typedef enum direction { FORWARD = 1, REVERSE = -1 } direction_t;


/**
 * node struct. A node in the doubly linked list.
 * @member data: pointer to a block of dynamically allocated memory.
 * @member prev: pointer to the previous node.
 * @member next: pointer to the next node.
 */
typedef struct node {
	void *data;
	struct node *prev;
	struct node *next;
} node_t;

/**
 * list struct. Representing a doubly linked list.
 * @member head: pointer to the head (first) node.
 * @member tail: pointer to the tail (last) node.
 * @member copy_func: pointer to a function that copies the content of one element to another.
 * @member free_func: pointer to a function that frees the content of an element.
 * @member elem_size: the size of an element in the linked list (number of bytes).
 */
typedef struct list {
	node_t *head;
	node_t *tail;
	elem_copy_func_t copy_func;
	elem_free_func_t free_func;
	size_t elem_size;
} list_t;

#endif //SNAKES_TEST_H
