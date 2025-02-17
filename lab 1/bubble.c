#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>  // Для uint64_t

void generation(double arr[], int n) {
	int i = 0;
    for(i = 0; i < n; i++) {
        arr[i] = (2.0 * rand() / RAND_MAX) - 1.0;
    }
}

void bubble(double arr[], int n, unsigned long long* pass_count, unsigned long long* swap_count) {
    *pass_count = 0;
    *swap_count = 0;
    double temp;
    
    int i, j = 0;
    for(i = 0; i < n - 1; i++) {
        (*pass_count)++;
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                (*swap_count)++;
            }
        }
    }
}

void measure(int m, int s) {
    FILE *f = fopen("times.txt", "a");
    if (!f) {
        printf("Error opening file\n");
        return;
    }

    double *arr = (double *)malloc(m * sizeof(double));
    if (arr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    double ttime = 0.0;
    clock_t start_time, end_time;

    // Сначала выводим размер массива в файл
    fprintf(f, "\n--- MASSIVE %d ---\n", m);
    printf("\n%d\n", m);
    fprintf(f, "TRY TIME PASS SWAP\n");
	
	int i = 0;
    for (i = 0; i < s; i++) {
        unsigned long long pass_count = 0, swap_count = 0;  // Обнуляем счетчики на каждой итерации
        generation(arr, m);  // Генерация случайных данных
        start_time = clock();

        bubble(arr, m, &pass_count, &swap_count);  // Сортировка с отслеживанием количества проходов и обменов

        end_time = clock();
        
        ttime = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        fprintf(f, "%2d %.10f %llu %llu\n", i + 1, ttime, pass_count, swap_count);  // Выводим результаты в файл
        printf("%d ", i + 1);  // Выводим номер текущего прогона на экран
    }

    fclose(f);
    free(arr);  // Освобождаем память
}


int main() {
    srand(time(NULL)); // Инициализация генератора случайных чисел

    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]), series = 20;

    FILE *f = fopen("times.txt", "a");
    if (!f) {
        printf("Error opening file\n");
        return 1;
    }
	
	int i = 0;
    for(i = 0; i < num_sizes; i++) {
        measure(sizes[i], series);
    }

    fclose(f);
    return 0;
}

