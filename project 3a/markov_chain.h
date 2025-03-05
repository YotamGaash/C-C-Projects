#ifndef _MARKOV_CHAIN_H_
#define _MARKOV_CHAIN_H_

#include "linked_list.h"
#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For exit(), malloc()
#include <stdbool.h> // for bool

#define MAX_WORD_LEN 101



/**
 * insert Structs here
 */

/**
 * A Markov chain that contains a LinkedList of Nodes
 * @param database a linked list of nodes
 * @param capacity the current max size of memory allocated to the database
 */
typedef struct MarkovChain {
    LinkedList *database;
    int capacity;
} MarkovChain;

/**
 * a Markov node that represent a word in the markov chain
 * @param data the string of the word
 * @param frequencies_list a list of all the words that come after
 *  the current word in the text
 *  @param freq_list_size the amount of words in the frequencies_list
 *  @param has_dot true if the word end with a dot, false otherwise
 */
typedef struct MarkovNode {
    char data[MAX_WORD_LEN];
    struct MarkovNodeFrequency *frequencies_list;
    int freq_list_size; //the number of words in the frequencies list
    bool has_dot;
} MarkovNode;

/**
 * contains a pointer to a markov node and the number of time it appears
 * in the text after a specific node
 */
typedef struct MarkovNodeFrequency{
    MarkovNode *markov_node;
    int frequency;
} MarkovNodeFrequency;

/**
 * creates an empty markov chain
 * @return
 */
MarkovChain *create_markov_chain();


/**
 * creates a new node, returns NULL if it fails to allocate memory
 * @param data_ptr the data string for the node
 * @return a pointer to the new node or NULL
 */
Node *create_new_node(char *data_ptr);


/**
* If data_ptr in markov_chain, return it's node. Otherwise, create new
 * node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's database,
 * returns NULL in case of memory allocation failure.
 */
Node* add_to_database(MarkovChain *markov_chain, char *data_ptr);

/**
* Check if data_ptr is in database. If so, return the markov_node
 * wrapping it in the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */
Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr);

/**
 * Add the second markov_node to the frequency list of the first markov_node.
 * If already in list, update it's occurrence frequency value.
 * @param first_node
 * @param second_node
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool add_node_to_frequencies_list(MarkovNode *first_node,
                                  MarkovNode *second_node);

/**
 * free the memory of a markov node.
 * @param markov_node the node to set free.
 */
void free_markov_node( MarkovNode *markov_node);

/**
 * free the memory of a node.
 * @param node the node to set free.
 */
void free_node(Node *node);

/**
 * frees a frequency list of a markov node.
 * @param markov_node the first node to set free.
 */
void free_frequencies_list(MarkovNodeFrequency *frequency_list, int size);
/**
 * free the memory of a a LinkedList.
 * @param node the linked list to set free.
 */
void free_linked_list(LinkedList *list);

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_database(MarkovChain ** ptr_chain);

/**
 * free the memory of a markov chain and all its nodes.
 * @param markov_chain the markov chain to set free.
 */
void free_markov_chain(MarkovChain *markov_chain);

/**
 * receives a frequency list and calculate the sum of all the frequencies
 * of the nodes in the list
 * @param freq_list
 * @return the total sum of frequencies
 */
int get_total_frequency(MarkovNode *marcov_node);

/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain
 * @return
 */
MarkovNode* get_first_random_node(MarkovChain *markov_chain);

/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr);

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain
 * @param first_node markov_node to start with,
 *                   if NULL- choose a random markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_tweet(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length);


#endif /* _MARKOV_CHAIN_H_ */
