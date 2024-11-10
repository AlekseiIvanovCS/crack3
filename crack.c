#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

#if __has_include("fileutil.h")
#include "fileutil.h"
#endif

#define PASS_LEN 50     // Maximum length any password will be.
#define HASH_LEN 33     // Length of hash plus one for null.


int compareStrings(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

char *exactMatchSearch(char *target, char **arr, int size)
{
    for(int i = 0; i < size; i++)
    {
        if(strcmp(arr[i], target) == 0)
        {
            return arr[i];
        }
    }
    return NULL;
}

char *binarySearch(char *target, char **arr, int size)
{
    int left = 0, right = size - 1;

    do
    {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(arr[mid], target);
        if (cmp == 0) return arr[mid];
        if (cmp < 0) left = mid + 1;
        else right = mid - 1;
    }
    while(left <= right);
    return NULL;
}

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Usage: %s hash_file dictionary_file\n", argv[0]);
        exit(1);
    }

    int size;
    char **hashes = loadFileAA(argv[1], &size);
    if(!hashes)
    {
        printf("Failed to load hashes\n");
        exit(1);
    }

    qsort(hashes, size, sizeof(char *), compareStrings);

    FILE *dictFile = fopen(argv[2], "r");
    if(!dictFile)
    {
        printf("Could not open dictionary file\n");
        freeAA(hashes, size);
        exit(1);
    }

    int crackedCount = 0;
    char word[PASS_LEN];

    while(fgets(word, PASS_LEN, dictFile))
    {
        word[strcspn(word, "\n")] = '\0';

        char *computedHash = md5(word, strlen(word));
        if(!computedHash)
        {
            printf("Failed to compute hash for %s\n", word);
            continue;
        }

        char *foundHash = binarySearch(computedHash, hashes, size);
        if(foundHash)
        {
            printf("%s %s\n", computedHash, word);
            crackedCount++;
        }

        free(computedHash);
    }

    fclose(dictFile);
    freeAA(hashes, size);
    printf("%d hashes cracked!\n", crackedCount);
}