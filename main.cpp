#include "tree.h"

int main ()
{
    TREE tree = {};

    create_tree (&tree, 10);

    add_node (&tree, tree.root, 5, LEFT);
    add_node (&tree, tree.root->left, 6, RIGHT);
    add_node (&tree, tree.root, 7, RIGHT);
    add_node (&tree, tree.root->right, 8, RIGHT);
    add_node (&tree, tree.root->left, 3, LEFT);

    add_node (&tree, tree.root->right, 11, LEFT);

    FILE *fp = fopen ("tree.txt", "w");

    print_tree (&tree, tree.root, fp);

    CALL_DUMP (&tree, 0);

    destroy_tree (&tree);

    return 0;
}