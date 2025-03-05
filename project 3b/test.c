//
// Created by Yotam Gaash on 20/02/2024.
//
#include "test.h"
/**
 * Initializes an empty linked list.
 * @param elem_size The size of the elements the list will hold.
 * @param copy_func pointer to a function that copies the content of one element to another.
 * @param free_func pointer to a function that frees the content of an element.
 * @return A pointer to an empty, dynamically allocated list_t with the given arguments set in the fields.
 */
list_t *list_new (size_t elem_size, elem_copy_func_t copy_func, elem_free_func_t free_func)
{
	list_t *new_list = (list_t *)malloc(sizeof(list_t));

	if (new_list == NULL) {
		// Allocation failed, return NULL
		return NULL;
	}

	// Initialize the list fields
	new_list->head = NULL;
	new_list->tail = NULL;
	new_list->elem_size = elem_size;
	new_list->copy_func = copy_func;
	new_list->free_func = free_func;

	return new_list;
}

/**
 * Adds a new element to the list before the given node.
 * @param list the list to add the element to
 * @param node the node to add the element before. If NULL - add the new element as tail.
 * @param data the element to copy to the list.
 * @return pointer to the new dynamically allocated node
 */
node_t *list_insert_before (list_t *list, node_t *node, void *data)
{
	node_t *new_node = (node_t *)malloc(sizeof(node_t));

	if (new_node == NULL) {
		// Allocation failed, return NULL
		return NULL;
	}

	// Allocate memory for the data in the new node
	new_node->data = malloc(list->elem_size);

	if (new_node->data == NULL) {
		// Allocation failed, free the node and return NULL
		free(new_node);
		return NULL;
	}

	// Copy data to the new node using the provided copy function
	if (list->copy_func(new_node->data, data) == 0) {
		// Copying failed, free allocated memory and return NULL
		free(new_node->data);
		free(new_node);
		return NULL;
	}
	// Update pointers in the new node
	new_node->prev = NULL;
	new_node->next = NULL;

	// Update pointers in the list to insert the new node
	if (node == NULL) {
		// Insert as tail
		if (list->tail == NULL) {
			// List is empty
			list->head = new_node;
			list->tail = new_node;
		} else {
			new_node->prev = list->tail;
			list->tail->next = new_node;
			list->tail = new_node;
		}
	} else {
		// Insert before the specified node
		new_node->prev = node->prev;
		new_node->next = node;

		if (node->prev == NULL) {
			// Inserting at the head
			list->head = new_node;
		} else {
			node->prev->next = new_node;
		}

		node->prev = new_node;
	}

	return new_node;
}

void *list_to_array(list_t *list, direction_t dir) {
	// Check if the list is empty
	if (list->head == NULL) {
		return NULL;
	}

	// Calculate the number of elements in the list
	size_t list_size = 0;
	node_t *current = (dir == FORWARD) ? list->head : list->tail;

	while (current != NULL) {
		list_size++;
		current = (dir == FORWARD) ? current->next : current->prev;
	}

	// Allocate memory for the array
	void *array = malloc(list_size * list->elem_size);

	if (array == NULL) {
		// Allocation failed, return NULL
		return NULL;
	}

	// Copy elements from the list to the array based on the specified direction
	current = (dir == FORWARD) ? list->head : list->tail;
	void *array_ptr = array;

	while (current != NULL) {
		// Copy element data to the array using the copy function
		if (list->copy_func(array_ptr, current->data) == 0) {
			// Copying failed, free allocated memory and return NULL
			free(array);
			return NULL;
		}

		array_ptr += list->elem_size;
		current = (dir == FORWARD) ? current->next : current->prev;
	}

	return array;
}

/** Number of courses */
#define NUM_COURSES 5

/** enum for courses */
typedef enum courses_idx {
	CPP, NAND, INFI, LINALG, OS
} courses_idx_t;

/**
 * struct for response.
 * @member id: Identifier for the response. Must be changeable
 * @member content: The content of the response. Must be changeable
 * @member score: The score the responder gave to the course.
 */
typedef struct response {
	char *id;
	char *content; //must be changeable
	double score; //must be changeable
} response_t;

/**
 * struct for managing the responses for NUM_COURSES courses.
 * @member courses: static array of linked lists, each should holds the responses for one course
 */
typedef struct response_manager {
	list_t *courses[NUM_COURSES];
} response_manager_t;

#include <stdio.h>
#include <string.h>

/**
 * Initializes a response manager.
 * @return A pointer to a dynamically allocated response_manager_t.
 */

/**
 * Adds a response to the specified course in the response manager.
 * @param manager The response manager.
 * @param course The course index.
 * @param new_response The response to add.
 * @return 1 on success, 0 on failure.
 */
/**
 * Function for your tests. Leave empty on final submission.
 */
void my_tests()
{
}

int response_copy(void *dest, const void *src) {
	const response_t *source = (const response_t *)src;
	response_t *destination = (response_t *)dest;

	// Allocate memory for id and content and copy data
	destination->id = (char *)malloc(strlen(source->id) + 1);
	destination->content = (char *)malloc(strlen(source->content) + 1);

	if (destination->id == NULL || destination->content == NULL) {
		// Allocation failed, free already allocated memory and return failure
		free(destination->id);
		free(destination->content);
		return 0;
	}

	strcpy(destination->id, source->id);
	strcpy(destination->content, source->content);

	// Copy the score
	destination->score = source->score;

	return 1; // Return success
}

void response_free(void *to_free) {
	response_t *response = (response_t *)to_free;

	// Free the dynamically allocated memory in the response struct
	free(response->id);
	free(response->content);

	// No need to free response itself, as it is part of the dynamically allocated node in the linked list
}

}
/**
 * Dynamically allocates a new response manager.
 * Must initialize empty vectors for each course, with matching copy and free functions for the responses.
 * @return Dynamically allocated response manager
 */
response_manager_t *response_manager_new() {
	response_manager_t *manager = (response_manager_t *)malloc(sizeof(response_manager_t));

	if (manager == NULL) {
		// Allocation failed, return NULL
		return NULL;
	}

	// Initialize each course's linked list with copy and free functions
	for (int i = 0; i < NUM_COURSES; i++) {
		manager->courses[i] = list_new(sizeof(response_t), &response_copy, &response_free);
		if (manager->courses[i] == NULL) {
			// Initialization failed, free already allocated resources and return NULL
			for(int j = i; j >= 0; j--){
				list_free(&manager->courses[j]);
				free(manager->courses[j])
			}
			free(&manager);
			return NULL;
		}

	}

	return manager;
}
