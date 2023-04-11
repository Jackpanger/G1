#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
//#include <sys/mman.h>
#include <fcntl.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {

    double tstart = 0.0, ttaken;
    int N;
    int t;
    FILE *fp;

    if (argc != 3) {
        printf("N: positive number bigger than 2\n");
        printf("t: number of characters in the file \n");
        exit(1);
    }
    N = atoi(argv[1]);
    t = atoi(argv[2]);
    tstart = omp_get_wtime();
    omp_set_num_threads(t);
    char filename[15];
    sprintf(filename, "%d.txt", N);

    fp = fopen(filename,"w");
//    bool isPrime[N+1];
    bool *isPrime = malloc(sizeof(int)*(N+1));

    for (int i = 2; i <= N; i++) {
        isPrime[i] = true;
    }

    // Step 2: remove all multiples of 2
    for (int i = 4; i <= N; i += 2) {
        isPrime[i] = false;
    }

    // Step 3-6: remove all multiples of remaining primes
    //3
    for (int i = 3; i <= N &&i*i<=N; i += 2) {
        if (isPrime[i]) {
            for (int j = i*i; j <= N; j += i) {
                isPrime[j] = false;
            }
        }
    }
    ttaken = omp_get_wtime() - tstart;
    printf("Time take for the main part: %f\n", ttaken);

    for (int i = 2, j = 0; i <= N; i++) {
        if (isPrime[i]) {
            j++;
//            printf("%d %d\n", j, i);
        }
    }

    fclose(fp);
    return 0;
}




