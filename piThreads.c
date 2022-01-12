#include <assert.h> 
#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/time.h> 
static int sed1, sed2, dentro, veces, numThreads; 
static pthread_mutex_t RC; 
void *esclavo(void *basura) {   
    int i, dentroLocal, tid;   
    unsigned short xi[3];   
    double x, y;   
    tid = (int) basura;   
    dentroLocal = 0;   
    xi[0] = sed1;   
    xi[1] = sed2;   
    xi[2] = tid;   
    for (i=tid; i<veces; i += numThreads) {       
        x = erand48(xi);       
        y = erand48(xi);       
        if (x*x+y*y <= 1.0) 
        dentroLocal++;   
    }     
    pthread_mutex_lock (&RC);     
    dentro += dentroLocal;   
    pthread_mutex_unlock (&RC);   
    pthread_exit(NULL);   
    exit (0); 
}
int main (int argc, char *argv[]) {   
    pthread_t tids[4];   
    int i;   
    struct timeval t0, t1, t;   
    assert (gettimeofday (&t0, NULL) == 0);   
    if (argc != 5) {     
        printf ("Uso: piThreads numVeces numThreads sed1 sed2\n");     
        return 0;   
    }     
    veces = atoi(argv[1]);   
    numThreads = atoi(argv[2]);   
    sed1 = atoi(argv[3]);   
    sed2 = atoi(argv[4]);   
    dentro = 0;   
    for (i=0; i<numThreads; i++)     
        assert (pthread_create (&tids[i], NULL, esclavo, (void *) i) == 0);   
    for (i=0; i<numThreads; i++)     
        assert (pthread_join (tids[i], NULL) == 0);
    assert (gettimeofday (&t1, NULL) == 0);   
    timersub(&t1, &t0, &t);   
    printf ("Tiempo = %3ld:%3ld (seg:miliseg)\n", t.tv_sec, t.tv_usec/1000);   
    printf ("Valor estimado de pi: %7.5f\n", 4.0*dentro/veces);   exit(0); 
}