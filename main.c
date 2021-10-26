#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

typedef int fileidx;

#define DEV_RANDOM_PATH "/dev/urandom"
#define LOCAL_CACHE_PATH "./.random_cache"

#define RANDOM_BUFFER_SIZE 10

void print_array(int*, int);

int main(void)
{
    /*** Variables ***/
    fileidx cache_file = open(LOCAL_CACHE_PATH, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    fileidx dev_random = open(DEV_RANDOM_PATH, O_RDONLY, 0);

    int random_buffer[RANDOM_BUFFER_SIZE];
    int cached_buffer[RANDOM_BUFFER_SIZE];

    /*** Opening Files ***/
    if(dev_random == -1)
    {
        fprintf(stderr, "Unable to open " DEV_RANDOM_PATH "! (%d) | Exiting...\n", errno);
        exit(-1);
    }

    if(cache_file == -1)
    {
        fprintf(stderr, "Unable to open " LOCAL_CACHE_PATH "! (%d) | Exiting...\n", errno);
        exit(-1);
    }

    /*** Reading Random Data ***/
    printf("Reading %lu Bytes from " DEV_RANDOM_PATH " -> random_buffer[]\n", sizeof(random_buffer));
    read(dev_random, random_buffer, sizeof(random_buffer));
    printf("Finished Reading %lu Bytes!\n\n", sizeof(random_buffer));

    /*** Printing Out Random Buffer ***/
    printf("random_buffer[]: ");
    print_array(random_buffer, RANDOM_BUFFER_SIZE);
    printf("\n");

    /*** Saving to Cache ***/
    printf("Writing %lu Bytes from random_buffer[] -> " LOCAL_CACHE_PATH "\n", sizeof(random_buffer));
    write(cache_file, random_buffer, sizeof(random_buffer));
    printf("Finished %lu Writing Bytes!\n\n", sizeof(random_buffer));

    /*** Closing and Reopening Cache File ***/
    printf("Closing " LOCAL_CACHE_PATH "\n");
    cache_file = close(cache_file);

    printf("Reopening " LOCAL_CACHE_PATH "\n\n");
    cache_file = open(LOCAL_CACHE_PATH, O_RDONLY, 0);

    if(cache_file == -1)
    {
        fprintf(stderr, "Unable to open " LOCAL_CACHE_PATH "! (%d) | Exiting...\n", errno);
        exit(-1);
    }

    /*** Reading from Cache ***/
    printf("Reading %lu Bytes from " LOCAL_CACHE_PATH " -> cached_buffer[]\n", sizeof(cached_buffer));
    read(cache_file, cached_buffer, sizeof(cached_buffer));
    printf("Finished Reading %lu Bytes!\n\n", sizeof(cached_buffer));

    /*** Printing Out Cached Buffer ***/
    printf("cached_buffer[]: ");
    print_array(cached_buffer, RANDOM_BUFFER_SIZE);
    printf("\n");

    return 0;
}


void print_array(int* buf, int n)
{
    int i;

    printf("[%d", buf[0]);

    for(i = 1; i < n; ++i)
    { printf(", %d", buf[i]); }

    printf("]\n");
}