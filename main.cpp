#include "tree.h"

int main ()
{
    TREE tree = {};

    create_tree (&tree, 10);

    add_node (&tree, tree.root, 5, LEFT);
    add_node (&tree, tree.root->left, 6, RIGHT);

    FILE *fp = fopen ("tree.txt", "w");

    print_tree (&tree, tree.root, fp);

    tree_dump_text (&tree, 1, __FILE__, __func__, __LINE__);
    tree_dump_graph_viz (&tree, 0, __FILE__, __func__, __LINE__);

    return 0;
}