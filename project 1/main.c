//#include "cipher.h"
//#include "tests.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <stdbool.h>
//#include <string.h>
//
//// macros
//
//#define MAX_LINE_LEN 1026
//#define ARG_NUM_ENC_DEC 5
//#define ARG_NUM_TEST 2
//#define ENCODE_CMD "encode"
//#define DECODE_CMD "decode"
//#define TEST_CMD "test"
//
//typedef enum { ENCODE, DECODE, TEST } CommandType;
//
//// Error Massages
//
//#define ARG_NUM_ERROR "The program receives 1 or 4 arguments only.\n"
//#define ARG_TEST_ERROR "Usage: cipher test\n"
//#define K_VAL_ERROR "The given shift value is invalid.\n"
//#define FILE_ERROR "The given file is invalid.\n"
//
//
//
//// your code goes here
//
///**
// * tries to covert a string to int using strtol, returns a pointer to the int
// * if it succeeded otherwise returns NULL.
// * @param k a string to convert
// * @return pointer to int or NULL
// */
//bool valid_k(char* k)
//{
//    //checks if the input is not empty or longer than the max valid input
//    // length
//    if (k[0] == '\0' || strlen(k) > MAX_LINE_LEN)
//    {
//        return false;
//    }
//    //checking if k is a valid number
//    int i = k[0] == '-' ? 1 : 0; //skips the first char if its negative sign
//    while (k[i] != '\0') {
//        if (k[i] < '0' || k[i] > '9') {
//            return false;
//        }
//        i++;
//    }
//    return true;
//}
//
////checks if a valid number of arguments was entered
//bool valid_arg_num(int argc)
//{
// return (argc == ARG_NUM_TEST || argc == ARG_NUM_ENC_DEC) ? true : false;
//}
//
//bool valid_command(char command[], CommandType)
////checks for errors in the arguments from the user
//bool check_for_errors(int argc, char** argv){
//    if(!valid_arg_num(argc))
//    {
//        return false;
//    }
//
//}
//int main(int argc, char *argv[])
//    {
//        printf("%d", strlen("1"));
////        // ... (Include the encode, decode, and test functions)
////        // Run the tests
////        printf("Test 1: %s\n", test_encode_non_cyclic_lower_case_positive_k() ? "Passed" : "Failed");
////        printf("Test 2: %s\n", test_encode_cyclic_lower_case_special_char_positive_k() ? "Passed" : "Failed");
////        printf("Test 3: %s\n", test_encode_non_cyclic_lower_case_special_char_negative_k() ? "Passed" : "Failed");
////        printf("Test 4: %s\n", test_encode_cyclic_lower_case_negative_k() ? "Passed" : "Failed");
////        printf("Test 5: %s\n", test_encode_cyclic_upper_case_positive_k() ? "Passed" : "Failed");
////        printf("Test 6: %s\n", test_decode_non_cyclic_lower_case_positive_k() ? "Passed" : "Failed");
////        printf("Test 7: %s\n", test_decode_cyclic_lower_case_special_char_positive_k() ? "Passed" : "Failed");
////        printf("Test 8: %s\n", test_decode_non_cyclic_lower_case_special_char_negative_k() ? "Passed" : "Failed");
////        printf("Test 9: %s\n", test_decode_cyclic_lower_case_negative_k() ? "Passed" : "Failed");
////        printf("Test 10: %s\n", test_decode_cyclic_upper_case_positive_k() ? "Passed" : "Failed");
////        return 0;
//    }
//
//
#include "cipher.h"
#include "tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//macros

#define MAX_STR_LENGTH 1026
#define TEST_ARGS_NUM 2
#define ENCODE_DECODE_ARGS_NUM 5
#define BASE 10
#define ENCODE_COMMAND "encode"
#define DECODE_COMMAND "decode"
#define TEST_COMMAND "test"
//error messages
#define ERROR_ARGS_NUM "The program receives 1 or 4 arguments only.\n"
#define ERROR_TEST_COMMAND "Usage: cipher test\n"
#define ERROR_INVALID_COMMAND "The given command is invalid.\n"
#define ERROR_K_VALUE "The given shift value is invalid.\n"
#define ERROR_FILE "The given file is invalid.\n"

// your code goes here

bool is_valid_k(char k[])
/**
 * checks if the k value given by the user is a valid int number
 * @param k the string representing k
 * @return True of k is int, False otherwise
 */
{

    if (k[0] == '\0')
    { // empty string
        return false;
    }

    if (strlen(k) > MAX_STR_LENGTH)
    { //string is too long
        return false;
    }
    //negative number that is not the string '-'
    if (k[0] == '-' && strlen(k) == 1)
    {
        return false;
    }
    int i = k[0] == '-' ? 1 : 0; //if k is negative skip the first index
    while (k[i] != '\0')
    {
        if (k[i] < '0' || k[i] > '9')
        { //checks that k contains only digits
            return false;
        }
        i++;
    }
    return true;
}

bool valid_args_num(int argc)
/**
 * checks if the program was given 1 or 4 arguments from the CLI
 * @param argc the number of arguments
 * @return true of False
 */
{
    if (argc != TEST_ARGS_NUM && argc != ENCODE_DECODE_ARGS_NUM)
    {
        return false;
    }
    return true;
}

