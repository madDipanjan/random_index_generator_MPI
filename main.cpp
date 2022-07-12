#include<iostream>
#include<mpi.h>
#include<cstdlib>

using namespace std;

int main(int argc, char *argv[])
{
        int nprocs, myrank, count;
        int M = 25, N = 1500, min = 0;                   //M random numbers within the range of [0,N) on each process
        
	double rand_numbers[M];                         //variable for storing the random numbers on each process

        MPI_Init(&argc,&argv);                          //initiating MPI
        MPI_Comm_size(MPI_COMM_WORLD, &nprocs);         //get total number of process running
        MPI_Comm_rank(MPI_COMM_WORLD, &myrank);         //get my rank id
        
	double out_data[M*nprocs];
	int n_other_exist[M], rank_other_exist[M][nprocs-1], location_other_exist[M][nprocs-1];
      
        // generate M random numbers within the range of [0,N) on each process with different seed every time on every process
        srand(time(0)*myrank);
        for(int i=0; i<M; i++)
        {
                rand_numbers[i] = (rand() % (N-min+1)) + min;
        }
        
        // gather all the random numbers from all the processes to a single array "out_data" and then broadcast it to all processes
        MPI_Allgather(rand_numbers, M, MPI_DOUBLE, out_data, M, MPI_DOUBLE, MPI_COMM_WORLD);     
        
        //For each random number on a process, identify which other processes also have that random number, if any
        for(int i=0; i<M; i++)
        {
                count = 0;
                for(int j=0;j<M*nprocs;j++)
                {
                        if(j/M != myrank && rand_numbers[i] == out_data[j])
                        {
                                rank_other_exist[i][count] = j/M;       //For each random number on a process, "rank_other_exist" stores rank of other processes where the number has also appeared 
                                location_other_exist[i][count] = j%M;   //"locatoion_other_exist" stores the index location on other processes where the number appears again
                                count++;
                        }
                }
                n_other_exist[i] = count;       //For each random number on a process, "n_other_exist" stores how many other processes the number has also appeared 
        }
        
        MPI_Barrier(MPI_COMM_WORLD);
	
	//printing the random numbers generated
	if(myrank == 0)
	{
	        for(int i=0;i<nprocs; i++)
	                cout << "rank" << i << "  ";
	        cout << endl;
	        for(int i=0; i<M; i++)
	        {
		        for (int j=0; j<nprocs; j++)
		        {		
                                cout << out_data[i+M*j] << "  ";
	                }
	                cout << endl;
	        }
	        cout << endl;
        }	
        
        MPI_Barrier(MPI_COMM_WORLD);
        
        
        //printing the details of the numbers that appears on more than one processes
        if(myrank == 0)
	        cout << "My rank --> location index --> random number --> number of other processes it apeared --> rank of the other processes it appeared --> location index other existence" << endl; 
        
        MPI_Barrier(MPI_COMM_WORLD);
        
        for(int i=0;i<M;i++)
        {
                for(int j=0; j<n_other_exist[i]; j++){
                cout << myrank << "  -->  " << i << "  -->  " << rand_numbers[i] << " --> " << n_other_exist[i] << " --> " << rank_other_exist[i][j] << " --> " << location_other_exist[i][j] << endl; 
                }
        }
        
        MPI_Finalize();                                 //Terminate MPI

        return 0;
}
