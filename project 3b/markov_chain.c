//
// Created by Yotam Gaash on 02/05/2023.
//
#include <string.h>
#include <stdlib.h>
#include "markov_chain.h"


/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode *get_next_random_node(MarkovNode *state_struct_ptr) {

    if (state_struct_ptr == NULL || state_struct_ptr->counter_list == NULL) {
        return NULL; // invalid node
    }
    int total_freq = get_total_frequency(state_struct_ptr);
    int random_num = rand() % total_freq, count = 0;
    for(int i =0; i< state_struct_ptr->counter_list_size;i++){
        if (random_num <= count + state_struct_ptr->counter_list[i].counter){
            return state_struct_ptr->counter_list[i].markov_node;
        }
        count += state_struct_ptr->counter_list[i].counter;
    }
    return NULL;
}

MarkovNode *get_first_random_node(MarkovChain *markov_chain) {
    bool valid_node = false;
    Node *cur_node = markov_chain->database->first;
    while (!valid_node) {
        int max_number = markov_chain->database->size;
        int random_number = rand() % max_number;
        cur_node = markov_chain->database->first;
        for (int i = 0; i < random_number; i++) {
            cur_node = cur_node->next;
        }
        // the first node cant be a last state
        valid_node = !markov_chain->is_last(cur_node);
    }
    return cur_node->data;
}
bool add_node_to_counter_list(MarkovNode *first_node,
                              MarkovNode *second_node,
                              MarkovChain *markov_chain) {
    // Check if the first node is the last state
    if (markov_chain->is_last(first_node->data)) {
        return true; // Last state node has an empty counter list
    }

    // Check if the counter list is empty
    if (first_node->counter_list_size == 0) {
        // Allocate memory for the new counter list
        first_node->counter_list = malloc(sizeof(NextNodeCounter));
        if (!first_node->counter_list) {
            fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
            return false;
        }

        // Initialize the first element in the counter list
        first_node->counter_list[0] = (NextNodeCounter){
            second_node, 1};
        first_node->counter_list_size++;
        return true;
    }

    // Check if the node is already in the frequency list
    for (int i = 0; i < first_node->counter_list_size; i++) {
        if (markov_chain->comp_func(
                first_node->counter_list[i].markov_node->data,
                second_node->data) == 0) {
            // Node already exists, update its counter value
            first_node->counter_list[i].counter++;
            return true;
        }
    }

    // Node is not in the list, reallocate memory for the updated counter list
    NextNodeCounter *temp_list =
            realloc(first_node->counter_list,
                    (first_node->counter_list_size + 1)
                    * sizeof(NextNodeCounter));
    if (!temp_list) {
        // Failed to reallocate
        fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
        return false;
    }

    first_node->counter_list = temp_list;
    // Add the new entry to the end of the counter list
    first_node->counter_list[first_node->counter_list_size] =
            (NextNodeCounter){second_node, 1};
    first_node->counter_list_size++;
    return true;
}

void free_markov_chain(MarkovChain **markov_chain) {
    if (markov_chain == NULL || *markov_chain == NULL) {
        return; // empty chain
    }
    // Free the database using the designated free function
    if ((*markov_chain)->database != NULL) {
        Node *current_node = (*markov_chain)->database->first;
        while (current_node != NULL) {
            Node *next_node = current_node->next;

            // Free the counter list
            free(current_node->data->counter_list);

            // Free the data
            (*markov_chain)->free_data(current_node->data->data);

            // Free the markov_node structure
            free(current_node->data);

            // Free the node
            free(current_node);

            current_node = next_node;
        }

        // Free the database structure
        free((*markov_chain)->database);
    }

    // Free the markov_chain structure
    free(*markov_chain);
    *markov_chain = NULL; // Set the pointer to NULL
}


