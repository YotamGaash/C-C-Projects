#include "tests.h"
#include <string.h>

#define K_1 3
#define K_2 2
#define K_3 -1
#define K_4 -3
#define K_5 29

#define IN_2 "!abcdefghijklmnopqrstuvw"
#define OUT_2 "!cdefghijklmnopqrstuvwxy"

#define IN_3 "z*_*zy*_*zy*_*zy*_*zy*_*zy*_*z"
#define OUT_3 "y*_*yx*_*yx*_*yx*_*yx*_*yx*_*y"

#define IN_4 "abcdefghijklmnopqrstuvwxy"
#define OUT_4 "xyzabcdefghijklmnopqrstuv"

#define IN_5 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define OUT_5 "DEFGHIJKLMNOPQRSTUVWXYZABC"

#define IN_6 "defghijklmnopqrstuvwxyzabc"
#define OUT_6 "abcdefghijklmnopqrstuvwxyz"

#define IN_7 "!cdefghijklmnopqrstuvwxyzab"
#define OUT_7 "!abcdefghijklmnopqrstuvwxyz"

#define IN_8 "y*_*yx*_*yx*_*yx*_*yx*_*yx*_*y"
#define OUT_8 "z*_*zy*_*zy*_*zy*_*zy*_*zy*_*z"

#define IN_9 "xyzabcdefghijklmnopqrstuvw"
#define OUT_9 "abcdefghijklmnopqrstuvwxyz"

#define IN_10 "CDEFGHIJKLMNOPQRSTUVWXYZAB"
#define OUT_10 "ZABCDEFGHIJKLMNOPQRSTUVWXY"

// See full documentation in header file

int test_encode_non_cyclic_lower_case_positive_k ()
{
    char in[] = "abc";
    char out[] = "def";
    encode (in, K_1);
    return strcmp (in, out) == 0;
}


int test_encode_cyclic_lower_case_special_char_positive_k()
{
    char in[] = IN_2;
    char out[] = OUT_2;
    encode(in, K_2);

    return strcmp(in, out) == 0;
}

int test_encode_non_cyclic_lower_case_special_char_negative_k()
{
    char in[] = IN_3;
    char out[] = OUT_3;
    encode(in, K_3);
    return strcmp(in, out) == 0;
}

int test_encode_cyclic_lower_case_negative_k()
{
    char in[] = IN_4;
    char out[] = OUT_4;
    encode(in, K_4);

    return strcmp(in, out) == 0;
}

int test_encode_cyclic_upper_case_positive_k()
{
    char in[] = IN_5;
    char out[] = OUT_5;
    encode(in, K_5);
    return strcmp(in, out) == 0;
}

int test_decode_non_cyclic_lower_case_positive_k()
{
    char in[] = IN_6;
    char out[] = OUT_6;
    decode(in, K_1);
    return strcmp(in, out) == 0;
}

int test_decode_cyclic_lower_case_special_char_positive_k()
{
    char in[] = IN_7;
    char out[] = OUT_7;
    decode(in, K_2);

    return strcmp(in, out) == 0;
}

int test_decode_non_cyclic_lower_case_special_char_negative_k()
{
    char in[] = IN_8;
    char out[] = OUT_8;
    decode(in, K_3);
    return strcmp(in, out) == 0;
}

int test_decode_cyclic_lower_case_negative_k()
{
    char in[] = IN_9;
    char out[] = OUT_9;
    decode(in, K_4);

    return strcmp(in, out) == 0;
}

int test_decode_cyclic_upper_case_positive_k()
{
    char in[] = IN_10;
    char out[] = OUT_10;
    decode(in, K_5);
    return strcmp(in, out) == 0;
}