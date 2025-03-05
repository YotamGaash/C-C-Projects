#include "sort_bus_lines.h"
#include "test_bus_lines.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
// macros

#define MAX_INPUT_LEN 61
#define MAX_LINE_LEN 22
#define MIN_DURATION 10
#define MAX_DURATION 100
#define MIN_DISTANCE 0
#define MAX_DISTANCE 1000
#define BUS_FIELDS_NUM 3
#define VALID_ARGS_NUM 2
#define NUM_OF_TESTS 6
#define BASE 10
#define DURATION_COMMAND "by_duration"
#define DISTANCE_COMMAND "by_distance"
#define NAME_COMMAND "by_name"
#define TEST_COMMAND "test"
#define BUS_NUM_MSG "Enter number of lines. Then enter\n"
#define LINE_INFO_MSG "Enter line info. Then enter\n"
#define ERROR_LINE_NUM "ERROR: Number of lines should be a positive integer\n"
#define ERROR_NAME "ERROR: name contains only small letters and numbers\n"
#define ERROR_INPUT "ERROR: input format <line_name>,<distance>,<duration>\n"
#define USAGE_ERROR "USAGE: valid args:by_duration,by_distance,by_name,test\n"
#define ERROR_DURATION "ERROR: duration should be an integer between 10 and\
100 (includes)\n"
#define ERROR_DISTANCE "ERROR: distance must be a number between 0 to 1000\
(includes)\n"
#define TEST_MSG "TEST %d %s%s"
#define PASS "PASSED: "
#define FAIL "FAILED: NOT "
#define TEST_1 "The array is sorted by distance\n"
#define TEST_2 "The array has the same items after sorting\n"
#define TEST_3 "The array is sorted by duration\n"
#define TEST_4 "The array has the same items after sorting\n"
#define TEST_5 "sorted by name\n"
#define TEST_6 "The array has the same items after sorting\n"


#define BUS_PRINT_FORMAT "%s,%d,%d\n"

// Function prototypes
int valid_int(char *input, int min, int max);

int valid_name(char *in);

int valid_bus_line(char *name, char *distance, char *duration);

int get_line_num();

BusLine get_bus_line();

BusLine *get_n_bus_lines(int n);

void print_buses(BusLine *start, BusLine *end);

void run_tests(char test_msgs[NUM_OF_TESTS][MAX_INPUT_LEN],
			   BusLine *copy, BusLine *start, BusLine
			   *end, int n);

int tests(BusLine *start, BusLine *end);

/**
 * checks if the CLI args are of a valid format
 * @param argc number of args
 * @param argv array of args strings
 * @return 1 if valid 0 otherwise
 */
int valid_args(int argc, char *argv[])
{
	if (argc != VALID_ARGS_NUM)
	{
		return 0;
	}
	if (strcmp(argv[1], NAME_COMMAND) != 0 && strcmp(argv[1],
													 DISTANCE_COMMAND) != 0 &&
		strcmp(argv[1], DURATION_COMMAND) != 0 && strcmp(argv[1],
														 TEST_COMMAND) != 0)
	{
		return 0;
	}
	return 1;
}

/**
 * checks if a given string can represent a number in a given range
 * @param input string to be checked
 * @param min minimum value of the range
 * @param max maximum value of the range
 * @return 1 if true, 0 otherwise
 */
int valid_int(char *input, int min, int max)
{
	char *end_ptr;
	int n = (int) strtol(input, &end_ptr, BASE);
	//checking if the conversion was successful
	if (end_ptr == input || *end_ptr != '\0')
	{
		return 0;
	}
	//checking if n is in the range
	if (n < min || n > max)
	{
		return 0;
	}
	return 1;
}

/**
 * get the number of bus lines from the user
 * @return the number of bus lines
 */
int get_line_num()
{
	char in[MAX_INPUT_LEN];
	int line_num;
	while (1)
	{
		fprintf(stdout, BUS_NUM_MSG);
		if (!fgets(in, MAX_INPUT_LEN, stdin))
		{
			fprintf(stdout, ERROR_LINE_NUM);
			continue;
		}
		in[strcspn(in, "\n")] = '\0'; //removing the new line char
		if (!valid_int(in, 0, INT_MAX))
		{
			fprintf(stdout, ERROR_LINE_NUM);
			continue;
		}
		line_num = (int) strtol(in, NULL, BASE);
		return line_num;
	}
}

/**
 * checks if a string is a valid bus name
 * @param in input string
 * @return 1 if it is valid 0 otherwise
 */
int valid_name(char *in)
{
	if (!in)
	{
		return 0; //empty string
	}
	//checking if in contains only lowercase letters and numbers
	for (int i = 0; i < (int) strlen(in); i++)
	{
		if (!(isdigit(in[i]) || islower(in[i])))
		{
			return 0;
		}
	}
	return 1;
}

int valid_bus_line(char *name, char *distance, char *duration)
{
	if (!valid_name(name))
	{
		fprintf(stdout, ERROR_NAME);
		return 0;
	}
	if (!valid_int(distance, MIN_DISTANCE, MAX_DISTANCE))
	{
		fprintf(stdout, ERROR_DISTANCE);
		return 0;

	}
	if (!valid_int(duration, MIN_DURATION, MAX_DURATION))
	{
		fprintf(stdout, ERROR_DURATION);
		return 0;
	}
	return 1;
}

