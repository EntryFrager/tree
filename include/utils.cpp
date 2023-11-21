#include "utils.h"

int input_text (TREE *tree)
{
    my_assert (tree != NULL);

    tree->info.fp_base = fopen (tree->info.fp_name_base, "r + b");

    if (tree->info.fp_base == NULL)
    {
        return ERR_FOPEN;
    }

    size_t size_file = get_file_size (tree->info.fp_base);

    tree->info.buf = (char *) calloc (size_file + 1, sizeof (char));
    my_assert (tree->info.buf != NULL);

    size_t read_size = fread (tree->info.buf, sizeof (char), size_file, tree->info.fp_base);

    if (read_size != size_file)
    {
        return ERR_FREAD;
    }

    *(tree->info.buf + size_file) = '\0';

    if (fclose (tree->info.fp_base) != 0)
    {
        return ERR_FCLOSE;
    }

    return ERR_NO;
}

size_t get_file_size (FILE *stream)
{
    my_assert (stream != NULL);

    size_t start = ftell (stream);
    fseek (stream, start, SEEK_END);
    size_t size_file = ftell (stream);
    rewind (stream);

    return size_file;
}