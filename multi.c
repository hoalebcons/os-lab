#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
struct thread {
    int nPoints;
    int hits;
};
double getRandomNumber() {
    return (double)rand() / RAND_MAX * 2 - 1;
}
void* pi_thread(void* arg) {
    struct thread* t = (struct thread*)arg;
    for(int i = 0; i < t->nPoints; i++) {
        double x = getRandomNumber();
        double y = getRandomNumber();
        if(x*x + y*y <= 1) {
            t->hits++;
        }
    }
    return NULL;
}
int main(int argc, char **argv) {
    srand(time(NULL));
    int n = 10;
    int nPoints = atoi(argv[1]);
    int pointEachThread = nPoints / n;
    pthread_t* threads = malloc(n * sizeof(pthread_t));
    struct thread* args = malloc(n * sizeof(struct thread));
    int i;
    for(i = 0; i < n; i++) {
        args[i].nPoints = pointEachThread;
        args[i].hits = 0;
        pthread_create(&threads[i], NULL, pi_thread, &args[i]);
    }
    for(i = 0 ; i < n; i++) {
        pthread_join(threads[i], NULL);
    }
    double pi = 0;
    for(i = 0; i < n; i++) {
        pi += args[i].hits;
    }
    pi = 4*((double)pi/(double)nPoints);
    printf("pi = %f\n", pi);
    free(threads);
    free(args);
    pthread_exit(NULL);
    return 0;
}