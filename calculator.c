#include <stdio.h>

int main(void)
{
    double first;
    double second;
    double result;
    char operation;

    printf("=== Basic Calculator ===\n");
    printf("Enter an expression such as 12.5 + 3: ");

    if (scanf("%lf %c %lf", &first, &operation, &second) != 3) {
        printf("Invalid input. Use the format: number operator number\n");
        return 1;
    }

    switch (operation) {
        case '+':
            result = first + second;
            printf("Result: %.2f\n", result);
            break;
        case '-':
            result = first - second;
            printf("Result: %.2f\n", result);
            break;
        case '*':
            result = first * second;
            printf("Result: %.2f\n", result);
            break;
        case '/':
            if (second == 0.0) {
                printf("Error: division by zero is not allowed.\n");
                return 1;
            }
            result = first / second;
            printf("Result: %.2f\n", result);
            break;
        case '%':
            if ((long long)second == 0) {
                printf("Error: remainder by zero is not allowed.\n");
                return 1;
            }
            printf("Result: %lld\n", (long long)first % (long long)second);
            break;
        default:
            printf("Invalid operator. Use +, -, *, /, or %% .\n");
            return 1;
    }

    return 0;
}
