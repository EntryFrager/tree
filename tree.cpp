#include "tree.h"

#define WHITE_COLOR "\"#ffffff\""
#define BLUE_COLOR "\"#00BFFF\""
#define YELLOW_COLOR "\"#FFFF00\""
#define PURPLE_COLOR "\"#8B00FF\""
#define RED_COLOR "\"#ff0000\""
#define LIGHT_GREEN_COLOR "\"#ccff99\""
#define BACK_GROUND_COLOR "\"#696969\""
#define WEIGHT "\"10000\""

#define FONTNAME "\"Times-New-Roman\""

const char *fp_dump_text_name = "file_err.txt";
const char *fp_dot_name = "dump.dot";

int create_tree (TREE *tree, const int value)
{
    tree->root = create_node (value, NULL, NULL, NULL);
    tree->size = 1;

    tree->init_status = true;

    assert_tree (tree);

    return ERR_NO;
}

NODE *create_node (const int value, NODE *left, NODE *right, NODE *parent)
{
    NODE *node = (NODE *) calloc (1, sizeof (NODE));

    node->value  = value;
    node->left   = left;
    node->right  = right;
    node->parent = parent;

    return node;
}

int add_node (TREE *tree, NODE *node, const int value, const bool side)
{
    assert_tree (tree);

    NODE *new_node = create_node (value, NULL, NULL, node);
    
    if (side == LEFT)
    {
        node->left = new_node;
    }
    else
    {
        node->right = new_node;
    }

    tree->size++;

    assert_tree (tree);

    return ERR_NO;
}

int delete_node (TREE *tree, NODE *node)
{
    assert_tree (tree);

    if (node->parent->left == node)
    {
        node->parent->left = NULL;
    }
    else if (node->parent->right == node)
    {
        node->parent->right = NULL;
    }

    if (node->left != NULL)
    {
        CHECK_ERROR_RETURN (delete_node (tree, node->left));
    }
    
    if (node->right != NULL)
    {
        CHECK_ERROR_RETURN (delete_node (tree, node->right));
    }

    free (node);
    node = NULL;
    tree->size--;

    assert_tree (tree);
    
    return ERR_NO;
}

int print_tree (TREE *tree, NODE *node, FILE *stream)
{
    assert_tree (tree);

    if (!node)
    {
        fprintf (stream, " nil ");
        return ERR_NO;
    }

    fprintf (stream, "(");

    fprintf (stream, "%d ", node->value);

    print_tree (tree, node->left, stream);
    print_tree (tree, node->right, stream);

    fprintf (stream, ")");

    assert_tree (tree);

    return ERR_NO;
}

#ifdef DEBUG

static void print_tree_dump (NODE *node, FILE *stream);

static void node_connect (NODE *node, FILE *stream, int ip);
static void create_node (NODE *node, FILE *stream, int ip);
static void create_connect_nodes (NODE *node, FILE *stream, int ip);

#define VERIF_TREE_ELEM(expr, name_error) if (!(expr)) {code_error |= name_error;}

int tree_verificator (TREE *tree)
{
    int code_error = 0;

    VERIF_TREE_ELEM (tree != NULL, TREE_ERR_PTR)

    VERIF_TREE_ELEM (tree->init_status == true, TREE_INIT)

    VERIF_TREE_ELEM (tree->size > 0, TREE_ERR_SIZE)

    VERIF_TREE_ELEM (tree->root != NULL, TREE_ERR_ROOT_PTR)

    return code_error;
}

#undef VERIF_TREE_ELEM

#define DUMP_LOG(str) fprintf (fp_err, str "\n");
#define DUMP_LOG_PARAM(str, ...) fprintf (fp_err, str "\n", __VA_ARGS__);

