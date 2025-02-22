#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include <stdint.h>  // Äëÿ uint64_t

void generation(double arr[], int n) {
	int i = 0;
    for(i = 0; i < n; i++) {
        arr[i] = (2.0 * rand() / RAND_MAX) - 1.0;
    }
}

void shift(double* a, double* b){
	double temp = *a;
	*a = *b;
	*b = temp;
}

void heapify(double arr[], int m, int i,
				unsigned long long int* shift_count,
				unsigned long long int* recursive) {
    int largest = i, left = 2 * i + 1, right = 2 * i + 2;
    (*shift_count)++;
    
    if (left < m && arr[left] > arr[largest])
        largest = left;
    if (right < m && arr[right] > arr[largest])
    	largest = right;
    if( largest != i){
    	shift(&arr[i], &arr[largest]);
    	(*recursive)++;
    	heapify(arr, m, largest, shift_count, recursive);   	
	}
}

void heap(double arr[], int m, unsigned long long int* shift_count, unsigned long long int* recursive){
	*recursive = 0;
	*shift_count = 0;
	int i, j = 0;
	
	for (i = m / 2 - 1; i >= 0; i--){
		heapify(arr, m, i, shift_count, recursive);
		(*shift_count)++;
	}
	
	for(j = m - 1; i >= 1; i--){
		shift(&arr[0], &arr[i]);
		
		heapify(arr, i, 0, shift_count, recursive);
		(*shift_count)++;
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

    // Ñíà÷àëà âûâîäèì ðàçìåð ìàññèâà â ôàéë
    fprintf(f, "ELEMENTS TRY TIME TOTAL RCURSIVE\n");
	printf("\n%d\n", m);
    
	int i = 0;
    for (i = 0; i < s; i++) {
        unsigned long long int shift_count = 0, recursive = 0;  // Îáíóëÿåì ñ÷åò÷èêè íà êàæäîé èòåðàöèè
        generation(arr, m);  // Ãåíåðàöèÿ ìàññèâà
        start_time = clock();

        heap(arr, m, &shift_count, &recursive);  // Ñîðòèðîâêà ñ îòñëåæèâàíèåì êîëè÷åñòâà ïðîõîäîâ è îáìåíîâ

        end_time = clock();
        
        ttime = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        fprintf(f, "%d %2d %.8f %llu %llu\n", m, i + 1, ttime, shift_count, recursive);  // Âûâîäèì ðåçóëüòàòû â ôàéë
        printf("%d ", i + 1);  // Âûâîäèì íîìåð òåêóùåãî ïðîãîíà íà ýêðàí
    }

    fclose(f);
    free(arr);  // Îñâîáîæäàåì ïàìÿòü
}


int main() {
    srand(time(NULL)); // Èíèöèàëèçàöèÿ ãåíåðàòîðà ñëó÷àéíûõ ÷èñåë

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
