#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 

#define NO_PHIL 9 
#define HUNGRY_PHIL 1 
#define THINKING_PHIL 2 
#define EATING_PHIL 0 
#define LEFT (phnum + 4) % NO_PHIL
#define RIGHT (phnum + 1) % NO_PHIL

int state[NO_PHIL]; 
// int phil[NO_PHIL] = { 0, 1, 2, 3, 4 ,5,6,7,8}; 
int phil[NO_PHIL];

sem_t mutex; 
sem_t S[NO_PHIL];

// Function to test if the forks are available.If they are available, the philosophereats
void eat(int phnum) 
{ 
	if (state[phnum] == HUNGRY_PHIL 
		&& state[LEFT] != EATING_PHIL 
		&& state[RIGHT] != EATING_PHIL) { 
		// state that eating 
		state[phnum] = EATING_PHIL; 

		sleep(2); 

		printf("Philosopher %d has taken fork %d and %d\n", 
					phnum + 1, LEFT + 1, phnum + 1); 

		printf("Philosopher %d is Eating\n", phnum + 1); 

		sem_post(&S[phnum]); 	// Waking up hungry philosophers 
	} 
} 

// take up chopsticks 
void take_fork(int phnum) 
{ 

	sem_wait(&mutex); 

	// state that hungry 
	state[phnum] = HUNGRY_PHIL; 

	printf("Philosopher %d is Hungry\n", phnum + 1); 

	// eat if neighbours are not eating 
	eat(phnum); 
// 
	sem_post(&mutex); 

	// if unable to eat wait to be signalled--when all resources are free 
	sem_wait(&S[phnum]); 

	sleep(1); 
} 

// put down chopsticks 
void put_fork(int phnum) 
{ 

	sem_wait(&mutex); 

	// state that thinking 
	state[phnum] = THINKING_PHIL; 

	printf("Philosopher %d putting fork %d and %d down\n", 
		phnum + 1, LEFT + 1, phnum + 1); 
	printf("Philosopher %d is thinking\n", phnum + 1); 

	eat(LEFT); 
	eat(RIGHT); 

	sem_post(&mutex); 
} 

void* philospher(void* num) 
{ 

	while (1) { 

		int* i = num; 

		sleep(1); 

		take_fork(*i); 

		sleep(0); 

		put_fork(*i); 
	} 
} 

int main() 
{ 

// dynamically create N philosophers
for(int xx=0; xx < NO_PHIL; xx++){
phil[xx] = xx;
}

	int i; 

  // Initialize threads for each philosoper
	pthread_t thread_IDs[NO_PHIL]; 

	// initialize the semaphores 
	sem_init(&mutex, 0, 1); 

	for (i = 0; i < NO_PHIL; i++) 

		sem_init(&S[i], 0, 0); 

	for (i = 0; i < NO_PHIL; i++) { 

		// create philosopher processes 
		pthread_create(&thread_IDs[i], NULL, 
					philospher, &phil[i]); 

		printf("Philosopher %d is thinking\n", i + 1); 
	} 

	for (i = 0; i < NO_PHIL; i++) 

		pthread_join(thread_IDs[i], NULL); 
} 
