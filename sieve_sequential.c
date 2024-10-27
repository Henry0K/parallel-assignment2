#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void sieve_of_eratosthenes(int n) {
    // Initialize an array to mark prime numbers
    int *prime = (int *)malloc((n + 1) * sizeof(int));
    for (int i = 2; i <= n; i++) {
        prime[i] = 1;  // Mark all numbers as prime initially
    }

    int sqrt_n = (int)sqrt(n);

    // Sieve of Eratosthenes
    for (int i = 2; i <= sqrt_n; i++) {
        if (prime[i] == 1) {  // If i is a prime
            for (int j = i * i; j <= n; j += i) {
                prime[j] = 0;  // Mark multiples of i as non-prime
            }
        }
    }

    // Print all prime numbers
    printf("Prime numbers up to %d are:\n", n);
    for (int i = 2; i <= n; i++) {
        if (prime[i] == 1) {
            printf("%d ", i);
        }
    }
    printf("\n");

    free(prime);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <upper_bound>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    // Start timing the execution
    clock_t start_time = clock();

    // Run the sieve algorithm
    sieve_of_eratosthenes(n);

    // Stop timing the execution
    clock_t end_time = clock();
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Execution time: %f seconds\n", time_taken);

    return 0;
}
