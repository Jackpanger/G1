#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
//#include <fcntl.h>
#include <stdbool.h>

void sequential(bool* isPrime, int N);
void parallel(bool* isPrime, int N);

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

    char filename[15];
    sprintf(filename, "%d.txt", N);


    bool *isPrime = malloc(sizeof(bool)*(N+1));
    if (N>1) isPrime[2] = true;
    if (t==0)
        sequential(isPrime, N);
    else{
        omp_set_num_threads(t);
        parallel(isPrime, N);
    }

    ttaken = omp_get_wtime() - tstart;
    printf("Time take for the main part: %f\n", ttaken);

    fp = fopen(filename,"w");
    for (int i = 2, j = 0; i <= N; i++) {
        if (isPrime[i]) {
            j++;
            fprintf(fp,"%d %d\n", j, i);
        }
    }
    free(isPrime);

    fclose(fp);
    return 0;
}

void sequential(bool* isPrime, int N){
    for (int i = 3; i <= N; i+=2) {
        isPrime[i] = true;
    }

    for (int i = 4; i <= N; i += 2) {
        isPrime[i] = false;
    }

    for (int i = 3; i <= (N+1)/2; i += 2) {
        if (isPrime[i]&& i<=N/i) {
            for (int j = i*i; j <= N; j += i) {
                isPrime[j] = false;
            }
        }
    }


}

void parallel(bool* isPrime, int N) {
#pragma omp parallel default(none) shared(isPrime, N)
    {
#pragma omp for nowait
        for (int i = 3; i <= N; i+=2) {
            isPrime[i] = true;
        }
#pragma omp for
        for (int i = 4; i <= N; i += 2) {
            isPrime[i] = false;
        }
#pragma omp for
        for (int i = 3; i <= (N + 1) / 2 ; i +=2) {
            if (isPrime[i]&& i<=N/i) {
                for (int j = i * i; j <= N; j += i)
                    isPrime[j] = false;
            }
        }
    }
}


