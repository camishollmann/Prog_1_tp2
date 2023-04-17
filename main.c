#include <stdio.h>
#include "libAgenda.h"

int main (){
int ano;
    char saida = 'c';
    
    printf("Entre com o ano: ");
    scanf("%d", &ano);

    struct compromisso compr;
    struct agenda plan = criaAgenda(ano);
        
    while (saida!='s'){   

        leCompromisso(&plan, &compr);

        printf("Digite um char qualquer ou s para sair\n");
        scanf(" %c", &saida);

    }
    
    listaCompromissos(&plan);
           
    return 0;
}