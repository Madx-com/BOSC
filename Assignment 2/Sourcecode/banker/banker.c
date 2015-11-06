#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct state {
  int *resource;
  int *available;
  int **max;
  int **allocation;
  int **need;
} State;

// Global variables
int m, n;
State *s = NULL;

// Mutex for access to state.
pthread_mutex_t state_mutex;

// print availbility vector
void printstate();

/* Random sleep function */
void Sleep(float wait_time_ms)
{
  // add randomness
  wait_time_ms = ((float)rand())*wait_time_ms / (float)RAND_MAX;
  usleep((int) (wait_time_ms * 1e3f)); // convert from ms to us
}

/* Allocate resources in request for process i, only if it 
   results in a safe state and return 1, else return 0 */
int resource_request(int i, int *request)
{
	int j, safe = 0, safeAvailable[n], safeNeed[m][n], safeAllocation[m][n];	

	pthread_mutex_lock(&state_mutex);
	printf("Process %d request vector: ", i);
	for(j = 0; j < n; j++)
	{
		printf("%d ", request[j]);
	}
	printf("\n");

	// check if resource request is eligible for allocation
	for(j = 0; j < n; j++)
	{
		if(request[j] <= s->need[i][j])
		{		
			if(request[j] <= s->available[j])
			{
				continue;
			}
			else
			{
				printf("Not enough available resources(%d) for request(%d) try again later...\n", s->available[j], request[j]);
				pthread_mutex_unlock(&state_mutex);
				return 0;	
			}
		}
		else
		{
			printf("Request greater than need!\n");
			pthread_mutex_unlock(&state_mutex);
  		return 0;
		}
	}

	// allocate resources
	for(j = 0; j < n; j++)
	{
		// backup of current resources	
		safeAvailable[j] = s->available[j];
		safeNeed[i][j] = s->need[i][j];
		safeAllocation[i][j] = s->allocation[i][j];
	
		s->available[j] = s->available[j] - request[j];
		s->allocation[i][j] = s->allocation[i][j] + request[j];
		s->need[i][j] = s->max[i][j] - s->allocation[i][j];
	}

	//check state
	safe = checksafety();
	if(safe != 1)
	{
		for(j = 0; j < n; j++)
		{
			s->available[j] = safeAvailable[j];
			s->need[i][j] = safeNeed[i][j];
			s->allocation[i][j] = safeAllocation[i][j];
		}				
		printf("Request leads to unsafe state. Request Denied!\n");
		pthread_mutex_unlock(&state_mutex);
		return 0;		
	}
	else
	{
		printf("Request leads to safe state. Request Granted!\n");
		printf("Vector changed: ");
		printstate();					
		pthread_mutex_unlock(&state_mutex);
		return 1;
	}
}

/* Release the resources in request for process i */
void resource_release(int i, int *request)
{
	int j;
	pthread_mutex_lock(&state_mutex);
	printf("Process %d release vector: ", i);
	for(j = 0; j < n; j++)
	{
		printf("%d ", request[j]);
	}
	printf("\n");

	for(j = 0; j < n; j++)
	{
		// check if release request is less than the allocated resources
		if(request[j] <= s->allocation[i][j])
		{
			continue;
		}
		else
		{
			printf("Release request is more than the allocated!\n");
			pthread_mutex_unlock(&state_mutex);
			return;
		}
	}

	for(j = 0; j < n; j++)
	{
		// release the resources
		s->allocation[i][j] = s->allocation[i][j] - request[j];
		s->need[i][j] = s->max[i][j] - s->allocation[i][j];
		s->available[j] = s->available[j] + request[j];
	}
	printf("Released resources\n");
	printstate();
	pthread_mutex_unlock(&state_mutex);
}

/* Generate a request vector */
void generate_request(int i, int *request)
{
  int j, sum = 0;
  while (!sum) {
    for (j = 0;j < n; j++) {
      request[j] = s->need[i][j] * ((double)rand())/ (double)RAND_MAX;
      sum += request[j];
    }
  }
  printf("\nProcess %d: Requesting resources.\n",i);
}

/* Generate a release vector */
void generate_release(int i, int *request)
{
  int j, sum = 0;
  while (!sum) {
    for (j = 0;j < n; j++) {
      request[j] = s->allocation[i][j] * ((double)rand())/ (double)RAND_MAX;
      sum += request[j];
    }
  }
  printf("\nProcess %d: Releasing resources.\n",i);
}

int checksafety()
{
	int i, j, work[n], finish[m];

	for(i = 0; i < m; i++)
	{
		finish[i] = 0;
	}
	for(i = 0; i < n; i++)
	{
		work[i] = s->available[i];
	}

	i = 0;
	int dorepeat = 0;
	while(i < m)
	{
		printf("Checking safety of process %d\n", i);
		if(finish[i] == 0)
		{
			// flag for running process
			int check = 1;
			for(j = 0; j < n; j++)
			{
				if(s->need[i][j] > work[j])
				{
					check = 0;
				}
			}
			
			// simulate run of process
			if(check == 1)
			{
				for(j = 0; j < n; j++)
				{
					work[j] = work[j] + s->allocation[i][j];
				}
				finish[i] = 1;
				dorepeat = 1;
			}
		}

		i++;
		if(i == m && dorepeat == 1) 
		{
			i = 0;
			dorepeat = 0;
		}
	}

	for(i = 0; i < m; i++)
	{
		if(finish[i] == 0) return 0;
	}
	return 1;

}

