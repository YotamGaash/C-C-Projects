#include "cipher.h"
#include "string.h"
/// IN THIS FILE, IMPLEMENT EVERY FUNCTION THAT'S DECLARED IN cipher.h.

#define LOWER_CASE 'a'
#define UPPER_CASE 'A'
#define ABC_LEN 26

/**
 * this function shifts a char value by k if its either lower or upper case
 * letter
 * @param c the input char
 * @param k the shift value
 * @return the shifted character by k
 */
char shift_char_by_k(char c, int k)
{
    if (c >= LOWER_CASE && c <= (LOWER_CASE + ABC_LEN))
    {
        return LOWER_CASE + (((((c - LOWER_CASE) + k + ABC_LEN) % ABC_LEN) +
        ABC_LEN) % ABC_LEN);
    }
    if (c >= UPPER_CASE && c <= (UPPER_CASE + ABC_LEN))
    {
        return UPPER_CASE + (((((c - UPPER_CASE) + k + ABC_LEN) % ABC_LEN) +
                            ABC_LEN) % ABC_LEN);
    }
    return c;
}

// See full documentation in header file
void encode(char s[], int k)
{
    int str_len = strlen(s);
    for (int i = 0; i < str_len; i++)
    {
        s[i] = shift_char_by_k(s[i],k); //shifting each char of s by k
    }
}

// See full documentation in header file
void decode(char s[], int k)
{
    encode(s,-k); // decoding is the exact opposite action to decoding
}