void tree_dump_text (TREE *tree, const int code_error, 
                     const char *file_err, const char *func_err, 
                     const int line_err)
{
    FILE *fp_err = fopen (fp_dump_text_name, "a");

    if (fp_err == NULL)
    {
        my_strerr (ERR_FOPEN, stderr);
    }
    else
    {
        DUMP_LOG ("--------------------------------------------------------------------------------------------------------");

        my_strerr (code_error, fp_err);

        if (tree == NULL)
        {
            DUMP_LOG_PARAM ("tree[NULL] \"tree\" called from %s(%d) %s", file_err, line_err, func_err);
        }
        else
        {
            DUMP_LOG_PARAM ("tree[%p] \"tree\" called from %s(%d) %s", tree, file_err, line_err, func_err);
            DUMP_LOG ("{");
            DUMP_LOG_PARAM ("\tsize = %d", tree->size);
            DUMP_LOG_PARAM ("\tinit_status = %d", tree->init_status);
            DUMP_LOG ("\t{");

            if ((code_error & TREE_ERR_ROOT_PTR) == 0)
            {
                print_tree_dump (tree->root, fp_err);
            }
            else
            {
                DUMP_LOG ("\t\t*root[NULL]");
            }

            DUMP_LOG ("\t}");
            DUMP_LOG ("}");
        }

        DUMP_LOG ("--------------------------------------------------------------------------------------------------------\n\n");
    }

    if (fclose (fp_err) != 0)
    {
        my_strerr (ERR_FCLOSE, stderr);
    }
}

void print_tree_dump (NODE *node, FILE *stream)
{
    fprintf (stream, "\t\t*node[%p] = %d;\n", node, node->value);

    if (node->left != NULL)
    {
        print_tree_dump (node->left, stream);
    }
    else if (node->right != NULL)
    {
        print_tree_dump (node->right, stream);
    }
}

void tree_dump_graph_viz (TREE *tree, const int code_error, 
                         const char *file_err, const char *func_err, 
                         const int line_err)
{
    FILE *fp_dot = fopen (fp_dot_name, "w+");

    if (fp_dot == NULL)
    {
        my_strerr (ERR_FOPEN, stderr);
    }
    else
    {
        if (tree != NULL)
        {
            fprintf (fp_dot, "digraph List {\n");
            fprintf (fp_dot, "\trankdir = LR;\n");
            fprintf (fp_dot, "\tnode [shape = record];\n");
            fprintf (fp_dot, "\tbgcolor = " BACK_GROUND_COLOR ";\n");

            if (code_error != TREE_ERR_SIZE)
            {
                fprintf (fp_dot, "\t0");

                node_connect (tree->root, fp_dot, 0);

                fprintf (fp_dot, "[arrowsize = 0.0, weight = " WEIGHT ", color = " BACK_GROUND_COLOR ", fontname = " FONTNAME "];\n");

                create_node (tree->root, fp_dot, 0);

                create_connect_nodes (tree->root, fp_dot, 0);
            }

            fprintf (fp_dot, "\tlabel = \"tree_dump from function %s, Tree/%s:%d\";\n", func_err, file_err, line_err);
            fprintf (fp_dot, "\tAll[shape = Mrecord, style = filled, fillcolor = " PURPLE_COLOR ", label = \"size = %d\"];}\n", tree->size);
        }
    }

    if (fclose (fp_dot) != 0)
    {
        my_strerr (ERR_FCLOSE, stderr);
    }

    char *command = "dot -Tsvg dump.dot -o dot.svg";

    system (command);
}

void node_connect (NODE *node, FILE *stream, int ip)
{
    fprintf (stream, " -> %d", ip);

    if (node->left != NULL)
    {
        node_connect (node->left, stream, ip + 1);
    }

    if (node->right != NULL)
    {
        node_connect (node->right, stream, ip + 1);
    }
}

void create_node (NODE *node, FILE *stream, int ip)
{
    fprintf (stream, "\t%d [shape = Mrecord, style = filled, fillcolor = " LIGHT_GREEN_COLOR ", label = \"idx: %p | value: %d | left: %p | right: %p | parent: %p\"];\n", 
             ip, node, node->value, node->left, node->right, node->parent);
    
    if (node->left != NULL)
    {
        create_node (node->left, stream, ip + 1);
    }

    if (node->right != NULL)
    {
        create_node (node->right, stream, ip + 1);
    }
}

void create_connect_nodes (NODE *node, FILE *stream, int ip)
{
    if (node->left != NULL)
    {
        fprintf (stream, "\t%d -> %d\n", ip, ip + 1);
        create_connect_nodes (node->left, stream, ip + 1);
    }

    if (node->right != NULL)
    {
        fprintf (stream, "\t%d -> %d\n", ip, ip + 1);
        create_connect_nodes (node->right, stream, ip + 1);
    }
}

#endif