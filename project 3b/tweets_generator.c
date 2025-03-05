//
// Created by Yotam Gaash on 02/05/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "linked_list.h"
#include "markov_chain.h"

typedef char* String;
// macros
#define ARGS_NUM_V1 4
#define ARGS_NUM_V2 5

#define MAX_WORD_LEN 101
#define MAX_LINE_LEN 1001
#define BASE 10
#define SEED_ARG 1
#define TWEET_NUM_ARG 2
#define FILE_PATH_ARG 3
#define  MAX_TWEET_LENGTH 20
#define WORDS_TO_READ_ARG 4
#define MAX_WORDS_TO_READ 2147483647
//error massages
#define ERROR_PATH "Error: could not open the file or find the file path\n"
#define ERROR_ARGS_NUM "Usage:<seed> <tweets_num> <path> (opt)<read_num>\n"
#define ERROR_MEMORY "ERROR: failed to allocate/reallocate memory\n"

#define TWEET_NUMBER "Tweet %d: "

/***************************/
/* Function Prototypes    */
/***************************/

static bool is_last_state(void *data);

static void print_func(void *data);

static int compare_func(void *data1, void *data2);

static void free_func(void *data);

static void *copy_string(void *data);

static int fill_database(FILE *fp, int words_to_read,
                         MarkovChain *markov_chain);

/****************************************************/
/* Markov Chain Functions for string data types.    */
/****************************************************/

// Check if a string is the last state (assuming it ends with a '.')
static bool is_last_state(void *data) {
    if (data == NULL) {
        return false;
    }
    size_t length = strlen((char *) data);

    // Check if the last character is '.'
    return length > 0 &&((char *) data)[length - 1] == '.';
}

// Print function for strings
static void* copy_string(void* data) {
    if (data == NULL) {
        return NULL;  // Handle NULL case if needed
    }
    char *copy = malloc(strlen((char*)data) + 1);
    strcpy(copy, (char*)data);
    return copy;
}

//    // Allocate memory for the new string
//    char* copied_string = (char*)malloc(strlen((char*)data) + 1);
//
//    if (copied_string != NULL) {
//        // Copy the content of the original string to the new memory
//        strcpy(copied_string, (char*)data);
//    }

static void print_func(void *data) {
    if (data == NULL) {
        printf("\n");
    } else {
        String string_data = (char*) data;
        if (is_last_state(data)) {
            printf("%s\n", string_data);
        } else {
            printf("%s ", string_data);
        }
    }
}

// Comparison function for strings
static int compare_func(void *data1, void *data2) {
    if (data1 == NULL || data2 == NULL) {
        return 0;
    }
    return strcmp((char *) data1, (char *) data2);
}

// Free function for strings
void free_func(void *data) {
    free(data);
}


/**
 * this functions creates a Markov chain out of a txt file
 * @param fp the path to the txt file
 * @param words_to_read the number of words to read from the file
 * @param markov_chain The markov chain to which the words will be added
 * @return 0 is succeeded, 1 otherwise
 */
static int fill_database(FILE *fp, int words_to_read,
                         MarkovChain *markov_chain) {
    if (fp == NULL || markov_chain == NULL || words_to_read < 1) {
        //invalid parameters
        return EXIT_FAILURE;
    }
    char line[MAX_LINE_LEN];
    // Reading one line at a time and splitting it using strtok
    while (fgets(line, MAX_LINE_LEN, fp) != NULL &&
    words_to_read > 0) {
        char *current_word = strtok(line, " \n");//Tokenizing the line
        char *previous_word = NULL;

        while (current_word != NULL && words_to_read > 0) {
            Node *node = add_to_database(markov_chain, current_word);
            if (node == NULL) {
                fprintf(stdout, ERROR_MEMORY);
                free_markov_chain(&markov_chain);
                return EXIT_FAILURE;
            }

            if (previous_word != NULL) {
                // Adding the current word to the list of the previous word
                Node *previous_node =
                        get_node_from_database(markov_chain,
                                               previous_word);
                if (previous_node != NULL) {
                    if (!add_node_to_counter_list(previous_node->data,
                                                  node->data,
                                                  markov_chain)) {
                        fprintf(stdout, ERROR_MEMORY);
                        free_markov_chain(&markov_chain);
                        return EXIT_FAILURE;
                    }
                }
            }

            previous_word = current_word;
            current_word = strtok(NULL, " \n");
            words_to_read--;
        }
    }

    return 0;
}

static void generate_tweet(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length) {
    while (first_node == NULL) {
        first_node = get_first_random_node(markov_chain);
    }
    MarkovNode *current_node = first_node;
    if (max_length > MAX_TWEET_LENGTH) {
        max_length = MAX_TWEET_LENGTH;
    }
    for (int i = 0; i < max_length; i++) {
        if (current_node == NULL) {
            fprintf(stdout, "\n");
            return;
        }
        if (is_last_state(current_node->data)) {
            fprintf(stdout, "%s\n", (char*)current_node->data);
            return;
        }
        if (i == max_length - 1) { // adding \n at the end of the tweet
            fprintf(stdout, "%s\n", (char*)current_node->data);
        } else {
            fprintf(stdout, "%s ", (char*)current_node->data);
        }
        current_node = get_next_random_node(current_node);
    }
}
/**
 * prints all the contents of a markov node
 * @param markov_node
 */
