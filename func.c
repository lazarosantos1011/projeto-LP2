#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <stdlib.h>
#include "func.h"

struct client
{
    /* data */
    char cpf[12];
    char name[50];
    char password[7];
    char date_account[11];
    unsigned int n_account;
    float amount;
    char status;

}clients[50], *p_clients;

void escolhaMenu1()
{
    setlocale(LC_ALL, "Portuguese");
    int op;

    do
    {
        puts("Para acessar uma conta, digite 1.\nPara cadastrar uma conta, digite 2.\nSelecione uma opção: ");
        scanf("%i", &op);
        if (op == 1)
        {
        menu();
        }
        else if (op == 2)
        {
        cadastrar();
        }
        else
        {
        puts("\nOpção inválida, tente novamente.\n");
        }
    } while (0 < op < 3);
    
}

void cadastrar()
{
    FILE *arq;
    struct client cliente[1],*p_client;
    puts("informe CPF, NOME, SENHA, DATA, NUMERO");
    gets(cliente[0].cpf);
    gets(cliente[0].name);
    gets(cliente[0].password);
    gets(cliente[0].date_account);
    scanf("%d",&cliente[0].n_account);
    printf(" %s %s %s %s %d \n", cliente[0].cpf, cliente[0].name, cliente[0].password, cliente[0].date_account, cliente[0].n_account);
    arq= fopen("banco.txt","a+");
    fwrite(&cliente[0], sizeof(struct client), 1, arq);
    fclose(arq);


}
void ler()
{
    FILE *arq;
    int i = 0;
    arq = fopen("banco.txt","a+");

    while(fread(&clients[i],sizeof(struct client),1,arq))
    {
        printf(" %s %s %s %s %d \n\n", clients[i].cpf, clients[i].name, clients[i].password, clients[i].date_account, clients[i].n_account);
        i++;
    }
    printf(" registros %d \n\n", i);
    fclose(arq);
}


enum option{saldo, depositar, transferir, pagar, sacar};

char menu()
{
    setlocale(LC_ALL, "portuguese");

    printf("Sistema Bancario da UPE\n");
    printf("\t Escolha uma das opções:\n");
    printf("%d saldo\n",saldo);
    printf("%d depositar\n",depositar);
    printf("%d transferir\n",transferir);
    printf("%d pagar\n",pagar);
    printf("%d sacar\n",sacar);
    return(getche());
}

//salvar_arquivo(struct_client *p_client){};
