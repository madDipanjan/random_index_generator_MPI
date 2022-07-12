# random_index_generator_MPI
This is an MPI code written c++. This code generates a set of M random integers in the range  [0,N), on each process. The random number generator is seeded differently on each process. For each random number on a process, the program computes which other processes also have that number generated.
