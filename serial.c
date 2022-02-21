#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void circle_point();
static long int total_point;
static long int count_circle=0;
clock_t start_time, end_time;

int main(int argc, char *argv[]){
    if(argc==1){
       printf("Enter number point\n");
       return -1;
    }

    if(argc!=2){
       printf("Argument is wrong\n");
       return -1;
    }
   total_point=atoll(argv[1]);

   start_time=clock();
   circle_point();
   double pi=4.0*(double)count_circle/(double)total_point;
   end_time=clock();

   printf("PI = %17.15f\n",pi);
   printf("Time to compute= %g second\n",(double)(end_time-start_time)/CLOCKS_PER_SEC);
   return 0;
}

void circle_point(){
   srand(time(NULL));
   int i;
   for(i=0;i<total_point;i++){
       double x= (double)rand()/(double)RAND_MAX;
       double y=(double)rand()/(double)RAND_MAX;
       double r= sqrt(x*x+y*y);
       if(r<=1) count_circle+=1;
    }
}