#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H
// write only between #define EX2_REPO_TESTBUSLINES_H and #endif
// EX2_REPO_TESTBUSLINES_H
#include "sort_bus_lines.h"
#include <stdlib.h>

/**
 * checks if an array of buses is sorted according to the sorting type
 * @param start start of the array
 * @param end end of the array
 * @param sort_type sort type - either duration, distance or name
 * @return 1 if it is sorted 0 otherwise
 */
int is_sorted(BusLine *start, BusLine *end, SortType sort_type);
/**
 * checks if a bus line array is sorted by distance
 * @param start start of the array
 * @param end end of the array
 * @return 1 if it is sorted 0 otherwise
 */
int is_sorted_by_distance (BusLine *start, BusLine *end);

/**
 * checks if a bus line array is sorted by duration
 * @param start start of the array
 * @param end end of the array
 * @return 1 if it is sorted 0 otherwise
 */
int is_sorted_by_duration (BusLine *start, BusLine *end);

/**
 * checks if a bus line array is sorted by name
 * @param start start of the array
 * @param end end of the array
 * @return 1 if it is sorted 0 otherwise
 */
int is_sorted_by_name (BusLine *start, BusLine *end);

/**
 * checks if two bus arrays are equal - meaning they contain the same BusLine
 * elements not necessarily in the same order
 * @param start_sorted start of the sorted bus array
 * @param end_sorted end of the sorted bus array
 * @param start_original start of the original bus array
 * @param end_original end of the original bus array
 * @return return 1 if they are equal 0 otherwise
 */
int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original);
// write only between #define EX2_REPO_TESTBUSLINES_H and #endif
// EX2_REPO_TESTBUSLINES_H
#endif //EX2_REPO_TESTBUSLINES_H
