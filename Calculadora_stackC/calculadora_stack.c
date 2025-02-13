//LIBRERIAS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//CONSTANTES
#define TAMANO_STACK 8
#define OPERACION_BORRAR printf("\033[H\033[J")
#define PI 3.14159265358979323846
#define EPSILON 1e-9  

//DEFINO STACK
typedef struct {
    double stack[TAMANO_STACK];
    int top;
} Calculator;

void initCalculator(Calculator *calc) {
    calc->top = 0;
    memset(calc->stack, 0, sizeof(calc->stack));
}

//FUNCIONES PARA AGREGAR Y ELIMINAR NUMEROS AL STACK
void push(Calculator *calc, double value) {
    if (calc->top < TAMANO_STACK) {
        for (int i = TAMANO_STACK - 1; i > 0; i--) {
            calc->stack[i] = calc->stack[i - 1];
        }
        calc->stack[0] = value;
        if (calc->top < TAMANO_STACK) calc->top++;
    }
}

double pop(Calculator *calc) {
    if (calc->top == 0) return 0;
    double value = calc->stack[0];
    for (int i = 0; i < TAMANO_STACK - 1; i++) {
        calc->stack[i] = calc->stack[i + 1];
    }
    calc->stack[TAMANO_STACK - 1] = 0;
    calc->top--;
    return value;
}

void printStack(Calculator *calc) {
    printf("\nStack:\n");
    for (int i = 0; i < TAMANO_STACK; i++) {
        printf("%d. %lf\n", i + 1, calc->stack[i]);
    }
}

//CONTROL PARA ANUNCIAR ERRORES
void pauseError() {
    printf("\nPresione ENTER para continuar...\n");
    while (getchar() != '\n'); 
    getchar(); 
}

//QUIERO AL MENOS 2 NUMEROS EN EL STACK PARA OPERAR
void performOperation(Calculator *calc, char op) {
    if (calc->top < 2 && strchr("+-*/p", op)) {
        printf("Math Error: No hay suficientes operandos en la pila.\n");
        pauseError();
        return;
    }

    double a = pop(calc);
    double b = (calc->top > 0) ? pop(calc) : 0;
    double result = 0;
    int error = 0;  

//ALGUNOS ERRORES
    switch (op) {
        case '+': result = b + a; break;
        case '-': result = b - a; break;
        case '*': result = b * a; break;
        case '/':
            if (fabs(a) < EPSILON) {  
                printf("Math Error: División por cero.\n");
                error = 1;
            } else {
                result = b / a;
            }
            break;
        case 'p': result = pow(b, a); break;
        case 'r':
            if (a < 0) {
                printf("Math Error: Raíz de número negativo.\n");
                error = 1;
            } else {
                result = sqrt(a);
            }
            break;
        case 's': result = sin(a * PI / 180); break;
        case 'c': result = cos(a * PI / 180); break;
        case 't':
            if (fabs(fmod(a, 180) - 90) < EPSILON) { 
                printf("Math Error: Tangente indefinida en %lf grados.\n", a);
                error = 1;
            } else {
                result = tan(a * PI / 180);
            }
            break;
        default:
            printf("Operación inválida.\n");
            pauseError();
            return;
    }

    if (error) {
        push(calc, b);
        push(calc, a);
        pauseError();
    } else {
        push(calc, result);
    }
}

void saveToFile(Calculator *calc) {
    FILE *file = fopen("calculator_log.txt", "a");
    if (!file) return;
    fprintf(file, "Stack State:\n");
    for (int i = 0; i < TAMANO_STACK; i++) {
        fprintf(file, "%d. %lf\n", i + 1, calc->stack[i]);
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
        OPERACION_BORRAR;
        printStack(&calc);
        printf("\n¿Qué desea hacer?\n");
        printf("1. Ingresar número  2. Seleccionar operación  3. Eliminar último  4. Limpiar todo  5. Salir\n");
        scanf(" %c", &choice);
        
        switch (choice) {
            case '1':
                OPERACION_BORRAR;
                printStack(&calc);
                printf("Ingrese número: ");
                scanf("%lf", &value);
                push(&calc, value);
                break;
            case '2':
                OPERACION_BORRAR;
                printStack(&calc);
                printf("Seleccione operación: +, -, *, /, r (raíz cuadrada), s (seno), c (coseno), t (tangente), p (potencia)\n");
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
                printf("Opción inválida.\n");
                pauseError();
        }
    }
    return 0;
}
