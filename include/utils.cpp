#include "utils.h"

int input_text (GENERAL_INFO *gen_inf)
{
    my_assert (gen_inf != NULL);

    gen_inf->fp_input = fopen (gen_inf->fp_name_input, "r + b");

    if (gen_inf->fp_input == NULL)
    {
        return ERR_FOPEN;
    }

    gen_inf->size_file = get_file_size (gen_inf->fp_input);

    gen_inf->buf = (char *) calloc (gen_inf->size_file + 1, sizeof (char));
    my_assert (gen_inf->buf != NULL);

    size_t read_size = fread (gen_inf->buf, sizeof (char), gen_inf->size_file, gen_inf->fp_input);

    if (read_size != gen_inf->size_file)
    {
        return ERR_FREAD;
    }

    *(gen_inf->buf + gen_inf->size_file) = '\0';

    if (fclose (gen_inf->fp_input) != 0)
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