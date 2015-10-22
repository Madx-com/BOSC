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
State *s;

// Mutex for access to state.
pthread_mutex_t state_mutex;

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
	int j, gg = 1;
	pthread_mutex_lock(&state_mutex);
	for(j = 0; j < n; j++)
	{
		// check that request is not greater than need
		if((s->max[i][j] - s->allocation[i][j]) < request[j])
		{
			printf("Unsafe! Maximum(%d) - Curr.Allocated(%d) < Request(%d)\n", s->max[i][j], s->allocation[i][j], request[j]);
			gg = 0;
			return gg;
		}
		// check if the requested resource is available
		if(request[j] > s->available[j])
		{
			printf("Unsafe! Request(%d) < Available(%d)\n", request[j], s->available[j]);
			gg = 0;
			return gg;
		}
	}
	
	if(gg == 1)
	{
		for(j = 0; j < n; j++)
		{
			s->allocation[i][j] += request[j];
			if((s->available[j] = s->max[i][j] - s->allocation[i][j]) < 0)
			{
				s->available[j] = 0;
			}
			printstate();
		}
		pthread_mutex_unlock(&state_mutex);
		return gg;
	}
	else
	{
		pthread_mutex_unlock(&state_mutex);
		return gg;
	}
}

/* Release the resources in request for process i */
void resource_release(int i, int *request)
{
	int j, k, availablesum = 0;
	pthread_mutex_lock(&state_mutex);
	for(j = 0; j < n; j++)
	{
		// recalculate allocation and need
		if((s->allocation[i][j] = s->allocation[i][j] - request[j]) < 0)
		{
			s->allocation[i][j] = 0;
		}
		// recalculate available
		for(k = 0; k < m; k++)
		{
			availablesum += s->allocation[k][j];
		}
		s->available[j] = s->resource[j] - availablesum;		
	}
	pthread_mutex_unlock(&state_mutex);
	printstate();
}

/* Generate a request vector */
void generate_request(int i, int *request)
{
	int j, sum = 1;
	while (!sum) 
	{
		printf("Request:\n");
		for (j = 0;j < n; j++) 
		{
			request[j] = s->need[i][j] * ((double)rand())/ (double)RAND_MAX;
	    	sum += request[j];
		}
	}
	printf("\nProcess %d: Requesting resources.\n",i);
}

/* Generate a release vector */
void generate_release(int i, int *request)
{
	int j, sum = 1;
	while (!sum) 
	{
		for (j = 0;j < n; j++) 
		{
			request[j] = s->allocation[i][j] * ((double)rand())/ (double)RAND_MAX;
			sum += request[j];
		}
	}
	printf("Process %d: Releasing resources.\n",i);
}

/* Threads starts here */
void *process_thread(void *param)
{
	/* Process number */
	int i = (int) (long) param, j;
	/* Allocate request vector */
	int *request = malloc(n * sizeof(int));
	while (1) 
	{
		/* Generate request */
		generate_request(i, request);
		while (!resource_request(i, request)) 
		{
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

int checksafety()
{
	int i, j, alloc, available[n], need;
	for(i = 0; i < m; i++)
	{
		alloc = 0;
		need = 0;
		for(j = 0; j < n; j++)
		{
			need = s->max[i][j] - s->allocation[i][j];
			// check need
			if(need != s->need[i][j])
			{
				printf("Initial -> Unsafe! Maximum(%d) - Curr.Allocated(%d) != Need(%d)\n", s->max[i][j], s->allocation[i][j], s->need[i][j]);
				return 0;
			}

			alloc += s->allocation[i][j];	
			available[j] = s->resource[j] - alloc;
					
			// check if available is correct
			if(available[j] != s->available[j])
			{
				printf("Initial -> Unsafe! Calculated Available(%d) != State Available(%d)\n",  available[j], s->available[j]);
				return 0;
			}
		}
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
}

int main(int argc, char* argv[])
{
	/* Get size of current state as input */
	int i, j;
	printf("Number of processes: ");
	scanf("%d", &m);
	printf("Number of resources: ");
	scanf("%d", &n);

	printf("\nP:%d R:%d\n", m, n);

	/* Allocate memory for state */
	s = (State *)malloc(sizeof(State));
	s->resource = (int *)malloc(sizeof(int) * n);
	s->available = (int *)malloc(sizeof(int) * n);
	s->max = (int **)malloc(sizeof(int *) * n);
	s->allocation = (int **)malloc(sizeof(int *) * n);
	s->need = (int **)malloc(sizeof(int *) * n);

	for(i = 0; i < m; i++)
	{
		s->max[i] = (int *)malloc(sizeof(int) * n);
		s->allocation[i] = (int *)malloc(sizeof(int) * n);
		s->need[i] = (int *)malloc(sizeof(int) * n);
	}

	/* Get current state as input */
	printf("Resource vector: ");
	for(i = 0; i < n; i++)
	{
		scanf("%d", &s->resource[i]);
	}

	printf("Enter max matrix: ");
	for(i = 0;i < m; i++)
	{
		for(j = 0;j < n; j++)
		{
			scanf("%d", &s->max[i][j]);
		}
	}

	printf("Enter allocation matrix: ");
	for(i = 0; i < m; i++)
	{
		for(j = 0; j < n; j++) 
		{
			scanf("%d", &s->allocation[i][j]);
		}
	}

	printf("\n");
	/* Calcuate the need matrix */
	for(i = 0; i < m; i++)
	{
		for(j = 0; j < n; j++)
		{
			s->need[i][j] = s->max[i][j] - s->allocation[i][j];
		}
	}

	/* Calcuate the availability vector */
	for(j = 0; j < n; j++) 
	{
		int sum = 0;
		for(i = 0; i < m; i++)
		{
			sum += s->allocation[i][j];
		}
		s->available[j] = s->resource[j] - sum;
	}

	/* Output need matrix and availability vector */
	printf("Need matrix:\n");
	for(i = 0; i < n; i++)
	{
		printf("R%d ", i+1);
	}
	printf("\n");

	for(i = 0; i < m; i++) 
	{
		for(j = 0; j < n; j++)
		{
			printf("%d  ",s->need[i][j]);
		}
		printf("\n");
	}
	printf("\n");

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

	/* If initial state is unsafe then terminate with error */
	int r = checksafety();
	if(r != 1)
	{
		exit(1);
	}

	/* Seed the random number generator */
	struct timeval tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);
  
	/* Create m threads */
	pthread_t *tid = malloc(m*sizeof(pthread_t));
	for (i = 0; i < m; i++)
	{
		pthread_create(&tid[i], NULL, process_thread, (void *) (long) i);
	}

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
