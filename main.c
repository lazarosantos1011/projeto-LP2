#include "func.h"
#include <locale.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

/*
Ler_arquivo(){  
}
salvar_arquivo(){}
*/
int main()
{
	setlocale(LC_ALL, "Portuguese");
	
	ler();
	escolhaMenu1();

	return 0;
}
