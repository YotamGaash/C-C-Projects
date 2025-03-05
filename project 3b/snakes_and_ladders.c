#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))
#define MIN(X, Y) (((X) > (Y)) ? (Y) : (X))
#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60
#define FIRST_CELL 1
#define LAST_CELL 100
#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20
#define ARG_NUM 3
#define SEED_ARG 1
#define BASE 10
#define PATHS_NUM_ARG 2
#define LADDER_TXT "[%d]-ladder to %d-> "
#define SNAKE_TXT "[%d]-snake to %d-> "
#define CELL_TXT "[%d]-> "
#define LAST_CELL_TXT "[%d]\n"
#define RANDOM_PATH_NUM "Random Walk %d: "
#define ERROR_DB_FILL "ERROR: failed to fill the markov chain database.\n"
#define ERROR_ARGS_NUM "Usage:<seed> <tweets_num> <path> (opt)<read_num>\n"
/**
 * struct represents a Cell in the game board
 */
typedef struct Cell {
    int number; // Cell number 1-100
    int ladder_to;  // ladder_to represents the jump of the ladder in case
    // there is one from this square
    int snake_to;  // snake_to represents the jump of the snake in case there
    // is one from this square
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;
/***************************/
/* Function Prototypes    */

/***************************/

static bool is_last_state(void *data);

static void print_func(void *data);

static int compare_func(void *data1, void *data2);

static void free_func(void *data);

static void *copy_cell(void *data);

/****************************************************/
/* Markov Chain Functions for string data types.    */

/****************************************************/
// Check if a string is the last state (assuming it ends with a '.')

static bool is_last_state(void *data) {
    if (data == NULL) {
        return false;
    }
    Cell *cell_data = (Cell *) data;
    if (cell_data->number == LAST_CELL) {
        return true;
    }
    return false;
}

// Print function for strings
static void *copy_cell(void *data) {
    if (data == NULL) {
        return NULL;  // Handle NULL case if needed
    }
    // Cast the data to the specific type
    Cell *copy = malloc(sizeof(Cell));
    if (!copy) {
        return NULL;
    }
    copy->number = ((Cell *) data)->number;
    copy->ladder_to = ((Cell *) data)->ladder_to;
    copy->snake_to = ((Cell *) data)->snake_to;
    return copy;
}

static void print_func(void *data) {
    if (data == NULL) {
        return;
    } else {
        Cell *cell = (Cell *) data;
        if (is_last_state(cell)) {
            printf(LAST_CELL_TXT, cell->number);
            return;
        }
        if (cell->snake_to != EMPTY) {
            printf(SNAKE_TXT, cell->number, cell->snake_to);
            return;
        }
        if (cell->ladder_to != EMPTY) {
            printf(LADDER_TXT, cell->number, cell->ladder_to);
            return;
        }
        printf(CELL_TXT, cell->number);
    }
}

static int compare_func(void *data1, void *data2) {
    if (data1 == NULL && data2 == NULL) {
        return 0; // Both are NULL, consider them equal
    } else if (data1 == NULL || data2 == NULL) {
        return 1; // One is NULL, consider them not equal
    }
    Cell *cell1 = (Cell *) data1;
    Cell *cell2 = (Cell *) data2;

    return cell1->number != cell2->number; //comp function need to return 0
    // if equal
}

static void free_func(void *data) {
    free(data);
}


/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/** Error handler **/
static int handle_error(char *error_msg, MarkovChain **database) {
    printf("%s", error_msg);
    if (database != NULL) {
        free_markov_chain(database);
    }
    return EXIT_FAILURE;
}


static int create_board(Cell *cells[BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        cells[i] = malloc(sizeof(Cell));
        if (cells[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(cells[j]);
            }
            handle_error(ALLOCATION_ERROR_MASSAGE, NULL);
            return EXIT_FAILURE;
        }
        *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
    }

    for (int i = 0; i < NUM_OF_TRANSITIONS; i++) {
        int from = transitions[i][0];
        int to = transitions[i][1];
        if (from < to) {
            cells[from - 1]->ladder_to = to;
        } else {
            cells[from - 1]->snake_to = to;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database(MarkovChain *markov_chain) {
    Cell *cells[BOARD_SIZE];
    if (create_board(cells) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    MarkovNode *from_node = NULL, *to_node = NULL;
    size_t index_to;
    for (size_t i = 0; i < BOARD_SIZE; i++) {
        add_to_database(markov_chain, cells[i]);
    }

    for (size_t i = 0; i < BOARD_SIZE; i++) {
        from_node = get_node_from_database(markov_chain,
                                           cells[i])->data;

        if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY) {
            index_to = MAX(cells[i]->snake_to, cells[i]->ladder_to) - 1;
            to_node = get_node_from_database(markov_chain,
                                             cells[index_to])->data;
            add_node_to_counter_list(from_node, to_node,
                                     markov_chain);
        } else {
            for (int j = 1; j <= DICE_MAX; j++) {
                index_to = ((Cell *) (from_node->data))->number + j - 1;
                if (index_to >= BOARD_SIZE) {
                    break;
                }
                to_node = get_node_from_database(markov_chain,
                                                 cells[index_to])
                        ->data;
                add_node_to_counter_list(from_node,
                                         to_node, markov_chain);
            }
        }
    }
    // free temp arr
    for (size_t i = 0; i < BOARD_SIZE; i++) {
        free(cells[i]);
    }
    return EXIT_SUCCESS;
}

/**
 * creates a walking path in the board and prints it.
 * @param markov_chain markov_chain of the board
 */
static void create_path(MarkovChain *markov_chain) {
    MarkovNode *current_node =
            get_node_from_database(markov_chain,&((Cell)
            {FIRST_CELL,
             EMPTY,
             EMPTY}))->data;

    int steps = 0;
    while (steps < MAX_GENERATION_LENGTH) {
        // Check for victory
        if (((Cell *) (current_node->data))->number == LAST_CELL) {
            printf(LAST_CELL_TXT, LAST_CELL);
            return;  // End of the route
        }

        // Select next cell based on transitions
        int current_cell_number = ((Cell *) (current_node->data))->number;
        int next_cell_number = ((Cell *) (current_node->data))->number;
        if (((Cell *) (current_node->data))->ladder_to != EMPTY) {
            next_cell_number = ((Cell *) (current_node->data))->ladder_to;
            printf(LADDER_TXT, current_cell_number, next_cell_number);
        } else if (((Cell *) (current_node->data))->snake_to != EMPTY) {
            next_cell_number = ((Cell *) (current_node->data))->snake_to;
            printf(SNAKE_TXT, current_cell_number, next_cell_number);
        } else {
            printf(CELL_TXT, ((Cell *) (current_node->data))->number);
            // If no ladder or snake, select one of the next 6 cells with
            // equal chance
            int cell_options = MIN(DICE_MAX, LAST_CELL -
            current_cell_number);
            //if there are fewer cells than dice options, choosing rand number
            //from the remaining cells.
            next_cell_number += (rand() % cell_options) + 1;
        }

        // Update current node
        current_node = get_node_from_database(markov_chain, &((Cell)
                {next_cell_number,
                 EMPTY, EMPTY}))->data;
        steps++;
    }

    printf("\n");  // Route ended without reaching LAST_CELL
}

//static void print_cell(Cell *cell) {
//    printf("Cell %d, ladder %d, snake %d\n",
//           cell->number, cell->ladder_to, cell->snake_to);
//}
//
//static void print_markov_chain(MarkovChain *markov_chain) {
//    Node *current = markov_chain->database->first;
//    while (current) {
//        print_cell((Cell *) current->data->data);
//        current = current->next;
//    }
//}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[]) {
    if (argc != ARG_NUM) {
        fprintf(stdout, ERROR_ARGS_NUM);
        return EXIT_FAILURE;
    }
    unsigned int seed = strtol(argv[SEED_ARG], NULL, BASE);
    int paths_to_generate = (int) strtol(argv[PATHS_NUM_ARG],
                                         NULL, BASE);
    srand(seed); //setting the seed for rand function
    //creating a new markov chain
    MarkovChain *markov_chain = make_markov_chain(print_func,
                                                  compare_func,
                                                  free_func,
                                                  copy_cell,
                                                  is_last_state);
    if (!markov_chain) {
        return EXIT_FAILURE;
    }
    paths_to_generate -= 1;
    //filling the database
    if (fill_database(markov_chain) == EXIT_FAILURE) {
        fprintf(stdout, ERROR_DB_FILL);
        free_markov_chain(&markov_chain);
        return EXIT_FAILURE;
    }
//    print_markov_chain(markov_chain);
//     Generate and print random walks
    for (int i = 1; i <= paths_to_generate; i++) {
        printf("Random Walk %d: ", i);
        create_path(markov_chain);
    }

    // Free Markov Chain memory
    free_markov_chain(&markov_chain);
    return 0;
}


