#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *work(void *arg){
	// 1
	// int id = (int) arg;
	
	// 2
	// int id = *((int *) arg);
	
	// 3
	int id = *((int *) arg);	
	free(arg);

	printf("Hello from thread %d\n", id);

	return (void *) (id * 2);
}

int main() {
	pthread_t threads[10];
	for(int i = 0; i < 10; i++) {
		// 1
		// pthread_create(&threads[i], NULL, work, (void *) i);
		
		// 2
		// pthread_create(&threads[i], NULL, work, &i);
		
		// 3
		int *p = malloc(sizeof(int));
		*p = i;
		pthread_create(&threads[i], NULL, work, p);
	}

	for(int i = 0; i < 10; i++) {
		void *res;
		pthread_join(threads[i], &res);
		printf("Joined %d\n", (int) res);
	}
	return 0;
}
