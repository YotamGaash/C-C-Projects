//
// Created by Yotam Gaash on 02/05/2023.
//
#include <string.h>
#include <stdlib.h>
#include "markov_chain.h"

//#define MAX_WORD_LEN 102
#define MAX_TWEET_LEN 20
#define MIN_TWEET_LEN 2
#define ALLOC_ERROR_1 "allocation failure: in create_markov_chain - "\
"malloc failed for MarkovChain"
#define ALLOC_ERROR_2 "allocation failure: in in create_markov_chain - "\
"malloc failed for LinkedList"
#define ALLOC_ERROR_3 "allocation failure: in create_new_markov_node - "\
"malloc failed for MarkovNode"
#define ALLOC_ERROR_4 "allocation failure: in create_new_node - "\
"malloc failed for node"
#define ALLOC_ERROR_5 "allocation failure: in add_to_database failed"\
" to realloc database"
#define ALLOC_ERROR_6 "allocation failure: in add_node_to_frequencies_list- "\
"malloc failed for MarkovNodeFrequency"
#define ALLOC_ERROR_7 "allocation failure: in add_node_to_frequencies_list- "\
"realloc failed for the freq list"
#define ALLOC_ERROR_8 "allocation failure: in create_probability_list "\
"failed to create str list"
#define ALLOC_ERROR_9 "allocation failure: in create_probability_list "\
"failed to create sub list"

/**
 * frees the frequency list of a markov node.
 * @param markov_node the node to which the function sets the freq_list free.
 */
void free_frequencies_list(MarkovNodeFrequency *frequency_list, int size) {
    if (frequency_list == NULL) {
        return;
    }
    for (int i = 0; i < size; i++) {
        free(&frequency_list[i]);
    }
    free(frequency_list);
}


/**
 * free the memory of a markov node.
 * @param markov_node the node to set free.
 */
void free_markov_node(MarkovNode *markov_node) {
//    if (markov_node == NULL) {
//        return;
//    }
//    free_frequencies_list(markov_node->frequencies_list,
//                          markov_node->freq_list_size);
//    free(markov_node);
    if (markov_node == NULL) {
        return;
    }

    // free the frequencies_list and its associated elements
    for (int i = 0; i < markov_node->freq_list_size; i++) {
        free(markov_node->frequencies_list[i].markov_node);
    }
    if (markov_node->frequencies_list != NULL) {
        free(markov_node->frequencies_list);
    }

    free(markov_node);
}

/**
 * free the memory of a node.
 * @param node the node to set free.
 */
void free_node(Node *node) {
    if (node == NULL) {
        return;
    }
    if (node->data != NULL) {
        free(node->data);
    }
    free(node);
}


/**
 * free the memory of a a LinkedList.
 * @param node the linked list to set free.
 */
void free_linked_list(LinkedList *list) {
    if (list == NULL) {
        return;
    }
    Node *cur_node = list->first;
    while (cur_node != NULL) {
        Node *next_node = cur_node->next;
        free_node(cur_node);
        cur_node = next_node;
    }
//    if (list != NULL){
//        free(list);}
}

void free_markov_chain(MarkovChain *markov_chain){
    free_database(&markov_chain);

}
void generate_random_sequence(MarkovChain *markov_chain, MarkovNode
*first_node, int max_length){
    generate_tweet(markov_chain,first_node,max_length);

}


