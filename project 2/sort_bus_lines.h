#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H
// write only between #define EX2_REPO_SORTBUSLINES_H and #endif
// EX2_REPO_SORTBUSLINES_H
#include <string.h>
#define NAME_LEN 21
/**
 * A struct for a bus line containing it's name
 * the distance of the route and its duration
 * @param name The line name, contain only lowercase letters and numbers
 * @param distance route distance in range between 0 to 1000
 * @param duration route duration in range between 10 to 100
 */
typedef struct BusLine
{
    char name[NAME_LEN];
    int distance, duration;
} BusLine;
typedef enum SortType
{
    DISTANCE,
    DURATION,
    NAME
} SortType;

/**
 * swaps between two bus lines
 * @param bus_a pointer to bus a
 * @param bus_b pointer to bus b
 */
void swap_buses(BusLine *bus_a, BusLine *bus_b);

/**
 * sorts a bus line array by the bus lines' names using a bubble sort
 * algorithm
 * @param start start of the bus array
 * @param end end of the bus array
 */
void bubble_sort (BusLine *start, BusLine *end);

/**
 * sorts a bus line array by either duration or distance using quick sort
 * algorithm
 * @param start start of the array
 * @param end end of the array
 * @param sort_type either duration or distance
 */
void quick_sort (BusLine *start, BusLine *end, SortType sort_type);

/**
 * helper function for quick sort, divides an array to two using a pivot
 * @param start start of the array
 * @param end end of the array
 * @param sort_type either duration or distance
 * @return a pointer to the pivot element
 */
BusLine *partition (BusLine *start, BusLine *end, SortType sort_type);
// write only between #define EX2_REPO_SORTBUSLINES_H and #endif
// EX2_REPO_SORTBUSLINES_H
#endif //EX2_REPO_SORTBUSLINES_H
