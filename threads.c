/**
 * Takes an input file with one number/line and an integer and computes the
 * average between all read numbers, divided in integer number of threads
 *
 * Must be compiled: gcc -D_REENTRANT -lpthread threads.c
 * And sample txt file created: bash threads.sh
 * And run: ./a.out threads.txt <number-of-threads>
 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <fcntl.h>


struct limit {
    int from, to;
};


int numbers[5000], count = 0;
float averages[5000];
struct limit limits[5000];


void *do_average(void *arg) {
    // Get limit from passed argument
    int limit_id = (int)arg;

    // Compute average for `numbers` array, within `limits` limits
    int sum = 0, count = 0, i;
    for (i = limits[limit_id].from; i <= limits[limit_id].to; i++) {
        sum += numbers[i];
        count++;
    }

    averages[limit_id] = sum / count;

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Not enough arguments\n");
        exit(1);
    }

    int nof_threads = atoi(argv[2]);

    // Open input file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Given file \"%s\" could not be opened\n", argv[1]);
        exit(1);
    }

    // Save each number from file to numbers
    int number;
    while (fscanf(file, "%d", &number) == 1) {
        numbers[count++] = number;
    }

    // Equally split numbers to threads for parallel computing
    int numbers_per_thread = count / nof_threads;

    // Initialize threads
    pthread_t *threads = malloc(nof_threads * sizeof(pthread_t));

    int i;

    // Create given number of threads
    for (i = 0; i < nof_threads; i++) {
        limits[i].from = i * numbers_per_thread;
        limits[i].to = (i+1) * numbers_per_thread - 1;

        if (pthread_create(&threads[i], NULL, do_average, (void *)i) != 0) {
            printf("Thread \"%d\" could not be created\n", i);
            exit(1);
        }
    }

    // Join created threads (wait for each thread to terminate before proceeding)
    for (i = 0; i < nof_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            printf("Could not join thread \"%d\"\n", i);
            exit(1);
        }
    }

    // Compute overall average
    float sum = 0;
    for (i = 0; i < nof_threads; i++) {
        sum += averages[i];
    }

    printf("Average for given numbers is %.2f\n", (sum/nof_threads));

    return 0;
}
