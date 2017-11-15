/*
Manatee checks whether the expected values of two datasets are significantly different.

Copyright (c) 2016 Elod Csirmaz

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

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
T_DATA *DATA; /* Data points from both sets */
T_DATA SUM_1 = 0; /* Sum of data in set 1 */
T_DATA SUM_2 = 0; /* Sum of data in set 2 */

T_SIZE NUM_EXPERIMENTS;

/* Print a message and exit */
void die(char *msg)
{
    printf(msg);
    exit(1);
}

/* Load two datasets from STDIN */
void load_data(void)
{
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

/* Return the sum of n random data points with replacement */
T_DATA get_average_of_n(T_SIZE n)
{
    T_DATA sum = 0, x;
    T_SIZE i, j;

    for(i=0; i<n; i++){
        j = SIZE;
        while(j >= SIZE){ j = (rand() & RNDMASK); }
        sum += DATA[j];
    }
    return sum/((T_DATA)n);
}

/* 
 * Get differences between the expected values (averages) of bootstrapped data sets
 * of the size SIZE_1 and SIZE_2 from the combined dataset.
 * Then count how many times we get a difference at least as large as the difference between 
 * the expected values of dataset 1 and 2, and return their ratio.
 */
T_DATA do_experiments(T_DATA average1, T_DATA average2)
{
    T_SIZE e, larger_count = 0;
    T_DATA x, limit;
    
    limit = fabs(average2 - average1);
    printf("Average of set 1:%lf Average of set 2:%lf Difference:%lf\n", average1, average2, limit);
    
    for(e=0; e<NUM_EXPERIMENTS; e++){
        x = fabs(get_average_of_n(SIZE_2) - get_average_of_n(SIZE_1));
        
        if(x >= limit){
            larger_count++;
        }
    }
    
    return ((T_DATA)larger_count)/((T_DATA)NUM_EXPERIMENTS);
}

int main(void)
{
    srand(time(NULL));
    
    load_data();
    
    printf("Ratio of experiments that yielded the same or a larger difference\nthan that between the averages of the original sets:\nR=%lf\n",
        do_experiments(SUM_1/((T_DATA)SIZE_1), SUM_2/((T_DATA)SIZE_2))
    );
    return 0;
}
