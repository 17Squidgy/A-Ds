#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 100
#define MAX_DEGREE 10

// Функция для решения СЛАУ методом Гаусса
void gaussElimination(int n, double A[MAX_DEGREE+1][MAX_DEGREE+2], double result[MAX_DEGREE+1]) {
    for (int i = 0; i <= n; i++) {
        // Поиск максимального элемента в колонке
        int maxRow = i;
        for (int k = i+1; k <= n; k++) {
            if (fabs(A[k][i]) > fabs(A[maxRow][i])) {
                maxRow = k;
            }
        }

        // Перестановка строк
        for (int k = i; k <= n+1; k++) {
            double tmp = A[maxRow][k];
            A[maxRow][k] = A[i][k];
            A[i][k] = tmp;
        }

        // Прямой ход
        for (int k = i+1; k <= n; k++) {
            double factor = A[k][i] / A[i][i];
            for (int j = i; j <= n+1; j++) {
                A[k][j] -= factor * A[i][j];
            }
        }
    }

    // Обратный ход
    for (int i = n; i >= 0; i--) {
        result[i] = A[i][n+1];
        for (int j = i+1; j <= n; j++) {
            result[i] -= A[i][j] * result[j];
        }
        result[i] /= A[i][i];
    }
}

int main() {
    FILE *file;
    char filename[100];
    double x[MAX_POINTS], y[MAX_POINTS];
    int n = 0;

    // Считывание файла
    while (1) {
        printf("Введите имя файла с данными: ");
        scanf("%s", filename);
        file = fopen(filename, "r");
        if (file == NULL) {
            printf("Ошибка открытия файла. Попробуйте снова.\n");
        } else {
            break;
        }
    }

    while (fscanf(file, "%lf %lf", &x[n], &y[n]) == 2) {
        n++;
        if (n >= MAX_POINTS) break;
    }
    fclose(file);

    if (n < 2) {
        printf("Недостаточно данных.\n");
        return 1;
    }

    int degree;
    while (1) {
        printf("Введите степень полинома (0 - %d): ", n - 1);
        scanf("%d", &degree);
        if (degree >= 0 && degree < n && degree <= MAX_DEGREE) break;
        printf("Некорректная степень.\n");
    }

    // Формирование матрицы нормальных уравнений
    double A[MAX_DEGREE+1][MAX_DEGREE+2] = {0};
    for (int row = 0; row <= degree; row++) {
        for (int col = 0; col <= degree; col++) {
            for (int i = 0; i < n; i++) {
                A[row][col] += pow(x[i], row + col);
            }
        }
        for (int i = 0; i < n; i++) {
            A[row][degree + 1] += y[i] * pow(x[i], row);
        }
    }

    // Решение системы
    double coeffs[MAX_DEGREE+1];
    gaussElimination(degree, A, coeffs);

    // Вывод результата
    printf("\nКоэффициенты аппроксимирующего полинома:\n");
    for (int i = 0; i <= degree; i++) {
        printf("a%d = %.6lf\n", i, coeffs[i]);
    }

    printf("\nP(x) = ");
    for (int i = 0; i <= degree; i++) {
        printf("%+0.6lf*x^%d ", coeffs[i], i);
    }
    printf("\n");

    return 0;
}
