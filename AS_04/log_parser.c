#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK_SIZE 512

static int read_line(FILE *file, char **line, size_t *capacity)
{
    char chunk[CHUNK_SIZE];
    size_t length = 0;

    (*line)[0] = '\0';

    while (fgets(chunk, sizeof(chunk), file) != NULL)
    {
        size_t chunk_length = strlen(chunk);
        size_t required;

        if (length == (size_t)-1 ||
            chunk_length > (size_t)-1 - length - 1)
        {
            return -1;
        }

        required = length + chunk_length + 1;

        if (required > *capacity)
        {
            size_t new_capacity = *capacity;
            char *new_line;

            while (new_capacity < required)
            {
                if (new_capacity > (size_t)-1 / 2)
                {
                    new_capacity = required;
                    break;
                }

                new_capacity *= 2;
            }

            new_line = (char *)realloc(*line, new_capacity);

            if (new_line == NULL)
            {
                return -1;
            }

            *line = new_line;
            *capacity = new_capacity;
        }

        memcpy(*line + length, chunk, chunk_length + 1);
        length += chunk_length;

        if (chunk_length > 0 && chunk[chunk_length - 1] == '\n')
        {
            return 1;
        }
    }

    if (ferror(file))
    {
        return -2;
    }

    return length > 0 ? 1 : 0;
}

int main(int argc, char *argv[])
{
    FILE *file;
    char *line;
    const char *keyword;

    size_t capacity = CHUNK_SIZE;
    size_t line_number = 0;
    size_t alert_count = 0;

    int result;
    int exit_code = 0;

    /* Check command-line arguments. */
    if (argc != 3 || argv[2][0] == '\0')
    {
        fprintf(stderr, "Usage: %s <log_file> <keyword>\n", argv[0]);
        fprintf(stderr, "Example: %s security.log FAILED\n", argv[0]);
        return 1;
    }

    keyword = argv[2];

    /* Open the log file in read mode. */
    file = fopen(argv[1], "r");

    if (file == NULL)
    {
        perror("Error opening log file");
        return 1;
    }

    /* Allocate memory for reading lines. */
    line = (char *)malloc(capacity);

    if (line == NULL)
    {
        fprintf(stderr, "Error: Not enough memory to read the log file.\n");
        fclose(file);
        return 1;
    }

    printf("Log File : %s\n", argv[1]);
    printf("Keyword  : %s\n", keyword);
    printf("----------------------------------------\n");
    printf("Suspicious Log Entries:\n\n");

    /* Read and analyze the file line by line. */
    while ((result = read_line(file, &line, &capacity)) == 1)
    {
        size_t line_length;

        line_number++;

        if (strstr(line, keyword) != NULL)
        {
            alert_count++;

            printf("[ALERT] Line %zu: %s", line_number, line);

            line_length = strlen(line);

            if (line_length == 0 || line[line_length - 1] != '\n')
            {
                printf("\n");
            }
        }
    }

    /* Check for reading errors. */
    if (result == -1)
    {
        fprintf(stderr, "Error: Unable to allocate enough memory for a log line.\n");
        exit_code = 1;
    }
    else if (result == -2)
    {
        fprintf(stderr, "Error: A problem occurred while reading the log file.\n");
        exit_code = 1;
    }

    /* Close the file. */
    if (fclose(file) != 0)
    {
        perror("Error closing log file");
        exit_code = 1;
    }

    /* Display the final analysis report. */
    printf("\n========== Analysis Report ==========\n");
    printf("Total Lines Scanned : %zu\n", line_number);
    printf("Total Alerts Found  : %zu\n", alert_count);

    if (exit_code != 0)
    {
        printf("Status: Scan incomplete due to an error.\n");
    }
    else if (alert_count == 0)
    {
        printf("Status: No matching entries found.\n");
    }
    else
    {
        printf("Status: Suspicious entries detected.\n");
    }

    free(line);

    return exit_code;
}