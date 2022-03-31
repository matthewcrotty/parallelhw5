#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "clockcycle.h"
// different block sizes: 128K 256K 512K 1M 2M 4M 8M 16M
// each block is just full of ints = 1
// 32 blocks to write/read per test
// tests are 2, 4, 8, 16, 32, 64 MPI ranks
// between each I/O block test and re-open file.
// each rank reads block/n_ranks size block
// ranks read blocks in order.
// Each experiment run on scratch and NVME

int main(int argc, char** argv){

    int my_rank;
    int world_size;

    unsigned long long start_time;
    unsigned long long end_time;
    int clock_frequency = 512000000;
    double time_in_secs;

    // I hope that this is correct, in order for the largest file to be 32GB,
    // you need 64 ranks writing 32 blocks of 16M bytes to the file. That means
    // for the 16M processor block size, you actually need to write
    // 16M/sizeof(int) integers
    int block_bytes = atoi(argv[1]);
    int block_size = block_bytes/sizeof(int);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int* buffer = (int*)malloc(block_size * sizeof(int));
    int* ones = (int*)malloc(block_size * sizeof(int));
    for(int i = 0; i < block_size; i++)
        ones[i] = 1;


    MPI_File file;
    MPI_Status status;
    int offset;

    start_time = clock_now();

    MPI_File_open(MPI_COMM_SELF, "iotest.txt", MPI_MODE_RDWR | MPI_MODE_CREATE, MPI_INFO_NULL, &file);

    for(int i = 0; i < 32; i++){
        offset = (block_bytes * world_size * i) + (block_bytes* my_rank);
        MPI_File_write_at(file, offset, ones, block_size, MPI_INT, &status);
        MPI_File_read_at(file, offset, buffer, block_size, MPI_INT, &status);
    }

    MPI_File_close(&file);
    end_time = clock_now();

    time_in_secs = ((double)(end_time - start_time)) / clock_frequency;
    printf("Rank %d, Time %f\n", my_rank, time_in_secs);

    free(ones);
    free(buffer);

    MPI_Finalize();

}
