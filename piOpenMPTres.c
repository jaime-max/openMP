#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
int main (int argc, char *argv[]) {
    int dentro, veces, numThreads;
    struct timeval t0, t1, t;
    assert (gettimeofday (&t0, NULL) == 0);
    if (argc != 5) {
        printf ("Uso: piOpenMPUno numVeces numThreads sed1 sed2\n");
        return 0;
    }
    veces = atoi(argv[1]);
    numThreads = atoi(argv[2]);
    omp_set_num_threads (numThreads);
    dentro = 0;

    #pragma omp parallel reduction(+:dentro)
    {
        unsigned short xi[3];
        int yo, i;
        double x, y;
        xi[0] = atoi(argv[3]);
        xi[1] = atoi(argv[4]);
        yo = omp_get_thread_num();
        xi[2] = yo;
        #pragma omp
        for (i=yo; i<veces; i += numThreads) {
            x = erand48(xi);
            y = erand48(xi);
            if (x*x+y*y <= 1.0){
                dentro++;
            }   
        }
        
    }
    assert (gettimeofday (&t1, NULL) == 0);
    timersub(&t1, &t0, &t);
    printf ("Tiempo = %3ld:%3ld (seg:miliseg)\n", t.tv_sec, t.tv_usec/1000);
    printf ("Valor estimado de pi: %7.5f\n", 4.0*dentro/veces);
    return 0;
}