void print_markov_node(MarkovNode *markov_node) {
    printf("\n %s -> [ ", (char*)markov_node->data);
    for (int i = 0; i < markov_node->counter_list_size; i++) {
        printf("(%d,%s) ", markov_node->counter_list[i].counter,
               (char*)markov_node->counter_list[i].markov_node->data);
        if (i != markov_node->counter_list_size - 1) {
            printf(",");
        }
    }
    printf(" ]\n");
}

/**
 * Prints the given MarkovChain to stdout.
 *
 * @param markov_chain The MarkovChain to print
 */
void print_markov_chain(MarkovChain *markov_chain) {
    // Traverse the linked list and print the words
    Node *current_node = markov_chain->database->first;
    printf("%s", (char*)current_node->data->data);
    while (current_node->next != NULL) {
        current_node = current_node->next;
        printf(" -> %s", (char*)current_node->data->data);
    }
    printf("\n");

    // Print the frequency lists for each word
    current_node = markov_chain->database->first;
    while (current_node != NULL) {
        print_markov_node(current_node->data);
        current_node = current_node->next;
    }
    printf("\n");

}

static MarkovChain *create_and_populate_chain(FILE *fp, int words_to_read) {
    MarkovChain *markov_chain = make_markov_chain(print_func,
                                                  compare_func,
                                                  free_func,
                                                  copy_string,
                                                  is_last_state);

    if (markov_chain == NULL) {
        return NULL;
    }

    if (fill_database(fp, words_to_read, markov_chain) != 0) {
        free_markov_chain(&markov_chain);
        return NULL;
    }

    return markov_chain;
}

static void generate_and_print_tweets(MarkovChain *markov_chain,
                                      int tweets_num) {
    MarkovNode *first_node;

    for (int i = 1; i <= tweets_num; i++) {
        fprintf(stdout, TWEET_NUMBER, i);
        first_node = get_first_random_node(markov_chain);
        generate_tweet(markov_chain, first_node, MAX_TWEET_LENGTH);
    }
}
int main(int argc, char **argv) {
    if (argc != ARGS_NUM_V1 && argc != ARGS_NUM_V2) {
        fprintf(stdout, ERROR_ARGS_NUM);
        return EXIT_FAILURE;
    }
    unsigned int seed = strtol(argv[SEED_ARG], NULL, BASE);
    int tweets_num = (int) strtol(argv[TWEET_NUM_ARG],
                                  NULL, BASE);
    char *path = argv[FILE_PATH_ARG];
    int words_to_read = MAX_WORDS_TO_READ;
    if (argc == ARGS_NUM_V2) {
        words_to_read = (int) strtol(argv[WORDS_TO_READ_ARG],
                                     NULL, BASE);
    }
    srand(seed); // setting the seed
    FILE *fp = fopen(path, "r");
    if (fp == NULL) { // checks if the file path leads to a valid file
        fprintf(stdout, ERROR_PATH);
        return EXIT_FAILURE;
    }
    MarkovChain *markov_chain = create_and_populate_chain(fp, words_to_read);
    if (markov_chain == NULL) {
        fprintf(stdout, ERROR_MEMORY);
        fclose(fp);
        return EXIT_FAILURE;
    }
    fclose(fp);
    generate_and_print_tweets(markov_chain, tweets_num);
    free_markov_chain(&markov_chain);
    return EXIT_SUCCESS;
}
//    MarkovChain *markov_chain = make_markov_chain(print_func,
//                                                  compare_func,
//                                                  free_func,
//                                                  copy_string,
//                                                  is_last_state);
//    if (markov_chain == NULL) {
//        fprintf(stdout, ERROR_MEMORY);
//        fclose(fp);
//        return EXIT_FAILURE;
//    }
//    // populating the markov chain with the words from the file
//    if (fill_database(fp, words_to_read, markov_chain) != 0) {
//        fprintf(stdout, ERROR_MEMORY);
//        free_markov_chain(&markov_chain);
//        fclose(fp);
//        return EXIT_FAILURE;
//    }
//    fclose(fp); //finished using the file
//    // generating the random tweets
////    print_markov_chain(markov_chain);
//    MarkovNode *first_node = get_first_random_node(markov_chain);
//
//    for (int i = 1; i <= tweets_num; i++) {
//        fprintf(stdout, TWEET_NUMBER, i);
//       generate_tweet(markov_chain, first_node,
//                                 MAX_TWEET_LENGTH);
//        first_node = get_first_random_node(markov_chain);
//    }
//    free_markov_chain(&markov_chain);
//    return EXIT_SUCCESS;
//}