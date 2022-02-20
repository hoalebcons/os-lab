#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

void check(long points, long* count) {
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
    check(points, &count);
    printf("PI: %f\n", 4.0 * count / atol(argv[1]));
    clock_t endTime = clock();
    printf("Time: %f seconds\n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
    return 0;
}