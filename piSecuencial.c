#include <assert.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/time.h> 
int main (int argc, char *argv[]) {   
    int dentro, i, veces;   
    unsigned short xi[3];   
    double x, y;  
    struct timeval t0, t1, t; 
    assert (gettimeofday (&t0, NULL) == 0);   
        if (argc != 4) {     
            printf ("Uso: pi numVeces sed1 sed2\n");     
            return 0;   
        }     
        veces = atoi(argv[1]);   
        dentro = 0;   
        xi[0] = atoi(argv[2]);   
        xi[1] = atoi(argv[3]);   
        xi[2] = 0;   
        for (i=0; i<veces; i++) {       
            x = erand48(xi);       
            y = erand48(xi);       
            if (x*x+y*y <= 1.0) 
            dentro++;   
        }   
    assert (gettimeofday (&t1, NULL) == 0);   
    timersub(&t1, &t0, &t);   
    printf ("Tiempo = %3ld:%3ld (seg:miliseg)\n", t.tv_sec, t.tv_usec/1000);   
    printf ("Valor estimado de pi: %7.5f\n", 4.0*dentro/veces);   
    return 0; 
    
}