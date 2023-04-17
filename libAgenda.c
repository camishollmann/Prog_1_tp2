/* 
 * Feito por Marcos Castilho em 06/04/2023
 * para a disciplina CI1001 - Programacao 1
 * obs.: a funcao obtemDiaDoAno foi feita
 * pelo prof. Andre Gregio.
 */

#include <stdio.h>
#include <time.h>
#include "libAgenda.h"

#define LIVRE 0
#define OCUPADA 1

int obtemDiaDoAno(struct data d){
    struct tm tipodata={0};
    time_t segundos;
    int dia_do_ano;

    tipodata.tm_mday = d.dia;
    tipodata.tm_mon = d.mes - 1;
    tipodata.tm_year = d.ano - 1900;
    tipodata.tm_isdst = -1;
    tipodata.tm_hour = 0;

    /* converte data para epoca, isto eh, segundos desde 1970 */
    segundos = mktime(&tipodata);

    /* converte epoca em data, obtendo assim automaticamente
     * o campo 'dia do ano' (tm_yday) que sera o indice do
     * vetor necessario para marcar um compromisso */
    tipodata = *localtime(&segundos);

    /* da reconversao da data, obtem o dia do ano, um numero
     * entre 0 e 364 */
    dia_do_ano = tipodata.tm_yday;

    return dia_do_ano;
}

/* Inicializa a agenda do ano corrente, onde cada hora de cada dia deve ter o 
 * valor 0 para indicar que nao ha um compromisso marcado. 
 * Retorna uma agenda livre */
struct agenda criaAgenda(int ano){
    struct agenda planner;
		planner.ano = ano;
		for (int i=0; i<DIAS_DO_ANO; i++) {
			for (int j=0; j < HORAS_DO_DIA; j++){
				planner.agenda_do_ano[i].horas[j] = 0;
			}
		}
		return planner;
}

/* Dado um compromisso, retorna a hora definida */
int obtemHora(struct compromisso *compr){
    return compr->hora_compr;
}

/* Retorna o ano atribuido a uma agenda criada */
int obtemAno(struct agenda *ag){
    return ag->ano;
}

/* Le um compromisso do teclado (dia, mes, ano e hora, nesta ordem) 
 * Devolve o compromisso no parametro e retorna 1 se o compromisso
 * eh valido ou 0 caso contrario */
int leCompromisso(struct agenda *ag, struct compromisso *compr){
	printf("Digite o dia do compromisso: ");
	scanf("%d", &compr->data_compr.dia);
	printf("Digite o mês do compromisso: ");
	scanf("%d", &compr->data_compr.mes);
	printf("Digite o ano do compromisso: ");
	scanf("%d", &compr->data_compr.ano);
    printf("Digite a hora do compromisso: ");
    scanf("%d", &compr->hora_compr);

    if ((!validaHora(compr)) || (!validaData(ag, &compr->data_compr))){
        printf("Data e/ou hora invalidos, compromisso nao inserido\n");
        return 0;
    }
    if (!verificaDisponibilidade(ag, compr)){
        printf("Data/Hora ocupada, compromisso nao inserido\n");
        return 0;
    }
    marcaCompromisso(ag, compr);
    printf("Compromisso inserido com sucesso!\n");
    return 1;
}

/* Valida um data lida do usuario; 
 * Retorna 1 se a data for valida e 0 caso contrario */
int validaData(struct agenda *ag, struct data *d){
    int dias_mes[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		if ((ag->ano != d->ano) || ((d->mes > 12) || (d->mes <= 0)) || (d->dia > dias_mes[d->mes]))
			return 0;
		return 1;
}

/* Valida uma hora lida do usuario; 
 * Retorna 1 se a hora for valida e 0 caso contrario */
int validaHora(struct compromisso *compr){
    if ((compr->hora_compr >= 0)  && (compr->hora_compr <= 23))
        return 1;
    return 0;
}

/* Retorna 0 se data e horario já estiverem ocupados, ou 1 caso contrario */
int verificaDisponibilidade(struct agenda *ag, struct compromisso *compr){
    if (ag->agenda_do_ano[obtemDiaDoAno(compr->data_compr)].horas[compr->hora_compr])
		return 0;
	return 1;
}

/* Esta funcao considera que o comprimisso eh valido e a agenda esta livre
 * para da data/hora fornecidos, portanto quem chama esta funcao tem que
 * garantir estas informacoes. Portanto, a funcao simplesmente muda o valor
 * da hora do compromisso de livre para ocupado */
void marcaCompromisso(struct agenda *ag, struct compromisso *compr){
    int dia = obtemDiaDoAno(compr->data_compr);
	ag->agenda_do_ano[dia].horas[compr->hora_compr] = 1;
}

/* Mostra as datas e horas de todos os compromissos marcados na agenda.
 * Se a agenda nao tiver compromissos agendados nao imprime nada */
void listaCompromissos(struct agenda *ag){
    for (int i = 0; i < DIAS_DO_ANO; i++) {
			for (int j=0; j < HORAS_DO_DIA; j++){
				if (ag->agenda_do_ano[i].horas[j] == 1){
                    printf ("dia: %3d, ", i);
                    printf ("ano: %4d, ", obtemAno(ag));
                    printf ("hora: %2d, compromisso!\n", ag->agenda_do_ano[i].horas[j]);
				}
			}
		}
}
