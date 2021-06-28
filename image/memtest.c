/*
 * k8s Memory Tester - just allocates a lot of memory by effectively
 * creating a memory leak.
 *
 * Don't criticise my coding style!
 * 
 * Use at your own risk
 * Author: Matt Day
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#define DEFAULT_MEM 1024
#define DEFAULT_SLEEP 30
#define ALLOCATE_CHUNK 1048575
#define BYTE_SIZE 1024

long get_memory()
{
    // Memory to allocate
    long mem;
    // Memory from environment variable MEMORY
    char *memory_env;
    memory_env = getenv("MEMORY");
    if (memory_env)
    {
        mem = atoi(memory_env);
        if (mem == 0)
        {
            mem = DEFAULT_MEM;
        }
    }
    else
    {
        mem = DEFAULT_MEM;
    }
    return mem * ALLOCATE_CHUNK;
}

int get_sleep_time()
{
    // Time to sleep (in seconds)
    long sleep_time;
    // Sleep time from environment variable SLEEP
    char *sleep_env;
    sleep_env = getenv("SLEEP");
    if (sleep_env)
    {
        sleep_time = atoi(sleep_env);
        if (sleep_time == 0)
        {
            sleep_time = DEFAULT_SLEEP;
        }
    }
    else
    {
        sleep_time = DEFAULT_SLEEP;
    }
    return sleep_time;
}

int main()
{
    // Memory to allocate
    long mem;
    // Pointer to malloc'd memory (which will get dropped on each loop, creating a memory leak!)
    char *mem_data;
    // Time (in seconds) to sleep
    int sleep_time;
    // Random data to write
    char *random_data;
    // Size of memory to allocate in bytes
    long mem_size;
    // When we break up in chunks, how much memory remains?
    long mem_remain;

    mem = get_memory();
    printf("Consuming %lu bytes of memory (%lu megabytes / %f gigabytes) \n", mem, (mem / 1024 / 1024), (double)(mem / 1024.0 / 1024.0 / 1024.0));
    // Flush the buffer
    fflush(stdout);

    // Divide memory in to chunks of ALLOCATE_CHUNK
    mem_remain = mem;
    while (mem_remain > 0)
    {
        if (mem_remain > ALLOCATE_CHUNK)
        {
            mem_remain = (mem_remain - ALLOCATE_CHUNK);
            mem = ALLOCATE_CHUNK;
        }
        else
        {
            mem = mem_remain;
            mem_remain = 0;
        }
        mem_size = (mem);
        mem_data = (char *)malloc(mem_size);

        /*
        * Fill the memory by writing to it
        */
        for (int i = 0; i < mem_size; i++)
        {
            mem_data[i] = (char)rand();
        }
    }
    sleep_time = get_sleep_time();
    /*
     * Just wait in a loop, printing out just to prove we're alive
     */
    while (1)
    {
        printf("Sleeping for %i seconds\n", sleep_time);
        fflush(stdout);
        sleep(sleep_time);
    }

    return 0;
}
