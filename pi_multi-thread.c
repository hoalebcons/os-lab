#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<math.h>
#include<time.h>

#define NUM_THREAD 4

long counts[NUM_THREAD];
pthread_t thread[NUM_THREAD];
long total_point;

void* generate(void* x) {
    srand(time(NULL));
    long* count = (long*)x;
    long points = total_point / NUM_THREAD;
    for (long i = 0; i < points; i++) {
        double x = (double)rand() / (double)RAND_MAX;
        double y = (double)rand() / (double)RAND_MAX;
        if (sqrt(x * x + y * y) <= 1) {
            *count = *count + 1;
        }
    }
    pthread_exit(0);
}
int main(int argc, char* argv[]) {
    clock_t startTime = clock();
    long counter = 0;
    total_point = atol(argv[1]);
    for (long i = 0; i < NUM_THREAD; i++) {
        pthread_create(&thread[i], NULL, generate, &counts[i]);
    }
    for (long i = 0; i < NUM_THREAD; i++) {
        pthread_join(thread[i], NULL);
        counter += counts[i];
    }
    
    double pi = 4.0 * counter / atol(argv[1]);
    clock_t endTime = clock();
    double time_spent = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    
    
    printf("Pi: %lf\nTime: %lf seconds\n",pi,time_spent);
    pthread_exit(NULL);
    return 0;

}
