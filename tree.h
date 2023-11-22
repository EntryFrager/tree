#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "include/error.h"
#include "include/utils.h"

#define DEBUG

#ifdef DEBUG
    #define CALL_DUMP(tree, code_error)                                         \
    {                                                                           \
        tree_dump_text (tree, code_error, __FILE__, __func__, __LINE__);        \
        tree_dump_graph_viz (tree, __FILE__, __func__, __LINE__);               \
    }

    static int CODE_ERROR = 0;

    #define assert_tree(tree)                                                                   \
    {                                                                                           \
        if ((CODE_ERROR = (tree_verificator (tree) | node_verificator (tree->root))) != ERR_NO) \
        {                                                                                       \
            CALL_DUMP(tree, CODE_ERROR)                                                         \
            return CODE_ERROR;                                                                  \
        }                                                                                       \
    }

    #define CHECK_ERROR_PRINT(code_error)  if (code_error != ERR_NO) fprintf (stderr, "\x1b[31m%s\x1b[0m", my_strerr (code_error, stderr));
    #define CHECK_ERROR_RETURN(code_error) if (code_error != ERR_NO) return code_error;
#else
    #define CALL_DUMP(...)
    #define assert_tree(...)
    #define CHECK_ERROR_PRINT(...)
    #define CHECK_ERROR_RETURN(...)
#endif

const bool LEFT  = false;
const bool RIGHT = true;
const int VALUE_VENOM = -10000;

typedef struct NODE{
    NODE *left   = NULL;
    NODE *right  = NULL;
    NODE *parent = NULL;

    int value = VALUE_VENOM;
} NODE;

typedef struct {
    char *fp_name_base = NULL;
    FILE *fp_base      = NULL;

    size_t size_file = 0;

    char *buf = NULL;

#ifdef DEBUG
    char *fp_dump_text_name = NULL;
    char *fp_dot_name       = NULL;
    char *fp_name_html      = NULL;
    char *fp_image          = NULL;

    FILE *fp_html_dot = NULL;
#endif
} INFO;

typedef struct {
    NODE *root = NULL;

    bool init_status = false;

    INFO info = {};
} TREE;

int create_tree (TREE *tree, const int value);

NODE *create_node (const int value, NODE *left, NODE *right, NODE *parent);

int input_text (TREE *tree);

NODE *split_node (TREE *tree, NODE *node);

int add_node (TREE *tree, NODE *node, const int value, const bool side);

int delete_node (TREE *tree, NODE *node);

int print_tree (TREE *tree, NODE *node, FILE *stream);

int destroy_tree (TREE *tree);

#ifdef DEBUG
    int tree_verificator (TREE *tree);

    int node_verificator (NODE *node);

    void tree_dump_text (TREE *tree, const int code_error, 
                         const char *file_err, const char *func_err, 
                         const int line_err);

    void tree_dump_graph_viz (TREE *tree, const char *file_err, 
                              const char *func_err, const int line_err);

    void tree_dump_html (TREE *tree);
#endif

#endif //TREE_H