bool add_node_to_counter_list(MarkovNode *first_node, MarkovNode *second_node)
{
if (first_node->has_dot) {
// The freq list of a word with a dot at the end is empty
// so no need to add to the list.
return true;
}

// Check if the freq list is empty
if (first_node->freq_list_size == 0) {
first_node->frequencies_list = malloc(sizeof(MarkovNodeFrequency));
if (!first_node->frequencies_list) {
// Malloc failed
fprintf(stdout, ALLOC_ERROR_6);
return false;
}
first_node->frequencies_list[0] = (MarkovNodeFrequency)
{second_node,
1};
first_node->freq_list_size++;
return true;
}

// Check if the node is already in the freq list
for (int i = 0; i < first_node->freq_list_size; i++) {
if (first_node->frequencies_list[i].markov_node->data ==
second_node->data) {
first_node->frequencies_list[i].frequency++;
return true;
}
}

// Adding the second node to the freq list and reallocating memory
int new_size = first_node->freq_list_size + 1;
MarkovNodeFrequency *new_freq_list =
        realloc(first_node->frequencies_list,
                new_size *
                sizeof(MarkovNodeFrequency));
if (!new_freq_list) {
// Failed to reallocate
fprintf(stdout, ALLOC_ERROR_7);
return false;
}

new_freq_list[new_size - 1] = (MarkovNodeFrequency)
{second_node, 1};
first_node->frequencies_list = new_freq_list;
first_node->freq_list_size = new_size;

return true;
}

/**
 * creates an empty markov chain
 * the size of memory to be allocated to the markov chan
 * @return
 */
MarkovChain *create_markov_chain() {
    MarkovChain *markov_chain = malloc(sizeof(MarkovChain));
    if (markov_chain == NULL) { //failed to alloc memory to the markov_chain
        fprintf(stdout, ALLOC_ERROR_1);
        return NULL;
    }
    markov_chain->database = malloc(sizeof(LinkedList));
    if (markov_chain->database == NULL) {
        free_database(&markov_chain);
        if (markov_chain->database != NULL) {
            free(markov_chain->database);
        }
        fprintf(stdout, ALLOC_ERROR_2);
        return NULL;
    }
    markov_chain->database->size = 0;
    markov_chain->database->first = NULL;
    markov_chain->database->last = NULL;
    return markov_chain;
}

/**
 * creates a new markov node, returns NULL if it fails to allocate memory
 * @param data_ptr the data string for the node
 * @return a pointer to the new node or NULL
 */
MarkovNode *create_new_markov_node(char *data_ptr) {
    MarkovNode *new_markov_node = malloc((sizeof(MarkovNode)));
    if (new_markov_node == NULL) {
        fprintf(stdout, ALLOC_ERROR_3);
        return NULL;
    }
    strcpy(new_markov_node->data, data_ptr);
    new_markov_node->freq_list_size = 0;
    new_markov_node->frequencies_list = NULL;
    if (data_ptr[strlen(data_ptr) - 1] ==
        '.') //checks if the word ends with a dot
    {
        new_markov_node->has_dot = true; //words that end a sentence
        // have null freq list
    } else {
        new_markov_node->has_dot = false;
    }
    return new_markov_node;
}

/**
 * creates a new node, returns NULL if it fails to allocate memory
 * @param data_ptr the data string for the node
 * @return a pointer to the new node or NULL
 */
Node *create_new_node(char *data_ptr) {

    MarkovNode *new_markov_node = create_new_markov_node(data_ptr);
    if (new_markov_node == NULL) //returning null in case of memory allocation
        // failure
    {
        return NULL;
    }
    //wrapping the markov node in a Node
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) { //failed to allocate memory
        fprintf(stdout, ALLOC_ERROR_4);
        free(new_markov_node->frequencies_list);
        free_markov_node(new_markov_node);
        return NULL;
    }
    new_node->data = new_markov_node;
    new_node->next = NULL;
    return new_node;
}


/**
* If data_ptr in markov_chain, return it's node. Otherwise, create new
 * node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's database,
 * returns NULL in case of memory allocation failure.
 */
