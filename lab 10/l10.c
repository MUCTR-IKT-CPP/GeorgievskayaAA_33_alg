#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_WEIGHT 100000

// Функция для решения задачи о рюкзаке
int pack(int* weights, int n, int max_weight) {
    int dp[max_weight + 1];
    for (int i = 0; i <= max_weight; i++) {
        dp[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        for (int j = max_weight; j >= weights[i]; j--) {
            dp[j] = (dp[j] > dp[j - weights[i]] + weights[i]) ? dp[j] : dp[j - weights[i]] + weights[i];
        }
    }

    return dp[max_weight];
}

int main() {
    int n;

    // Ввод количества камней и их весов
    // printf("Введите количество камней: ");
    scanf("%d", &n);

    int weights[n];
    int total_weight = 0;
    int i = 0;
    // printf("Введите веса камней: ");
    while (scanf("%d", &weights[i] != EOF){
        total_weight += weights[i];
      i++;
    }
  /*for (int i = 0; i < n; i++) {
        scanf("%d", &weights[i]);
        total_weight += weights[i];
    }*/

    // Находим максимально возможный вес для одной из кучек
    int max_weight_for_one = total_weight / 2;
    
    // Используем рюкзак, чтобы найти наилучший вес для одной кучки
    int max_weight_in_pack = pack(weights, n, max_weight_for_one);

    // Разница в весах двух куч
    int difference = total_weight - 2 * max_weight_in_pack;

    // Выводим результат
    // printf("Минимальная разница в весах двух куч: %d\n", difference);
    printf("%d", difference);
    return 0;
}
