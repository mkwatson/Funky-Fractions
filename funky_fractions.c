#include <stdio.h>
#include <time.h>
#include <string.h>

#define LIMIT 1000000000

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

        if (reduced_denominator == 0 || digits(reduced_denominator) < cut_index) {
            continue;
        }

        int overlap_times_reduced_denominator = overlap * reduced_denominator;
        int denominator_minus_reduced_denominator_times_multiplier = denominator - reduced_denominator * reduced_numerator_multiplier;

        if (overlap_times_reduced_denominator % denominator_minus_reduced_denominator_times_multiplier == 0) {
            int reduced_numerator = overlap_times_reduced_denominator / denominator_minus_reduced_denominator_times_multiplier;
            if (0 < reduced_numerator && reduced_numerator < reduced_denominator && reduced_denominator % reduced_numerator == 0) {
                // int unit_fraction_denom = reduced_denominator / reduced_numerator;
                // int numerator = denominator / unit_fraction_denom;
                // printf("%d %d %d %d\n", numerator, denominator, unit_fraction_denom, overlap);
                return 1;
            }
        }
    }
    return 0;
}


int count_funky_fractions() {
    int count = 0;
    for (int denominator = 11; denominator < LIMIT; ++denominator) {
        count += is_funky_fraction(denominator);
    }
    return count;
}

int main() {
    clock_t start_time = clock();
    int funky_fraction_count = count_funky_fractions();
    clock_t end_time = clock();
    double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    char limit_str[50];
    char funky_str[50];
    format_with_commas(funky_str, funky_fraction_count);
    format_with_commas(limit_str, LIMIT);
    printf("%s Funky Fractions up to %s\n", funky_str, limit_str);
    printf("Total time taken: %.2f seconds\n", total_time);

    return 0;
}