Node *add_to_database(MarkovChain *markov_chain, char *data_ptr) {

    Node *node = get_node_from_database(markov_chain, data_ptr);
    if (node != NULL) //checking if the node is in the data base already
    {
        return node;
    }

    // increasing the size of the database by 1
    markov_chain->database = realloc(markov_chain->database,
                                     ((1 + markov_chain->database->size) *
                                      sizeof(LinkedList)));
    if (markov_chain->database == NULL) //failed to realloc
    {
        fprintf(stdout, ALLOC_ERROR_5);
        free(node);
        return NULL;
    }
    //creating a markov node to wrap the data_ptr
    MarkovNode *markov_node = create_new_markov_node(data_ptr);
    if (markov_node == NULL) {
        free(node);
        return NULL;
    }
    if (add(markov_chain->database, markov_node->data) == 1) {
        free(markov_node);
        return NULL;
    }

    markov_chain->database->last->data = markov_node;
    return markov_chain->database->last;
}


/**
* Check if data_ptr is in database. If so, return the markov_node
 * wrapping it in the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */
Node *get_node_from_database(MarkovChain *markov_chain, char *data_ptr) {
    Node *cur_node = markov_chain->database->first;
    while (cur_node != NULL) //running along the database list to check if
        //the node is there
    {

        if (strcmp(cur_node->data->data, data_ptr) == 0) {
            return cur_node;
        }
        cur_node = cur_node->next;
    }
    free_node(cur_node);
    return NULL; //the node is not in the database
}

/**
 * Add the second markov_node to the frequency list of the first markov_node.
 * If already in list, update it's occurrence frequency value.
 * @param first_node
 * @param second_node
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool
add_node_to_frequencies_list(MarkovNode *first_node, MarkovNode *second_node){

    if (first_node->has_dot) {
        // The freq list of a word with a dot at the end is empty
        // so no need to add to the list.
        return true;
    }

    // Check if the freq list is empty
    if (first_node->freq_list_size == 0) {
        first_node->frequencies_list = malloc(sizeof(MarkovNodeFrequency));
        if (!first_node->frequencies_list) {
            // Malloc failed
            fprintf(stdout, ALLOC_ERROR_6);
            return false;
        }
        first_node->frequencies_list[0] = (MarkovNodeFrequency)
                {second_node,
                 1};
        first_node->freq_list_size++;
        return true;
    }

    // Check if the node is already in the freq list
    for (int i = 0; i < first_node->freq_list_size; i++) {
        if (first_node->frequencies_list[i].markov_node->data ==
            second_node->data) {
            first_node->frequencies_list[i].frequency++;
            return true;
        }
    }

    // Adding the second node to the freq list and reallocating memory
    int new_size = first_node->freq_list_size + 1;
    MarkovNodeFrequency *new_freq_list =
            realloc(first_node->frequencies_list,
                    new_size *
                    sizeof(MarkovNodeFrequency));
    if (!new_freq_list) {
        // Failed to reallocate
        fprintf(stdout, ALLOC_ERROR_7);
        return false;
    }

    new_freq_list[new_size - 1] = (MarkovNodeFrequency)
            {second_node, 1};
    first_node->frequencies_list = new_freq_list;
    first_node->freq_list_size = new_size;

    return true;
}


/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_database(MarkovChain **ptr_chain) {

//    if (*ptr_chain == NULL) {
//        return; //empty list
//    }
//
//    free_linked_list((*ptr_chain)->database);
//    free(*ptr_chain);
    if (*ptr_chain == NULL) {
        return;
    }
    LinkedList *database = (*ptr_chain)->database;
    for (Node *node = database->first; node != NULL;) {
        Node *temp = node;
        node = node->next;
        free_node(temp);
    }
    free(database);
    free(*ptr_chain);
}


/**
 * Gets random number between 0 and max_number
 * @param max_number the maximum random number to return
 * @return Random number
*/
int get_random_number(int max_number) {
    return rand() % max_number;
}

