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
	int j, res = 1, avail;
	pthread_mutex_lock(&state_mutex);
	for(j = 0; j < n; j++)
	{
		if((s->max[i][j] - s->allocation[i][j]) < request[j])
		{
			// need is less than request
			printf("Unsafe! Maximum(%d) - Curr.Allocated(%d) < Request(%d)\n", s->max[i][j], s->allocation[i][j], request[j]);
			res = 0;
			pthread_mutex_unlock(&state_mutex);
			return res;
		}

		avail = s->resource[j] - (s->allocation[i][j] + request[i]);
		if(avail > s->available[j])
		{
			// not enough available resources
			printf("Unsafe! Request(%d) > Available(%d)\n", request[j], s->available[j]);
			res = 0;
			pthread_mutex_unlock(&state_mutex);
			return res;
		}
	}
	
	if(res == 1)
	{
		// request granted for now
		int safe = 0;
		int alloc[n], available[n], need[n];
		for(j = 0; j < n; j++)
		{
			// copy current values
			alloc[j] = s->allocation[i][j];
			available[j] = s->available[j];
			need[j] = s->need[i][j];
			// assign new values 
			s->allocation[i][j] += request[j];
			s->need[i][j] = s->max[i][j] - s->allocation[i][j];
			available[j] = s->resource[j] - s->allocation[i][j];
		}
		// simulate to check safety of processes
		if((checksafety()) == 1)
		{
			printf("Resources allocated safely!\n");
			printstate();
			pthread_mutex_unlock(&state_mutex);
			return 1;
		}
		else
		{
			printf("Resources allocated unsafely! Reverting allocation...\n");
			for(j = 0; j < n; j++)
			{
				s->allocation[i][j] = alloc[j];
				s->available[j] = available[j];
				s->need[i][j] = need[j]; 
			}
			printstate();
			pthread_mutex_unlock(&state_mutex);
			return 0;
		}
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
		if((s->need[i][j] = s->max[i][j] - s->allocation[i][j]) < 0)
		{
			s->need[i][j] = 0;
		}
		// recalculate available
		for(k = 0; k < m; k++)
		{
			availablesum += s->allocation[k][j];
		}
		s->available[j] = s->resource[j] - availablesum;		
	}
	printstate();
	pthread_mutex_unlock(&state_mutex);
}

/* Generate a request vector */
void generate_request(int i, int *request)
{
	int j, sum = 0;
	while (!sum) 
	{
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
	int j, sum = 0;
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

/* simulates the current states to check the safety of all processes */
int checksafety()
{
	int c = m, executing[m], i, j, issafe, avail[n], alloc[m][n], need[m][n], availablesum, needs = 1;
	// copy variables
	for(j = 0; j < n; j++)
	{
		availablesum = 0;
		for(i = 0; i < m; i++)
		{
			executing[i] = 1;
			need[i][j] = s->max[i][j] - s->allocation[i][j];
			alloc[i][j] = s->allocation[i][j];
			availablesum += alloc[i][j];
		}
		avail[j] = s->resource[j] - availablesum;
	}

	// run simulation
	for (i = 0; i < m; i++) 
	{
		issafe = 0;
        if (executing[i]) 
		{
            for (j = 0; j < n; j++) 
			{
				// check if need exceeds available
                if (need[i][j] > avail[j]) 
				{
                    needs = 0;
                    break;
				}
			}
            if (needs) 
			{
				// simulate execution
                executing[i] = 0;
                c -= 1;

				// it can execute so it's safe
                issafe = 1;

                for (j = 0; j < n; j++) {
                    avail[j] += alloc[i][j];
                }

                break;
            }
        }
    }
	// check safety
    if (!issafe) 
	{
        printf("Unsafe state found.\n");
        return 0;
    }
	else
	{
		printf("Everything is good!\n");
		return 1;
	}
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
		s->max[i] = (int *)malloc(sizeof(int) * m);
		s->allocation[i] = (int *)malloc(sizeof(int) * m);
		s->need[i] = (int *)malloc(sizeof(int) * m);
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
