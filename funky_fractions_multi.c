#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

#define LIMIT 1000000000
#define NUM_THREADS 8


void format_with_commas(char* out, int n) {
    char buffer[50];
    sprintf(buffer, "%d", n);

    int buffer_len = strlen(buffer);
    int comma_count = (buffer_len - 1) / 3;

    char* p = out;
    for (int i = 0; i < buffer_len; ++i) {
        *p++ = buffer[i];
        if (i < buffer_len - 1 && (buffer_len - 1 - i) % 3 == 0) {
            *p++ = ',';
        }
    }
    *p = '\0';  // Null-terminate the string
}

int digits(int n) {
    if (n < 10) return 1;
    else if (n < 100) return 2;
    else if (n < 1000) return 3;
    else if (n < 10000) return 4;
    else if (n < 100000) return 5;
    else if (n < 1000000) return 6;
    else if (n < 10000000) return 7;
    else if (n < 100000000) return 8;
    else if (n < 1000000000) return 9;
    else return 10;
}

int is_funky_fraction(int denominator) {
    int denominator_digits = digits(denominator);
    int powers_of_10[11] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

    for (int cut_index = 1; cut_index < denominator_digits; ++cut_index) {
        int divisor = powers_of_10[cut_index];
        int reduced_numerator_multiplier = powers_of_10[denominator_digits - cut_index];

        int overlap = denominator / divisor;
        int reduced_denominator = denominator % divisor;

        if (reduced_denominator == 0 || digits(reduced_denominator) + 1 < cut_index) {
            continue;
        }

        int overlap_times_reduced_denominator = overlap * reduced_denominator;
        int denominator_minus_reduced_denominator_times_multiplier = denominator - reduced_denominator * reduced_numerator_multiplier;

        if (overlap_times_reduced_denominator % denominator_minus_reduced_denominator_times_multiplier == 0) {
            int reduced_numerator = overlap_times_reduced_denominator / denominator_minus_reduced_denominator_times_multiplier;
            if (0 < reduced_numerator && reduced_numerator < reduced_denominator && reduced_denominator % reduced_numerator == 0) {
                return 1; // True
            }
        }
    }
    return 0; // False
}

typedef struct {
    int start;
    int end;
    int count;
} ThreadArg;

void* thread_func(void* arg) {
    ThreadArg* thread_arg = (ThreadArg*) arg;
    thread_arg->count = 0;
    for (int denominator = thread_arg->start; denominator < thread_arg->end; ++denominator) {
        thread_arg->count += is_funky_fraction(denominator);
    }
    return NULL;
}

int count_funky_fractions() {
    pthread_t threads[NUM_THREADS];
    ThreadArg thread_args[NUM_THREADS];
    int chunk_size = LIMIT / NUM_THREADS;

    // Create threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_args[i].start = i * chunk_size + 1;
        thread_args[i].end = (i == NUM_THREADS - 1) ? LIMIT : thread_args[i].start + chunk_size;
        pthread_create(&threads[i], NULL, thread_func, &thread_args[i]);
    }

    // Join threads and sum up results
    int total_count = 0;
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
        total_count += thread_args[i].count;
    }

    return total_count;
}


int main() {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int funky_fraction_count = count_funky_fractions();

    clock_gettime(CLOCK_MONOTONIC, &end);
    double total_time = end.tv_sec - start.tv_sec;
    total_time += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    char limit_str[50];
    char funky_str[50];

    format_with_commas(funky_str, funky_fraction_count);
    format_with_commas(limit_str, LIMIT);
    printf("%s Funky Fractions up to %s\n", funky_str, limit_str);
    printf("Total time taken: %.2f seconds\n", total_time);

    return 0;
}