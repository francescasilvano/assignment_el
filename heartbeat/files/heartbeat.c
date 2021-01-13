#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
//#define q	11		    /* for 2^11 points */
#define N	2048		/* N-point FFT, iFFT */

typedef float real;
typedef struct{real Re; real Im;} complex;
typedef struct{complex v[N]; complex scratch[N];int fd;}parameters;
#ifndef PI
# define PI	3.14159265358979323846264338327950288
#endif

void fft( complex *v, int n, complex *tmp )
{
  if(n>1) {			/* otherwise, do nothing and return */
    int k,m;    
    complex z, w, *vo, *ve;
    ve = tmp; vo = tmp+n/2;
    for(k=0; k<n/2; k++) {
      ve[k] = v[2*k];
      vo[k] = v[2*k+1];
    }
    fft( ve, n/2, v );		/* FFT on even-indexed elements of v[] */
    fft( vo, n/2, v );		/* FFT on odd-indexed elements of v[] */
    for(m=0; m<n/2; m++) {
      w.Re = cos(2*PI*m/(double)n);
      w.Im = -sin(2*PI*m/(double)n);
      z.Re = w.Re*vo[m].Re - w.Im*vo[m].Im;	/* Re(w*vo[m]) */
      z.Im = w.Re*vo[m].Im + w.Im*vo[m].Re;	/* Im(w*vo[m]) */
      v[  m  ].Re = ve[m].Re + z.Re;
      v[  m  ].Im = ve[m].Im + z.Im;
      v[m+n/2].Re = ve[m].Re - z.Re;
      v[m+n/2].Im = ve[m].Im - z.Im;
    }
  }
  return;
}
void* compute( void *arg )
{
	parameters * p=(parameters*)arg;
	complex *v=p->v;
	complex *tmp=p->scratch;
    float abs[N];
    int minIdx, maxIdx;
    int k,m;
	  // FFT computation
    fft( v, N, tmp );


	  // PSD computation
    for(k=0; k<N; k++) {
      abs[k] = (50.0/2048)*((v[k].Re*v[k].Re)+(v[k].Im*v[k].Im));
    }

    	  minIdx = (0.5*N)/50;   // position in the PSD of the spectral line corresponding to 30 bpm
    	  maxIdx = 3*N/50;       // position in the PSD of the spectral line corresponding to 180 bpm

    	  // Find the peak in the PSD from 30 bpm to 180 bpm
    	  m = minIdx;
    	  for(k=minIdx; k<(maxIdx); k++) {
    		  	 if( abs[k] > abs[m] )
    		  		 m = k;
    	  }

    	  // Print the heart beat in bpm
    	  printf( "\n\n\n%d bpm\n\n\n", (m)*60*50/N );
    	  pthread_exit(NULL);
  return(0);
}
void* threadread (void* arg){
	parameters* p=(parameters *)arg;
	complex *v=p->v;
	int k;
	int res;
	for(k=0;k<N;k++){
		 res=read( p->fd, NULL, 1);
		 v[k].Re = res;
		 v[k].Im= 0;
		usleep(20000);//sleep for 0.2s
	}
	pthread_exit(NULL);
	return(0);
}
int main(void)
{
  parameters argument;
  argument.fd=-1;
  int ret;
  char *dev_name = "/dev/mydriver_dev";
  pthread_t readthread,calcthread;
  if ((argument.fd = open(dev_name, O_RDWR)) < 0)
  {
  fprintf(stderr, " unable to open %s\n", dev_name);
  return(1);
  }
  while(1){
	  // Initialize the complex array for FFT computation
	  ret= pthread_create(&readthread,NULL,threadread,&argument);//crea il thread, myThread è il nome della funzione che il thread deve eseguire
	  if(ret!=0){
		   	printf("Can't create the thread (%s)\n",strerror(errno));
		   	exit(-1);
	   }
	  //}quando si arriva a 2048 valori si va avanti
	  pthread_join(readthread,NULL);//aspetta che il primo thread abbia finito
	  ret= pthread_create(&calcthread,NULL,compute,&argument);
	  if(ret!=0){
		   	printf("Can't create the thread (%s)\n",strerror(errno));
		   	exit(-1);
	   }
	  pthread_detach(&calcthread);
  }
  exit(EXIT_SUCCESS);
}
