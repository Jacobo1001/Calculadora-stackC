#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define STACK_SIZE 8
#define CLEAR_SCREEN printf("\033[H\033[J")
#define PI 3.14159265358979323846

typedef struct {
    double stack[STACK_SIZE];
    int top;
} Calculator;

void initCalculator(Calculator *calc) {
    calc->top = 0;
    memset(calc->stack, 0, sizeof(calc->stack));
}

void push(Calculator *calc, double value) {
    if (calc->top < STACK_SIZE) {
        for (int i = STACK_SIZE - 1; i > 0; i--) {
            calc->stack[i] = calc->stack[i - 1];
        }
        calc->stack[0] = value;
        if (calc->top < STACK_SIZE) calc->top++;
    }
}

double pop(Calculator *calc) {
    if (calc->top == 0) return 0;
    double value = calc->stack[0];
    for (int i = 0; i < STACK_SIZE - 1; i++) {
        calc->stack[i] = calc->stack[i + 1];
    }
    calc->stack[STACK_SIZE - 1] = 0;
    calc->top--;
    return value;
}

void printStack(Calculator *calc) {
    printf("\nStack:\n");
    for (int i = 0; i < STACK_SIZE; i++) {
        printf("%d. %lf\n", i + 1, calc->stack[i]);
    }
}

void performOperation(Calculator *calc, char op) {
    if (calc->top < 2 && strchr("+-*/p", op)) {
        printf("Math Error\n");
        return;
    }
    double a = pop(calc);
    double b = (calc->top > 0) ? pop(calc) : 0;
    double result = 0;
    
    switch (op) {
        case '+': result = a + b; break;
        case '-': result = b - a; break;
        case '*': result = a * b; break;
        case '/':
            if (a == 0) {
                printf("Math Error: Division by zero\n");
                return;
            }
            result = b / a;
            break;
        case 'p': result = pow(b, a); break;
        case 'r':
            if (a < 0) {
                printf("Math Error: Negative square root\n");
                return;
            }
            result = sqrt(a);
            break;
        case 's': result = sin(a * PI / 180); break;
        case 'c': result = cos(a * PI / 180); break;
        case 't':
            if (fmod(fabs(a), 180) == 90) {
                printf("Math Error: tan(90) undefined\n");
                return;
            }
            result = tan(a * PI / 180);
            break;
        default:
            printf("Invalid operation\n");
            return;
    }
    push(calc, result);
}

void saveToFile(Calculator *calc) {
    FILE *file = fopen("calculator_log.txt", "a");
    if (!file) return;
    fprintf(file, "Stack State:\n");
    for (int i = 0; i < STACK_SIZE; i++) {
        fprintf(file, "%d. %lf\n", STACK_SIZE - i, calc->stack[i]);
    }
    fprintf(file, "\n");
    fclose(file);
}

int main() {
    Calculator calc;
    initCalculator(&calc);
    char choice;
    double value;
    
    while (1) {
        CLEAR_SCREEN;
        printStack(&calc);
        printf("\nQue desea hacer?\n");
        printf("1. Ingresar numero 2. Seleccionar operacion 3. Eliminar ultimo 4. Limpiar todo 5. Salir\n");
        scanf(" %c", &choice);
        
        switch (choice) {
            case '1':
                CLEAR_SCREEN;
                printStack(&calc);
                printf("Ingrese numero: ");
                scanf("%lf", &value);
                push(&calc, value);
                break;
            case '2':
                CLEAR_SCREEN;
                printStack(&calc);
                printf("Seleccione operacion: +, -, *, /, r (raiz cuadrada), s (seno), c (coseno), t (tangente), p (potencia)\n");
                scanf(" %c", &choice);
                performOperation(&calc, choice);
                saveToFile(&calc);
                break;
            case '3':
                pop(&calc);
                break;
            case '4':
                initCalculator(&calc);
                break;
            case '5':
                return 0;
            default:
                printf("Opcion invalida\n");
        }
    }
    return 0;
}
