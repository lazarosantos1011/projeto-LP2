#include <stdio.h>
#include <windows.h>

typedef int (*AddFunction)(int, int);

int main() {
    HINSTANCE hDLL = LoadLibrary("mydll.dll");
    if (hDLL == NULL) {
        printf("Erro ao carregar a DLL.\n");
        return 1;
    }

    AddFunction addFunc = (AddFunction)GetProcAddress(hDLL, "add");
    if (addFunc == NULL) {
        printf("Erro ao obter o endereço da função.\n");
        return 1;
    }

    int result = addFunc(5, 3);
    printf("Resultado: %d\n", result);

    FreeLibrary(hDLL);
    return 0;
}
