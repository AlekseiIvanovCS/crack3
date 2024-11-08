#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileutil.h"

#define INITIAL_CAPACITY 10 // Initial size for the array of strings

// Load the text file into an array of strings (array of arrays).
char **loadFileAA(char *filename, int *size)
{
    FILE *in = fopen(filename, "r");
    if(!in)
	{
        printf("Can't open file");
        exit(1);
    }

    int capacity = INITIAL_CAPACITY;
    char **arr = malloc(capacity * sizeof(char *));
    if(!arr)
	{
        printf("Memory allocation failed");
        exit(1);
    }

    char buffer[COLS];
    *size = 0;

    while(fgets(buffer, COLS, in))
	{
        // Trim newline
        char *nl = strchr(buffer, '\n');
        if (nl) *nl = '\0';

        if(*size >= capacity)
		{
            capacity *= 2;
            arr = realloc(arr, capacity * sizeof(char *));
            if(!arr)
			{
                printf("Memory reallocation failed");
                exit(1);
            }
        }

        // Allocate memory for the string and copy it
        arr[*size] = malloc((strlen(buffer) + 1) * sizeof(char));
        if(!arr[*size])
		{
            printf("Memory allocation for string failed");
            exit(1);
        }
        strcpy(arr[*size], buffer);

        (*size)++;
    }

    fclose(in);
    return arr;
}

// Search the array for the target string.
char *substringSearchAA(char *target, char **arr, int size)
{
    for(int i = 0; i < size; i++)
	{
        if (strstr(arr[i], target))
		{
            return arr[i];
        }
    }
    return NULL;
}

// Free the memory used by the array.
void freeAA(char **arr, int size)
{
    for (int i = 0; i < size; i++)
	{
        free(arr[i]);
    }
    free(arr);
}


// void free2D(char (*arr)[COLS])
// {

// }