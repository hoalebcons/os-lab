#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

void generate(long points, long* count) {
    srand(time(NULL));
    for (long i = 0; i < points; i++) {
        double x = (double)rand() / (double)RAND_MAX;
        double y = (double)rand() / (double)RAND_MAX;
        if (sqrt(x * x + y * y) <= 1) {
            *count = *count + 1;
        }
    }
}

int main(int argc, char* argv[])
{
    clock_t startTime = clock();
    long count = 0;
    long points = atol(argv[1]);
    generate(points, &count);
    
    double pi = 4.0 * count / atol(argv[1]);
    clock_t endTime = clock();
    double time_spent = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    
    
    printf("Pi: %lf\nTime: %lf seconds\n",pi,time_spent);
    return 0;

}
