#include <stdio.h>

#define MAX_SIZE 10

typedef struct {
    int rows;
    int columns;
    double values[MAX_SIZE][MAX_SIZE];
} Matrix;

void read_matrix(Matrix *matrix, const char *name)
{
    printf("Enter rows and columns for matrix %s (1-%d): ", name, MAX_SIZE);
    while (scanf("%d %d", &matrix->rows, &matrix->columns) != 2 ||
           matrix->rows < 1 || matrix->rows > MAX_SIZE ||
           matrix->columns < 1 || matrix->columns > MAX_SIZE) {
        printf("Invalid dimensions. Enter two values from 1 to %d: ", MAX_SIZE);
        while (getchar() != '\n') {
        }
    }

    printf("Enter the values of matrix %s:\n", name);
    for (int row = 0; row < matrix->rows; row++) {
        for (int column = 0; column < matrix->columns; column++) {
            while (scanf("%lf", &matrix->values[row][column]) != 1) {
                printf("Enter a numeric value: ");
                while (getchar() != '\n') {
                }
            }
        }
    }
}

void print_matrix(const Matrix *matrix)
{
    for (int row = 0; row < matrix->rows; row++) {
        for (int column = 0; column < matrix->columns; column++) {
            printf("%8.2f", matrix->values[row][column]);
        }
        printf("\n");
    }
}

Matrix add_matrices(const Matrix *first, const Matrix *second)
{
    Matrix result = {first->rows, first->columns, {{0}}};

    for (int row = 0; row < result.rows; row++) {
        for (int column = 0; column < result.columns; column++) {
            result.values[row][column] = first->values[row][column] + second->values[row][column];
        }
    }
    return result;
}

Matrix multiply_matrices(const Matrix *first, const Matrix *second)
{
    Matrix result = {first->rows, second->columns, {{0}}};

    for (int row = 0; row < first->rows; row++) {
        for (int column = 0; column < second->columns; column++) {
            for (int index = 0; index < first->columns; index++) {
                result.values[row][column] += first->values[row][index] * second->values[index][column];
            }
        }
    }
    return result;
}

Matrix transpose_matrix(const Matrix *matrix)
{
    Matrix result = {matrix->columns, matrix->rows, {{0}}};

    for (int row = 0; row < matrix->rows; row++) {
        for (int column = 0; column < matrix->columns; column++) {
            result.values[column][row] = matrix->values[row][column];
        }
    }
    return result;
}

int main(void)
{
    Matrix first;
    Matrix second;
    Matrix result;
    int choice;

    printf("=== Matrix Operations ===\n");
    printf("1. Matrix addition\n");
    printf("2. Matrix multiplication\n");
    printf("3. Matrix transpose\n");
    printf("Choose an operation: ");

    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 3) {
        printf("Invalid operation.\n");
        return 1;
    }

    read_matrix(&first, "A");

    if (choice == 1) {
        read_matrix(&second, "B");
        if (first.rows != second.rows || first.columns != second.columns) {
            printf("Addition requires matrices with equal dimensions.\n");
            return 1;
        }
        result = add_matrices(&first, &second);
    } else if (choice == 2) {
        read_matrix(&second, "B");
        if (first.columns != second.rows) {
            printf("Multiplication requires columns of A to equal rows of B.\n");
            return 1;
        }
        result = multiply_matrices(&first, &second);
    } else {
        result = transpose_matrix(&first);
    }

    printf("Result:\n");
    print_matrix(&result);
    return 0;
}
