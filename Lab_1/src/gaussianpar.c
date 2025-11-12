/***************************************************************************
 *
 * Parallel version of Gaussian elimination
 *
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdalign.h>
#include <pthread.h>
#include <immintrin.h>

#define MAX_SIZE 4096
#define THREADS 16
#define MAX_SEG_SIZE MAX_SIZE / THREADS

typedef double matrix[MAX_SIZE][MAX_SIZE];

typedef struct {
  int id;
} th_argument;

int N;                /* matrix size */
int maxnum;           /* max number of element*/
char *Init;           /* matrix init type */
int PRINT;            /* print switch */
alignas(32) matrix A;             /* matrix A */
alignas(32) double b[MAX_SIZE];   /* vector b */
alignas(32) double y[MAX_SIZE];   /* vector y */

th_argument arguments[THREADS];
pthread_barrier_t step_sync;


/* forward declarations */
void start_job(void);
void* work(void*);
void Init_Matrix(void);
void Print_Matrix(void);
void Init_Default(void);
void Init_Arguments(void);
int Read_Options(int, char **);

int
main(int argc, char **argv)
{
  int i, timestart, timeend, iter;

  Init_Default();           /* Init default values */
  Read_Options(argc,argv);  /* Read arguments */
  Init_Matrix();            /* Init the matrix */
  Init_Arguments();
  start_job();
  if (PRINT == 1)
    Print_Matrix();
}

/*
  1. create x threads.
  2. assign rows to the threads. (cyclic).
  3. if thread is owner of kth row preform div step and remove from 
      elimination step.
  4. notify all.
  5. preform elimination step.
*/

void
start_job(void)
{
  pthread_t threads[THREADS];
  int i;

  pthread_barrier_init(&step_sync, NULL, THREADS);

  for (i = 0; i < THREADS; i++)
  {
    pthread_create(&threads[i], NULL, work, (void*) &arguments[i]);
  }

  for (i = 0; i < THREADS; i++)
  {
    pthread_join(threads[i], NULL);
  }
}

void* 
work(void* _arg)
{
  int i, j, k;
  th_argument arg = *(th_argument*) _arg;

  double temp[MAX_SIZE];

  for (k = 0; k < N; k++)
  {
    for (j = (k + 1) + arg.id; j < N; j += THREADS)
    {
      A[k][j] = A[k][j] / A[k][k]; /* Division step */
    }

    if (arg.id == 0)
      y[k] = b[k] / A[k][k];

    pthread_barrier_wait(&step_sync); /* Wait for other threads to finish first row */

    if (arg.id == 0)
      A[k][k] = 1.0;

    for (i = (k + 1) + arg.id; i < N; i += THREADS) 
    {
      for (j = k+1; j < N; j++)
      {
        temp[j] = A[i][k]*A[k][j];
      }

      for (j = k+1; j < N; j++)
      {
        A[i][j] -= temp[j];
      }

      b[i] = b[i] - A[i][k]*y[k];
      A[i][k] = 0.0;
    }
    pthread_barrier_wait(&step_sync); /* Wait for other threads to finnish eliminating */
  }

  return NULL;
}

void
seq_work(void)
{
  int i, j, k;

  /* Gaussian elimination algorithm, Algo 8.4 from Grama */
  for (k = 0; k < N; k++) { /* Outer loop */
    for (j = k+1; j < N; j++)
      A[k][j] = A[k][j] / A[k][k]; /* Division step */
    y[k] = b[k] / A[k][k];
    A[k][k] = 1.0;
    for (i = k+1; i < N; i++) {
      for (j = k+1; j < N; j++)
        A[i][j] = A[i][j] - A[i][k]*A[k][j]; /* Elimination step */
      b[i] = b[i] - A[i][k]*y[k];
      A[i][k] = 0.0;
    }
  }
}

void
Init_Matrix()
{
  int i, j;

  printf("\nsize      = %dx%d ", N, N);
  printf("\nmaxnum    = %d \n", maxnum);
  printf("Init	  = %s \n", Init);
  printf("Initializing matrix...");

  if (strcmp(Init,"rand") == 0) {
    for (i = 0; i < N; i++){
      for (j = 0; j < N; j++) {
        if (i == j) /* diagonal dominance */
          A[i][j] = (double)(rand() % maxnum) + 5.0;
        else
          A[i][j] = (double)(rand() % maxnum) + 1.0;
      }
    }
  }
  if (strcmp(Init,"fast") == 0) {
    for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
        if (i == j) /* diagonal dominance */
          A[i][j] = 5.0;
        else
          A[i][j] = 2.0;
      }
    }
  }

  /* Initialize vectors b and y */
  for (i = 0; i < N; i++) {
    b[i] = 2.0;
    y[i] = 1.0;
  }

  printf("done \n\n");
  if (PRINT == 1)
    Print_Matrix();
}

void
Print_Matrix()
{
  int i, j;

  printf("Matrix A:\n");
  for (i = 0; i < N; i++) {
    printf("[");
    for (j = 0; j < N; j++)
      printf(" %5.2f,", A[i][j]);
    printf("]\n");
  }
  printf("Vector b:\n[");
  for (j = 0; j < N; j++)
    printf(" %5.2f,", b[j]);
  printf("]\n");
  printf("Vector y:\n[");
  for (j = 0; j < N; j++)
    printf(" %5.2f,", y[j]);
  printf("]\n");
  printf("\n\n");
}

void
Init_Default()
{
  N = 2048;
  Init = "rand";
  maxnum = 15.0;
  PRINT = 0;
}

void
Init_Arguments(void) {
  printf("Initializing thread arguments ... ");

  int i, j;

  for (i = 0; i < THREADS; i++ )
  {
    arguments[i].id = i;
  }

  printf("done.\n");
}

int
Read_Options(int argc, char **argv)
{
  char    *prog;

  prog = *argv;
  while (++argv, --argc > 0)
    if (**argv == '-')
      switch ( *++*argv ) {
        case 'n':
          --argc;
          N = atoi(*++argv);
          break;
        case 'h':
          printf("\nHELP: try sor -u \n\n");
          exit(0);
          break;
        case 'u':
          printf("\nUsage: gaussian [-n problemsize]\n");
          printf("           [-D] show default values \n");
          printf("           [-h] help \n");
          printf("           [-I init_type] fast/rand \n");
          printf("           [-m maxnum] max random no \n");
          printf("           [-P print_switch] 0/1 \n");
          exit(0);
          break;
        case 'D':
          printf("\nDefault:  n         = %d ", N);
          printf("\n          Init      = rand" );
          printf("\n          maxnum    = 5 ");
          printf("\n          P         = 0 \n\n");
          exit(0);
          break;
        case 'I':
          --argc;
          Init = *++argv;
          break;
        case 'm':
          --argc;
          maxnum = atoi(*++argv);
          break;
        case 'P':
          --argc;
          PRINT = atoi(*++argv);
          break;
        default:
          printf("%s: ignored option: -%s\n", prog, *argv);
          printf("HELP: try %s -u \n\n", prog);
          break;
      }
}
