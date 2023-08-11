#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef int (*MathFunction)(int, int);

int main() {
    char opcao;
    do{
    HINSTANCE hDLL = LoadLibrary("complexo.dll");
    if (hDLL == NULL) {
        printf("Erro ao carregar a DLL.\n");
        return 1;
    }

    MathFunction addFunc = (MathFunction)GetProcAddress(hDLL, "add");
    MathFunction subtractFunc = (MathFunction)GetProcAddress(hDLL, "subtract");
    MathFunction multiplyFunc = (MathFunction)GetProcAddress(hDLL, "multiply");
    MathFunction divideFunc = (MathFunction)GetProcAddress(hDLL, "divide");

    if (addFunc == NULL || subtractFunc == NULL || multiplyFunc == NULL || divideFunc == NULL) {
        printf("Erro ao obter endereços das funções.\n");
        return 1;
    }

    int a = 10, b = 5;
    printf("Resultado da adição: %d\n", addFunc(a, b));
    printf("Resultado da subtração: %d\n", subtractFunc(a, b));
    printf("Resultado da multiplicação: %d\n", multiplyFunc(a, b));
    printf("Resultado da divisão: %d\n", divideFunc(a, b)); 

    FreeLibrary(hDLL);

    fflush(stdin);
    opcao=getchar();
    putchar(opcao);
    }while(opcao!='s');
    return 0;
}