void printstate()
{
	int i, j;
	printf("Availability vector:\n");
	for(i = 0; i < n; i++)
	{
		printf("R%d ", i+1);
	}
	printf("\n");

	for(j = 0; j < n; j++)
	{
		printf("%d  ",s->available[j]);
	}
	printf("\n");

	printf("Allocaton Matrix:\n");
	for(i = 0; i < m; i++)
	{
		for(j = 0; j < n; j++)
		{
			printf("%d ", s->allocation[i][j]);
		}
		printf("\n");
	}

	printf("Need Matrix:\n");
	for(i = 0; i < m; i++)
	{
		for(j = 0; j < n; j++)
		{
			printf("%d ", s->need[i][j]);
		}
		printf("\n");
	}
}

/* Threads starts here */
void *process_thread(void *param)
{
  /* Process number */
  int i = (int) (long) param, j;
  /* Allocate request vector */
  int *request = malloc(n*sizeof(int));
  while (1) {
    /* Generate request */
    generate_request(i, request);
    while (!resource_request(i, request)) {
      /* Wait */
      Sleep(100);
    }
    /* Generate release */
    generate_release(i, request);
    /* Release resources */
    resource_release(i, request);
    /* Wait */
    Sleep(1000);
  }
  free(request);
}

int main(int argc, char* argv[])
{
  /* Get size of current state as input */
  int i, j;
  printf("Number of processes: ");
  scanf("%d", &m);
  printf("Number of resources: ");
  scanf("%d", &n);

  /* Allocate memory for state */
  s = (State *)malloc(sizeof(State));
  s->resource = (int *)malloc(sizeof(int) * n);
  s->available = (int *)malloc(sizeof(int) * n);
  s->max = (int **)malloc(sizeof(int *) * n);
  s->allocation = (int **)malloc(sizeof(int *) * n);
  s->need = (int **)malloc(sizeof(int *) * n);

  for(i = 0; i < m; i++)
  {
		s->max[i] = (int *)malloc(sizeof(int) * m);
		s->allocation[i] = (int *)malloc(sizeof(int) * m);
		s->need[i] = (int *)malloc(sizeof(int) * m);
  }
  
  /* Get current state as input */
  printf("Resource vector: ");
  for(i = 0; i < n; i++)
    scanf("%d", &s->resource[i]);
  printf("Enter max matrix: ");
  for(i = 0;i < m; i++)
    for(j = 0;j < n; j++)
      scanf("%d", &s->max[i][j]);
  printf("Enter allocation matrix: ");
  for(i = 0; i < m; i++)
    for(j = 0; j < n; j++) {
      scanf("%d", &s->allocation[i][j]);
    }
  printf("\n");

  /* Calcuate the need matrix */
  for(i = 0; i < m; i++)
    for(j = 0; j < n; j++)
      s->need[i][j] = s->max[i][j]-s->allocation[i][j];

  /* Calcuate the availability vector */
  for(j = 0; j < n; j++) {
    int sum = 0;
    for(i = 0; i < m; i++)
      sum += s->allocation[i][j];
    s->available[j] = s->resource[j] - sum;
  }

  /* Output need matrix and availability vector */
  printf("Need matrix:\n");
  for(i = 0; i < n; i++)
    printf("R%d ", i+1);
  printf("\n");
  for(i = 0; i < m; i++) {
    for(j = 0; j < n; j++)
      printf("%d  ",s->need[i][j]);
    printf("\n");
  }
  printf("Availability vector:\n");
  for(i = 0; i < n; i++)
    printf("R%d ", i+1);
  printf("\n");
  for(j = 0; j < n; j++)
    printf("%d  ",s->available[j]);
  printf("\n");

  /* If initial state is unsafe then terminate with error */
	int r = checksafety();
	if(r != 1)
	{
		printf("Initial state unsafe!\n");
		exit(1);
	}
  printf("Initial state safe!\n");

	/* Seed the random number generator */
  struct timeval tv;
  gettimeofday(&tv, NULL);
  srand(tv.tv_usec);
  
  /* Create m threads */
  pthread_t *tid = malloc(m*sizeof(pthread_t));
  for (i = 0; i < m; i++)
    pthread_create(&tid[i], NULL, process_thread, (void *) (long) i);
  
  /* Wait for threads to finish */
  pthread_exit(0);
  free(tid);

  /* Free state memory */
	free(s->resource);
	free(s->available);
	for(i = 0; i < m; i++)
	{
		free(s->max[i]);
		free(s->allocation[i]);
		free(s->need[i]);
	}
	free(s->max);
	free(s->allocation);
	free(s->need);
	free(s);
}