/**
 * gets a bus line from the user, checks if it is valid
 * @return
 */
BusLine get_bus_line()
{
	char name[MAX_LINE_LEN], distance[MAX_LINE_LEN], duration[MAX_LINE_LEN];
	char input[MAX_INPUT_LEN];
	while (1)
	{
		fprintf(stdout, LINE_INFO_MSG);
		fgets(input, MAX_INPUT_LEN, stdin);
		input[strcspn(input, "\n")] = '\0';
		if (sscanf(input, "%[^,],%[^,],%[^,]",
				   name, distance, duration) !=
			BUS_FIELDS_NUM)
		{
			fprintf(stdout, USAGE_ERROR);
			continue;
		}
		if (!valid_bus_line(name, distance, duration))
		{
			continue;
		}
		BusLine bus_line;
		strcpy(bus_line.name, name);
		bus_line.distance = (int) strtol(distance, NULL, BASE);
		bus_line.duration = (int) strtol(duration, NULL, BASE);
		return bus_line;

	}
}

/**
 * create an array of n bus lines
 * @param n the size of the array
 * @return array of bus lines, if memory allocation failed returns null
 */
BusLine *get_n_bus_lines(int n)
{
	BusLine *bus_lines = malloc(n * sizeof(BusLine));
	if (!bus_lines)
	{
		return NULL;
	}
	for (BusLine *bus_p = bus_lines; bus_p < bus_lines + n; bus_p++)
	{
		*bus_p = get_bus_line();
	}
	return bus_lines;
}

void print_buses(BusLine *start, BusLine *end)
{
	for (BusLine *bus_ptr = start; bus_ptr < end; bus_ptr++)
	{
		printf(BUS_PRINT_FORMAT,
			   bus_ptr->name, bus_ptr->distance, bus_ptr->duration);
	}
}


void run_tests(char test_msgs[NUM_OF_TESTS][MAX_INPUT_LEN],
			   BusLine *copy, BusLine *start, BusLine
			   *end, int n)
{
	int test_num = 0;
	//test 1
	quick_sort(copy, copy + n, DISTANCE);
	fprintf(stdout, TEST_MSG, test_num + 1,
			is_sorted_by_distance(copy, copy + n) ?
			PASS : FAIL, test_msgs[test_num]);
	test_num++;
	// test 2
	fprintf(stdout, TEST_MSG, test_num + 1,
			is_equal(copy, copy + n,
					 start, end) ?
			PASS : FAIL, test_msgs[test_num]);
	test_num++;
	// test 3
	quick_sort(copy, copy + n, DURATION);
	fprintf(stdout, TEST_MSG, test_num + 1,
			is_sorted_by_duration(copy, copy + n) ?
			PASS : FAIL, test_msgs[test_num]);
	test_num++;
	//test 4
	fprintf(stdout, TEST_MSG, test_num + 1,
			is_equal(copy, copy + n,
					 start, end) ?
			PASS : FAIL, test_msgs[test_num]);
	test_num++;
	// test 5
	bubble_sort(copy, copy + n);
	fprintf(stdout, TEST_MSG, test_num + 1,
			is_sorted_by_name(copy, copy + n) ?
			PASS : FAIL, test_msgs[test_num]);
	test_num++;
	//test 6
	fprintf(stdout, TEST_MSG, test_num + 1,
			is_equal(copy, copy + n,
					 start, end) ?
			PASS : FAIL, test_msgs[test_num]);
}


int tests(BusLine *start, BusLine *end)
{
	char test_msgs[NUM_OF_TESTS][MAX_INPUT_LEN] = {TEST_1,
												   TEST_2,
												   TEST_3,
												   TEST_4,
												   TEST_5,
												   TEST_6};
	int n = (int) (end - start);
	BusLine *copy = (BusLine *) malloc((n + 1) * sizeof(BusLine));
	if (!copy)
	{
		return 0;
	}
	memcpy(copy, start, (n + 1) * sizeof(BusLine));
	run_tests(test_msgs, copy, start, end, n);
	free(copy);
	return 1;
}

int main(int argc, char *argv[])
{

	if (!valid_args(argc, argv))
	{
		fprintf(stdout, USAGE_ERROR);
		return EXIT_FAILURE;
	}
	int n = get_line_num();
	BusLine *bus_lines = get_n_bus_lines(n);
	if (!bus_lines)
	{
		return EXIT_FAILURE;
	}
	if (!strcmp(argv[1], TEST_COMMAND))
	{
		if (!tests(bus_lines, bus_lines + n - 1))
		{
			free(bus_lines);
			return EXIT_FAILURE;
		}
		free(bus_lines);
		return EXIT_SUCCESS;
	} else if (!strcmp(argv[1], DISTANCE_COMMAND))
	{
		quick_sort(bus_lines, bus_lines + n - 1, DISTANCE);
	} else if (!strcmp(argv[1], DURATION_COMMAND))
	{
		quick_sort(bus_lines, bus_lines + n - 1, DURATION);
	} else if (!strcmp(argv[1], NAME_COMMAND))
	{
		bubble_sort(bus_lines, bus_lines + n - 1);
	}
	print_buses(bus_lines, bus_lines + n);
	free(bus_lines);
	return EXIT_SUCCESS;
}