Node *get_node_from_database(MarkovChain *markov_chain, void *data_ptr) {
    if (markov_chain == NULL || markov_chain->database == NULL || data_ptr
    == NULL) {
        return NULL; // Invalid chain or database
    }

    Node *current_node = markov_chain->database->first;
    while (current_node != NULL) {
        // Use the comparison function to check if data_ptr matches
        // the current node's data
        if (markov_chain->comp_func(current_node->data->data, data_ptr) == 0) {
            return current_node; // Node found
        }
        current_node = current_node->next;
    }

    return NULL; // Node not found in the database
}

Node *add_to_database(MarkovChain *markov_chain, void *data_ptr) {
    if (markov_chain == NULL || markov_chain->database == NULL) {
        return NULL; // Invalid chain or database
    }

    // Check if the data is already in the database using get node from
    // database function
    Node *existing_node = get_node_from_database(markov_chain, data_ptr);
    if (existing_node != NULL) {
        return existing_node; // Node already exists in the database
    }

    // adding node to data base using add function for linked lists
    void *data = markov_chain->copy_func(data_ptr);
    if (data == NULL) {
        fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
        return NULL;
    }
    MarkovNode *markov_node = make_markov_node(data);
    if (markov_node == NULL) {
        fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
        free(data);
        return NULL;
    }
    if (add(markov_chain->database, markov_node)) {
        //adding to database failed
        fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
        free(data);
        return NULL;
    }
    return markov_chain->database->last;
}

int get_total_frequency(MarkovNode *markov_node) {
    int total_frequency = 0;
    for(int i=0; i< markov_node->counter_list_size;i++){
        total_frequency += markov_node->counter_list[i].counter;
    }
    return total_frequency;
}

void generate_random_sequence(MarkovChain *markov_chain,
                              MarkovNode *first_node, int max_length) {
    if (markov_chain == NULL) {
        fprintf(stdout, ERROR_NULL_INPUT);
        return;
    }
    max_length = max_length < MIN_SEQ_LEN ? MIN_SEQ_LEN : max_length;
    // If first_node is NULL, choose a random starting node from the database
    MarkovNode *current_node = (first_node != NULL) ? first_node :
            get_first_random_node(markov_chain);

    markov_chain->print_func(current_node->data);  // Print the starting node

    int length = 1;

    // Generate the random sequence
    while (length < max_length) {
        MarkovNode *next_node = get_next_random_node(current_node);
        if (next_node == NULL) {
            break;  // No valid next node found
        }
        markov_chain->print_func(next_node->data);
        current_node = next_node;
        length++;
        // Check if the sequence has reached an ending state
        if (markov_chain->is_last(current_node->data)) {
            break;
        }
    }
}
MarkovNode *make_markov_node(void *data) {
    MarkovNode *markov_node = malloc(sizeof(MarkovNode));
    if (!markov_node) {
        fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
        return NULL;
    }
    markov_node->data = data;
    markov_node->counter_list = NULL;
    markov_node->counter_list_size = 0;
    return markov_node;
};

MarkovChain *make_markov_chain(PrintFunc print_func,
                               CompFunc comp_func,
                               FreeFunc free_data,
                               CopyFunc copy_func,
                               IsLastFunc is_last) {
    // Allocate memory for the MarkovChain structure
    MarkovChain *markov_chain = malloc(sizeof(MarkovChain));
    if (markov_chain == NULL) {
        fprintf(stdout, ALLOC_ERROR_MC);
        return NULL;
    }

    // Initialize the components of the MarkovChain
    markov_chain->database = malloc(sizeof(LinkedList));
    if (markov_chain->database == NULL) {
        fprintf(stderr, ALLOC_ERROR_LS);
        free_markov_chain(&markov_chain);
        return NULL;
    }

    markov_chain->database->first = NULL;
    markov_chain->database->last = NULL;
    markov_chain->database->size = 0;

    markov_chain->print_func = print_func;
    markov_chain->comp_func = comp_func;
    markov_chain->free_data = free_data;
    markov_chain->copy_func = copy_func;
    markov_chain->is_last = is_last;

    return markov_chain;
}





