/// @file error.cpp

#include "error.h"

/**
 * Types of errors.
*/

static const char *err_msgs_arr[] = {
    "an error occurred while opening the file",
    "an error occurred while closing the file",
    "an error occurred while reading text from the file",
    "pointer to tree structure null",
    "the tree was not initialized",
    "wrong tree size",
    "invalid pointer to tree root",
    "communication between nodes is broken"
};

/**
 * Error print function.
 * @param[in] code_error
*/

void my_strerr (unsigned code_error, FILE *stream)
{
    for (int i = 0; i < ERR_CNT; i++)
    {
        if (code_error & (1 << i))
        {
            fprintf (stream, "ERROR: %s\n", err_msgs_arr[i]);
        }
    }
}
