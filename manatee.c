
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define T_DATA double
#define T_SIZE int

T_SIZE SIZE_1; /* Size of data set 1 */
T_SIZE SIZE_2; /* Size of data set 2 */
T_SIZE SIZE; /* Sum of sizes */
T_SIZE RNDMASK;
T_DATA *DATA;
T_DATA SUM_1 = 0; /* Sum of data in set 1 */
T_DATA SUM_2 = 0; /* Sum of data in set 2 */

T_SIZE NUM_EXPERIMENTS;

void die(char *msg)
{
    printf(msg);
    exit(1);
}

/* Load two datasets from STDIN */
void load_data(void)
{
    /*
     * Read from STDIN:
     * size of data set 1 (integer)
     * size of data set 2 (integer)
     * number of experiments (integer)
     * data points of set 1 (float)
     * data points of set 2 (float)
     */
    T_SIZE i;
    T_DATA d;

    if(scanf("%d", &SIZE_1) < 0){ die("Error reading size of dataset 1\n"); }
    if(scanf("%d", &SIZE_2) < 0){ die("Error reading size of dataset 2\n"); }
    SIZE = SIZE_1 + SIZE_2;
    DATA = malloc(sizeof(T_DATA) * SIZE);
    if(DATA == NULL){ die("Cannot allocate memory\n"); }
    
    /* Get the smallest mask of the form (2**n-1) that is more than SIZE */
    RNDMASK = 1;
    while(RNDMASK < SIZE){ RNDMASK <<= 1; RNDMASK++; }

    if(scanf("%d", &NUM_EXPERIMENTS) < 0){ die("Error reading number of experiments\n"); }
    
    printf("Size1:%d Size2:%d Exp:%d Mask:%d\n", SIZE_1, SIZE_2, NUM_EXPERIMENTS, RNDMASK);
    
    for(i=0; i<SIZE_1; i++){
        if(scanf("%lf", &d) < 0){ die("Error reading data point\n"); }
        SUM_1 += d;
        DATA[i] = d;
        // printf("Set 1, %d: %lf\n", i, d);
    }
    for(i=0; i<SIZE_2; i++){
        if(scanf("%lf", &d) < 0){ die("Error reading data point\n"); }
        SUM_2 += d;
        DATA[i + SIZE_1] = d;
        // printf("Set 2, %d: %lf\n", i, d);
    }
    printf("Sum1:%lf Sum2:%lf\n", SUM_1, SUM_2);
}

/* Return the average of n random data points */
T_DATA get_average_of_n(T_SIZE n)
{
    T_DATA sum = 0, x;
    T_SIZE i, j;

    for(i=0; i<n; i++){
        // for(j=0; j<SIZE; j++){ printf("%lf ",DATA[j]); } printf("\n");
        j = SIZE;
        while(j >= SIZE - i){ j = (rand() & RNDMASK); }
        j += i;
        sum += DATA[j];
        // printf("  Of %d, selected %d: %lf\n", n, j, DATA[j]);
        if(i != j){
            x = DATA[i];
            DATA[i] = DATA[j];
            DATA[j] = x;
        }
    }
    // printf(" Sum:%lf\n", sum);
    return sum/((T_DATA)n);
}

/* 
 * Get sample differences between SIZE_1 and SIZE_2 random points from the combined dataset.
 * Then count how many differences are larger than the difference between the expected values
 * (averages) of dataset 1 and 2, and return their ratio.
 */
T_DATA do_experiments(T_DATA average1, T_DATA average2)
{
    T_SIZE e, larger_count = 0;
    T_DATA x, limit;
    
    limit = fabs(average2 - average1);
    printf("Avg1:%lf Avg2:%lf Limit:%lf\n", average1, average2, limit);
    
    for(e=0; e<NUM_EXPERIMENTS; e++){
         x = fabs(get_average_of_n(SIZE_2) - get_average_of_n(SIZE_1));
         // printf("Experiment %d: %lf\n", e, x);
         if(x > limit){
            larger_count++;
         }
    }
    
    return ((T_DATA)larger_count)/((T_DATA)NUM_EXPERIMENTS);
}

int main(void)
{
    srand(time(NULL));
    
    load_data();
    
    printf("Ratio of experiments that yielded a larger distance than that between the sets: %lf\n",
        do_experiments(SUM_1/((T_DATA)SIZE_1), SUM_2/((T_DATA)SIZE_2))
    );
    return 0;
}
