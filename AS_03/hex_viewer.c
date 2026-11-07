#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 16

int main(int argc, char *argv[])
{
    FILE *file;
    unsigned char buffer[BUFFER_SIZE];
    size_t bytesRead;
    unsigned long long offset = 0;

    /* Check that the user provided exactly one filename. */
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    /* Open the file in binary read mode. */
    file = fopen(argv[1], "rb");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    printf("Offset     Hexadecimal Bytes                                  ASCII\n");
    printf("--------------------------------------------------------------------------\n");

    /* Read and display up to 16 bytes in each iteration. */
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) > 0)
    {
        size_t i;

        /* Display the offset of the first byte in this row. */
        printf("%08llX   ", offset);

        /* Display hexadecimal values, padding incomplete rows. */
        for (i = 0; i < BUFFER_SIZE; i++)
        {
            if (i < bytesRead)
                printf("%02X ", (unsigned int)buffer[i]);
            else
                printf("   ");

            if (i == 7)
                printf(" ");
        }

        /* Display the ASCII representation. */
        printf(" |");
        for (i = 0; i < bytesRead; i++)
        {
            if (buffer[i] >= 32 && buffer[i] <= 126)
                printf("%c", buffer[i]);
            else
                printf(".");
        }
        printf("|\n");

        /* Move the offset by the number of bytes actually read. */
        offset += bytesRead;
    }

    /* Distinguish normal end-of-file from a read error. */
    if (ferror(file))
    {
        perror("Error reading file");
        fclose(file);
        return 1;
    }

    if (fclose(file) != 0)
    {
        perror("Error closing file");
        return 1;
    }

    if (offset == 0)
        printf("The file is empty.\n");
    else
        printf("File reading completed successfully (%llu bytes).\n", offset);

    return 0;
}