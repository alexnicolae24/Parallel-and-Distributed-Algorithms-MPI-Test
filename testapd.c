#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

int main (int argc, char *argv[])
{
    int procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int proc_num = 2 * rank;//valoarea initiala a task-ului
    int neigh, recv;//am retinut in neigh numarul pentru vecin,iar in recv valoarea trimisa

    //Implementarea algoritmului de sortare OETS
    for (int i = 0; i < procs; i++){
        if (i % 2 == 0){
            if (rank % 2 == 0){
                if (rank != procs - 1){
                    MPI_Send(&proc_num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                    MPI_Recv(&neigh, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    proc_num = max(proc_num, neigh);//efectuarea maximului dintre valoarea vecinului si ce a task-ului actual
                }
            }
            else{
                MPI_Send(&proc_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
                MPI_Recv(&neigh, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                proc_num = min(proc_num, neigh);//efectuarea minimului dintre valoarea vecinului si ce a task-ului actual
            }
        }
        else{
            if (rank % 2 != 0){
                if (rank != procs - 1){
                    MPI_Send(&proc_num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                    MPI_Recv(&neigh, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    proc_num = max(proc_num, neigh);//efectuarea maximului dintre valoarea vecinului si ce a task-ului actual
                }
            }
            else{
                if (rank > 0){
                    MPI_Send(&proc_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
                    MPI_Recv(&neigh, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    proc_num = min(proc_num, neigh);//efectuarea minimului dintre valoarea vecinului si ce a task-ului actual
                }
            }
        }
    }
    
    //Afisarea fiecarui task cu numarul corespunzator
    printf("%d - %d\n", rank, proc_num);

    //punerea unei bariere inainte de a strange valorile pentru fiecare task
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank < procs - 1){
        for (int i = 0; i <= rank; i++) {
            //transmiterea valorilor vecinilor de la stanga
            if (i != rank) {
                MPI_Recv(&recv, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(&recv, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            } 
            else{
                MPI_Send(&proc_num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            }
        }
    } 
    else{
        for (int i = 0; i < rank; i++) {
            //primirea numerelor vecinilor de la stanga
            MPI_Recv(&recv, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%d ", recv);
        }
            printf("%d\n", proc_num);
    }
   
    MPI_Finalize();
}

