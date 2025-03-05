//
// Created by Yotam Gaash on 02/05/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "linked_list.h"
#include "markov_chain.h"

// macros
#define ARGS_NUM_V1 4
#define ARGS_NUM_V2 5

//#define MAX_WORD_LEN 101
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


/**
 * this functions creates a Markov chain out of a txt file
 * @param fp the path to the txt file
 * @param words_to_read the number of words to read from the file
 * @param markov_chain The markov chain to which the words will be added
 * @return 0 is succeeded, 1 otherwise
 */

int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain) {
    char line[MAX_LINE_LEN];

    // reading one line at a time and splitting it using strtok
    while (fgets(line, MAX_LINE_LEN, fp) != NULL && words_to_read > 0) {
        char *current_word = strtok(line, " \n"); //tokenizing the line
        char *previous_word = NULL;

        while (current_word != NULL && words_to_read > 0) {
            Node *node = add_to_database(markov_chain, current_word);
            if (node == NULL) {
                fprintf(stdout, ERROR_MEMORY);
                free_database(&markov_chain);
                return 1;
            }
            if (previous_word != NULL) {
                // Adding the current word to the frequencies list of the
                // previous word
                Node *previous_node = get_node_from_database(markov_chain,
                                                             previous_word);
                if (previous_node != NULL) {
                    if (!add_node_to_frequencies_list(previous_node->data,
                                                      node->data)) {
                        fprintf(stdout, ERROR_MEMORY);
                        free_database(&markov_chain);
                        return 1;
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

/**
 * prints all the contents of a markov node
 * @param markov_node
 */
void print_markov_node(MarkovNode *markov_node) {
    printf("\n %s -> [ ", markov_node->data);
    for (int i = 0; i < markov_node->freq_list_size; i++) {
        printf("(%d,%s) ", markov_node->frequencies_list[i].frequency,
               markov_node->frequencies_list[i].markov_node->data);
        if (i != markov_node->freq_list_size - 1) {
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
    printf("%s", current_node->data->data);
    while (current_node->next != NULL) {
        current_node = current_node->next;
        printf(" -> %s", current_node->data->data);
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

int main(int argc, char **argv) {
    if (argc != ARGS_NUM_V1 && argc != ARGS_NUM_V2) {
        fprintf(stdout, ERROR_ARGS_NUM);
        return EXIT_FAILURE;
    }

    unsigned int seed = strtol(argv[SEED_ARG], NULL, BASE);
    int tweets_num = (int) strtol(argv[TWEET_NUM_ARG], NULL, BASE);
    char *path = argv[FILE_PATH_ARG];
    int words_to_read = MAX_WORDS_TO_READ;
    if (argc == ARGS_NUM_V2) {
        words_to_read = (int) strtol(argv[WORDS_TO_READ_ARG], NULL, BASE);
    }
    srand(seed); // setting the seed
    FILE *fp = fopen(path, "r");
    if (fp == NULL) { // checks if the file path leads to a valid file
        fprintf(stdout, ERROR_PATH);
        return EXIT_FAILURE;
    }
    MarkovChain *markov_chain = create_markov_chain();
    if (markov_chain == NULL) {
        fprintf(stdout, ERROR_MEMORY);
        fclose(fp);
        return EXIT_FAILURE;
    }
    // populating the markov chain with the words from the file
    if (fill_database(fp, words_to_read, markov_chain) != 0) {
        fprintf(stdout, ERROR_MEMORY);
        free_database(&markov_chain);
        fclose(fp);
        return EXIT_FAILURE;
    }
    fclose(fp); //finished using the file
    // generating the random tweets
    MarkovNode *first_node = get_first_random_node(markov_chain);

    for (int i = 1; i <= tweets_num; i++) {
        fprintf(stdout, TWEET_NUMBER, i);
        generate_tweet(markov_chain, first_node, MAX_TWEET_LENGTH);
        first_node = get_first_random_node(markov_chain);
    }
    free_database(&markov_chain);
    return EXIT_SUCCESS;
}