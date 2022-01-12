#include <stdlib.h> 
#include <stdio.h> 
#include <sys/time.h> 
#include "mpi.h" 
#define MAX_PROCESOS         16 
#define MAX_ITER     1000000000 
static int yo, numProcesos, numVeces, sed1, sed2; 
int computar(void) {   
    int i, enCirculo=0;   
    unsigned short xi[3];   
    double x, y;   
    xi[0] = sed1;   
    xi[1] = sed2;   
    xi[2] = yo;   
    for (i=yo; i<numVeces; i+=numProcesos) {     
        x = erand48(xi);     
        y = erand48(xi);     
        if (x*x+y*y <= 1.0) 
        enCirculo++;   
    }     
    return enCirculo; 
}

void esclavo(void) {   
    int enCirculo;   
    enCirculo = computar();   
    MPI_Send(&enCirculo, 1, MPI_INT, 0, 1, MPI_COMM_WORLD); 
} 
void maestro(void) {   
    int i, enCirculoEsc, enCirculoTot;   
    double pi;   
    struct timeval t0, t1, t;   
    MPI_Status estado;   
    assert (gettimeofday (&t0, NULL) == 0);   
    enCirculoTot = computar();   
    for (i=1; i<numProcesos; i++) {     
        MPI_Recv(&enCirculoEsc, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &estado);     
        enCirculoTot+=enCirculoEsc;   
    }     
    pi = 4.0 * enCirculoTot / numVeces;   
    assert (gettimeofday (&t1, NULL) == 0);   
    timersub(&t1, &t0, &t);   
    printf ("Valor estimado de PI = %7.5f\n", pi);   
    printf ("Tiempo = %ld:%ld(seg:mseg)\n", t.tv_sec, t.tv_usec/1000); 
}
int main(int argc, char *argv[]) { 
    setbuf (stdout, NULL);   
    MPI_Init (&argc, &argv);   
    MPI_Comm_rank (MPI_COMM_WORLD, &yo);   
    MPI_Comm_size (MPI_COMM_WORLD, &numProcesos);   
    // Control del numero de procesos   
    if ((numProcesos < 2) || (numProcesos > MAX_PROCESOS)) {     
        if (yo == 0) printf ("Numero de procesos incorrecto\n");     
        MPI_Finalize();     
        exit(0);   
    }     
    // Control del numero de parametros   
    if (argc != 4) {     
        if (yo == 0) 
            printf ("Uso: ... piMPI numVeces sed1 sed2\n");     
            MPI_Finalize();     
            exit(0);   
    }     
    numVeces = atoi(argv[1]);   
    sed1     = atoi(argv[2]);   
    sed2     = atoi(argv[3]);   
    if (yo == 0) 
        maestro();   
    else         
        esclavo();   
    MPI_Finalize();   
    return 0; 
}