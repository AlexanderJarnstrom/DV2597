/***************************************************************************
 *
 * Sequential version of Quick sort
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define KILO (1024)
#define MEGA (1024*1024)
#define MAX_ITEMS (64*MEGA)
#define swap(v, a, b) {unsigned tmp; tmp=v[a]; v[a]=v[b]; v[b]=tmp;}
#define THREADS 16
#define LINE MAX_ITEMS / THREADS

typedef struct {
  int *v;
  unsigned low;
  unsigned high;
} argument_t;

static int *v;
sem_t sem;

static void
print_array(void)
{
  int i;
  for (i = 0; i < MAX_ITEMS; i++)
    printf("%d ", v[i]);
  printf("\n");
}

static void
init_array(void)
{
  int i;
  v = (int *) malloc(MAX_ITEMS*sizeof(int));
  for (i = 0; i < MAX_ITEMS; i++)
    v[i] = rand();
}

static unsigned
partition(int *v, unsigned low, unsigned high, unsigned pivot_index)
{
  /* move pivot to the bottom of the vector */
  if (pivot_index != low)
    swap(v, low, pivot_index);

  pivot_index = low;
  low++;

  /* invariant:
   * v[i] for i less than low are less than or equal to pivot
   * v[i] for i greater than high are greater than pivot
   */

  /* move elements into place */
  while (low <= high) {
    if (v[low] <= v[pivot_index])
      low++;
    else if (v[high] > v[pivot_index])
      high--;
    else
      swap(v, low, high);
  }

  /* put pivot back between two groups */
  if (high != pivot_index)
    swap(v, pivot_index, high);
  return high;
}

static void*
quick_sort(void *_arg)
{
  argument_t arg, low_arg, high_arg;
  unsigned pivot_index;
  pthread_t low_thread, high_thread;
  int sem_v;
  int created_low, created_high, size_low, size_high;

  arg = *(argument_t*) _arg;
  created_low = created_high = size_low = size_high= 0;

  /* no need to sort a vector of zero or one element */
  if (arg.low >= arg.high)
    return NULL;

  /* select the pivot value */
  pivot_index = (arg.low + arg.high)/2;

  /* partition the vector */
  pivot_index = partition(v, arg.low, arg.high, pivot_index);

  /* sort the two sub arrays */
  if (arg.low < pivot_index)
  {
    low_arg.v = arg.v;
    low_arg.low = arg.low;
    low_arg.high = pivot_index - 1;
    size_low = low_arg.high - low_arg.low;

    if (size_low > LINE && sem_trywait(&sem) == 0)
    {
      pthread_create(&low_thread, NULL, quick_sort, &low_arg);
      created_low = 1;
    }
    else 
    {
      quick_sort(&low_arg);
    }
  }

  if (pivot_index < arg.high)
  {
    high_arg.v = arg.v;
    high_arg.low = pivot_index + 1;
    high_arg.high = arg.high;
    size_high = high_arg.high - high_arg.low;

    if (size_high > LINE && sem_trywait(&sem) == 0)
    {
      pthread_create(&high_thread, NULL, quick_sort, &high_arg);
      created_high = 1;
    }
    else 
    {
      quick_sort(&high_arg);
    }
  }

  if (size_low < size_high)
  {
    if (created_high)
    { 
      pthread_join(high_thread, NULL);
      sem_post(&sem);
    }
    if (created_low)
    {
      pthread_join(low_thread, NULL);
      sem_post(&sem);
    }
  }
  else
  {
    if (created_low)
    {
      pthread_join(low_thread, NULL);
      sem_post(&sem);
    }
    if (created_high)
    { 
      pthread_join(high_thread, NULL);
      sem_post(&sem);
    }
  }

  

  return NULL;
}

void
validate()
{
  int i;
  for (i = 1; i < MAX_ITEMS; i++)
  {
    if (v[i - 1] > v[i])
      printf("Wrong order.\n");
  }
}

int
main(int argc, char **argv)
{
  time_t start, end;
  argument_t arg = {v, 0, MAX_ITEMS-1};
  sem_init(&sem, 0, THREADS);

  init_array();
  //print_array();
  time(&start);
  quick_sort((void*) &arg);
  time(&end);

  printf("%lds\n", end - start);
  //validate();
  //print_array();
}
