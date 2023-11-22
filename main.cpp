#include "tree.h"

int main ()
{
    TREE tree = {};

    create_tree (&tree, 10);

    input_text (&tree);

    FILE *fp = fopen (tree.info.fp_name_base, "w");

    print_tree (&tree, tree.root, fp);

    if (fclose (fp) != 0)
    {
        my_strerr (ERR_FCLOSE, stderr);
    }

    CALL_DUMP (&tree, 0);

    destroy_tree (&tree);

    return 0;
}