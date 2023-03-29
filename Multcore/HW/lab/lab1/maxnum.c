#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/mman.h>
#include <fcntl.h>

void sequential(const char buff[], int num);

void parallel(const char buff[], int N, int num);

int maxvalue(const int local_histogram[]);

void display(int x, int y, int z);


int main(int argc, char *argv[]) {


    int N;
    int num;
    FILE *fp;
//    int num_kind_character_flag = 0; // check if num_kind_character is changed

    if (argc != 4) {
        printf("N: number of threads\n");
        printf("num: number of characters in the file \n");
        printf("Filename: name of the file\n");
        exit(1);
    }
    N = atoi(argv[1]);
    num = atoi(argv[2]);
    char *buff = malloc(num * sizeof(char));
    fp = fopen(argv[3], "r");
    if (fp == NULL) {
        printf("Cannot open the file %s\n", argv[3]);
        exit(1);
    }
    fread(buff, num + 1, 1, fp);
    switch (N) {
        case 0: {
            sequential(buff, num);
            break;
        }
        case 1:
        case 4: {
            parallel(buff, N, num);
            break;
        }

        default:
            exit(1);
    }

    fclose(fp);
    free(buff);
    return 0;
}

void sequential(const char buff[], int num) {
    int histogram[4] = {0};
    for (int i = 0; i < num; ++i) {
        char c = buff[i];
        histogram[c - 97]++;
    }
    int maxIndex = maxvalue(histogram);
    display(maxIndex + 97, histogram[maxIndex], num);
}

void parallel(const char buff[], int N, int num) {
       //method 1
//    int histogram[4] = {0};
//#pragma omp parallel num_threads(N)
//    {
//        int local_histogram[4] = {0};
//#pragma omp for
//        for (int i = 0; i < num; ++i) {
//            local_histogram[buff[i] - 97] += 1;
//        }
//#pragma omp critical
//        for (int j = 0; j < 4; j++) {
//            histogram[j] += local_histogram[j];
//        }
//    }

//     method 2
    int histogram[4] = {0};
    omp_set_num_threads(N);
#pragma omp parallel
    {
        int local_histogram[4][4] = {0};
        int tid = omp_get_thread_num();

#pragma omp for
        for (int i = 0; i < num; i++) {
            local_histogram[tid][buff[i] - 97] ++;
        }
#pragma omp critical
        for (int j = 0; j < 4; j++) {
            for (int t = 0; t < 4; t++) {
                histogram[j] += local_histogram[t][j];
            }
        }
    }
    int maxIndex = maxvalue(histogram);
    display(maxIndex + 97, histogram[maxIndex], num);
}


int maxvalue(const int local_histogram[]) {
    int maxIndex = 0;
    for (int i = 0; i < 4; ++i) {
        if (local_histogram[i] > local_histogram[maxIndex]) maxIndex = i;
    }
    return maxIndex;
}

void display(int x, int y, int z) {
    printf("%c occurred the most %d times of a total of %d character.\n", x, y, z);
}