/**
* Get one random state from the given markov_chain's database.
* @param markov_chain
* @return
*/
MarkovNode *get_first_random_node(MarkovChain *markov_chain) {
    bool valid_word = false;
    Node *cur_node = markov_chain->database->first;
    while (!valid_word) {
        int max_number = markov_chain->database->size;
        int random_number = get_random_number(max_number);
        cur_node = markov_chain->database->first;
        for (int i = 0; i < random_number; i++) {
            cur_node = cur_node->next;
        }
        if (!cur_node->data->has_dot) {
            valid_word = true;
        }
    }
    return cur_node->data;
}

/**
 * receives a frequency list and calculate the sum of all the frequencies
 * of the nodes in the list
 * @param markov_node a markov node
 * @return the total sum of frequencies
 */
int get_total_frequency(MarkovNode *markov_node) {
    int total_frequency = 0;
    for (int i = 0; i < markov_node->freq_list_size; i++) {
        total_frequency += markov_node->frequencies_list[i].frequency;
    }
    return total_frequency;
}

/**
 * recieves a markov node and creates a dynamic list of strings
 * that contains number of elements equal to the freq_list_size
 * @param markov_node a markov node
 * @return a list of strings
 */
char **create_probability_list(MarkovNode *markov_node) {
    int total_freq_num = get_total_frequency(markov_node);
    // allocating memory for the list
    char **probability_list = malloc(total_freq_num * sizeof(char *));
    if (probability_list == NULL) {
        fprintf(stdout, ALLOC_ERROR_8);
        return NULL;
    }
    for (int i = 0; i < total_freq_num; i++) {
        probability_list[i] = malloc(MAX_WORD_LEN * sizeof(char));
        if (probability_list[i] == NULL) {
            fprintf(stdout, ALLOC_ERROR_9);
            for (int j = 0; j < i; j++) {
                free(probability_list[j]);
            }
            free(probability_list);
            return NULL;
        }
    }

    int current_cell = 0;
    for (int i = 0; i < markov_node->freq_list_size; i++) {
        char *current_word = markov_node->frequencies_list[i]
                .markov_node->data;
        int word_freq = markov_node->frequencies_list[i].frequency;
        for (int j = 0; j < word_freq; j++) {
            strcpy(probability_list[current_cell], current_word);
            current_cell++;
        }
    }
    return probability_list;
}


/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode *get_next_random_node(MarkovNode *state_struct_ptr) {
    if (state_struct_ptr->frequencies_list == NULL) {
        return NULL; // empty freq list
    }
    int total_freq = get_total_frequency(state_struct_ptr);
    int random_num = get_random_number(total_freq);
    char **prob_list = create_probability_list(state_struct_ptr);
    if (prob_list == NULL) {
        return NULL;
    }
    char random_word[MAX_WORD_LEN];
    strcpy(random_word, prob_list[random_num]);

    for (int i = 0; i < total_freq; i++) {
        free(prob_list[i]);
    }
    free(prob_list);

    for (int i = 0; i < total_freq; i++) {
        if (strcmp(state_struct_ptr->frequencies_list[i].markov_node->data,
                   random_word) == 0) {
            return state_struct_ptr->frequencies_list[i].markov_node;
        }
    }
    return NULL;

}

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain
 * @param first_node markov_node to start with,
 *                   if NULL- choose a random markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_tweet(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length) {
    while (first_node == NULL) {
        first_node = get_first_random_node(markov_chain);
    }
    MarkovNode *current_node = first_node;
    if (max_length > MAX_TWEET_LEN) {
        max_length = MAX_TWEET_LEN;
    }
    for (int i = 0; i < max_length; i++) {
        if (current_node == NULL) {
            fprintf(stdout, "\n");
            return;
        }
        if (current_node->has_dot) {
            fprintf(stdout, "%s\n", current_node->data);
            return;
        }
        if (i == max_length - 1) { // adding \n at the end of the tweet
            fprintf(stdout, "%s\n", current_node->data);
        } else {
            fprintf(stdout, "%s ", current_node->data);
        }
        current_node = get_next_random_node(current_node);
    }
}




