import time
import multiprocessing


LIMIT = 10_000_000


def digits(n):
    if n < 10:
        return 1
    elif n < 100:
        return 2
    elif n < 1_000:
        return 3
    elif n < 10_000:
        return 4
    elif n < 100_000:
        return 5
    elif n < 1_000_000:
        return 6
    elif n < 10_000_000:
        return 7
    elif n < 100_000_000:
        return 8
    elif n < 1_000_000_000:
        return 9
    else:
        return 10


# Precompute powers of 10
powers_of_10 = [10 ** i for i in range(digits(LIMIT) + 1)]


def is_funky_fraction(denominator):
    denominator_digits = digits(denominator)

    # iterate every possible cut location
    for cut_index in range(1, denominator_digits):
        divisor = powers_of_10[cut_index]
        reduced_numerator_multiplier = powers_of_10[denominator_digits - cut_index]

        # beginning of the denominator that overlaps with the end of the numerator
        overlap = denominator // divisor

        reduced_denominator = denominator % divisor

        # if reduced denominator starts with a zero, skip it
        # if not using strings, we could make sure that the number of digits is expected
        if reduced_denominator == 0 or digits(reduced_denominator) < cut_index:
            continue

        # see if there's an integer solution
        overlap_times_reduced_denominator = overlap * reduced_denominator
        denominator_minus_reduced_denominator_times_multipler = (denominator - (reduced_denominator * reduced_numerator_multiplier))
        is_integer_solution = overlap_times_reduced_denominator % denominator_minus_reduced_denominator_times_multipler == 0

        if is_integer_solution:
            reduced_numerator = overlap_times_reduced_denominator // denominator_minus_reduced_denominator_times_multipler
            # Ensure that the reduced fraction is a positive unit fraction less than 1
            if 0 < reduced_numerator < reduced_denominator and reduced_denominator % reduced_numerator == 0:
                # `numerator` is only used for the print statement
                # numerator = int((reduced_numerator * 10 ** cut_index) + overlap)
                # print(f"{numerator}/{denominator}\t{reduced_numerator_int}/{reduced_denominator}")
                # print(f"{denominator}")
                return True

    return False


def count_funky_fractions():
    with multiprocessing.Pool() as pool:
        results = pool.map(is_funky_fraction, range(11, LIMIT))
    return sum(results)


# Example usage
if __name__ == "__main__":
    start_time = time.time()
    funky_fraction_count = count_funky_fractions()
    end_time = time.time()
    total_time = end_time - start_time
    print(f"{funky_fraction_count} Funky Fractions")
    print(f"Total time taken: {total_time:.2f} seconds")

# Tests
# assert is_funky_fraction(98) == True
# assert is_funky_fraction(64) == True
# assert is_funky_fraction(95) == True
# assert is_funky_fraction(664) == True
# assert is_funky_fraction(110) == True
# assert is_funky_fraction(727040) == True
# assert is_funky_fraction(49450545) == True
# assert is_funky_fraction(22) == False
# assert is_funky_fraction(1001091) == False