bool valid_command(char command[], int arg_num)
/**
 * checks if the user entered a valid command from the CLI
 * @param command the argument for the command entered
 * @return true if the command is valid false otherwise
 */
{

    if (arg_num == ENCODE_DECODE_ARGS_NUM)
    {
        if (!strcmp(command, DECODE_COMMAND) ||
            !strcmp(command, ENCODE_COMMAND))
        {
            return true;
        }
    } else if (arg_num == TEST_ARGS_NUM)
    {
        if (!strcmp(command, TEST_COMMAND))
        {
            return true;
        }
    }
    return false;
}

bool valid_files(char file_name[], char rw_mode[])
/**
 * checks if the given file path and the file are valid
 * @param file_name the file name
 * @param rw_mode read or write mode
 * @return true if the path and file are valid, false otherwise
 */
{
    FILE *test_file;
    if ((test_file = fopen(file_name, rw_mode)) != NULL)
    {
        fclose(test_file);
        return true;
    }
    return false;
}

bool check_for_errors(int argc, char *argv[])
/**
 * checks for errors before running the encode/decode code
 * @param argc the number of arguments the program received
 * @param argv the arguments the program received
 * @return EXIT_SUCCESS if there are no errors else EXIT_FAILURE
 */
{

    //checking for errors
    if (!valid_args_num(argc)) //invalid amount of arguments
    {
        fprintf(stderr, ERROR_ARGS_NUM);
        return EXIT_FAILURE;
    }
//    printf("ok arg num\n");
    //invalid test command
    if (argc == TEST_ARGS_NUM)
    {
        if (!valid_command(argv[1], TEST_ARGS_NUM))
        {
            fprintf(stderr, ERROR_TEST_COMMAND);
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS; //no need for other errors in test mode
    }
    //invalid decode/encode command
    if (argc == ENCODE_DECODE_ARGS_NUM)
    {
        if (!valid_command(argv[1], ENCODE_DECODE_ARGS_NUM))
        {
            fprintf(stderr, ERROR_INVALID_COMMAND);
            return EXIT_FAILURE;
        }
    }
//    printf("ok encode/decode command\n");
    if (!is_valid_k(argv[2]))
    {
        fprintf(stderr, ERROR_K_VALUE);
        return EXIT_FAILURE;
    }
//    printf("ok k value\n");
    if (!valid_files(argv[3], "r") ||
        !valid_files(argv[4], "w"))
    {
        fprintf(stderr, ERROR_FILE);
        return EXIT_FAILURE;
    }
//    printf("ok files\n");
    return EXIT_SUCCESS;
}

bool run_encode_tests()
/**
 * run all the encode tests
 * @return returns true if all the tests passed, false otherwise.
 */
{
    if (test_encode_non_cyclic_lower_case_positive_k() == 0)
    {
        return false;
    } else if (test_encode_cyclic_lower_case_special_char_positive_k() == 0)
    {
        return false;
    } else if (test_encode_non_cyclic_lower_case_special_char_negative_k() ==
               0)
    {
        return false;
    } else if (test_encode_cyclic_lower_case_special_char_positive_k() == 0)
    {
        return false;
    } else if (test_encode_cyclic_lower_case_negative_k() == 0)
    {
        return false;
    } else if (test_encode_cyclic_upper_case_positive_k() == 0)
    {
        return false;
    }
    return true;
}

bool run_decode_tests()
/**
 * run all the decode tests
 * @return returns true if all the tests passed, false otherwise.
 */
{
    if (test_decode_non_cyclic_lower_case_positive_k() == 0)
    {
        return false;
    } else if (test_decode_cyclic_lower_case_special_char_positive_k() == 0)
    {
        return false;
    } else if (test_decode_non_cyclic_lower_case_special_char_negative_k() ==
               0)
    {
        return false;
    } else if (test_decode_cyclic_lower_case_negative_k() == 0)
    {
        return false;
    } else if (test_decode_cyclic_upper_case_positive_k() == 0)
    {
        return false;
    }
    return true;
}

bool run_tests()
/**
 * runs the encode and decode tests, if both of them passed it returns true
 * @return true if all tests passed, false otherwise
 */
{
    return (run_encode_tests() && run_decode_tests());
}

void write_to_file(char *mode, char *shift_val, char *input, char *output)
/**
 * this function is called after we made sure all the parameters are valid and
 * won't return any errors. this function receive txt from a file and it encode
 * or decode it to an output file by a shift value k.
 * @param mode encode or decode
 * @param shift_val shift value for the txt
 * @param input input file
 * @param output output file
 */
{
    FILE *input_file = fopen(input, "r");
    FILE *output_file = fopen(output, "w");
    char s[MAX_STR_LENGTH]; //creating a buffer to avoid segmentation errors
    int k = strtol(shift_val, NULL, BASE);
    while (fgets(s, sizeof(s), input_file))
    {
        if (!strcmp(mode, ENCODE_COMMAND))
        {
            encode(s, k);
        } else
        {
            decode(s, k);
        }
        fputs(s, output_file);
    }

    //closing open files
    fclose(input_file);
    fclose(output_file);

}

int main(int argc, char *argv[])
{

    //checking for errors
    if (check_for_errors(argc, argv) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    // test mode
    if (argc == TEST_ARGS_NUM)
    {
        if (run_tests() == false)
        {
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    // decode/encode mode
    write_to_file(argv[1], argv[2], argv[3], argv[4]);
    return EXIT_SUCCESS;

}

