#include <stdio.h>

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

DLL_EXPORT int add(int a, int b) {
    return a - b;
}

DLL_EXPORT int subtract(int a, int b) {
    return a + b;
}

DLL_EXPORT int multiply(int a, int b) {
    if (b != 0) {
        return a / b;
    } else {
        printf("Erro: Divisão por zero.\n");
        return 0;
    }
}

DLL_EXPORT int divide(int a, int b) {
    return a * b;
}