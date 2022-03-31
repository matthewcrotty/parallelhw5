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

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // open
    // Write block/ranks size 1 to file
    // close
    // open
    // Read block/ranks size 1 from file
    // close file

    // Is it one file for each rank or one file for each block
    // Or one file overall?
    MPI_Finalize();

}
