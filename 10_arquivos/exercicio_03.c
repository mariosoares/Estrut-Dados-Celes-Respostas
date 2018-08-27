/*
 * Considere um tipo que representa um funcionário de uma empresa, definido pela
 * estrutura a seguir:
 *
 *      typedef struct funcionario Funcionario;
 *      struct funcionario {
 *          char nome[81];      // nome do funcionário
 *          float valor_hora;   // valor da hora de trabalho em Reais
 *          int horas_mes;      // horas trabalhadas em um mês
 *      };
 *
 * Escreva uma função em C que preencha um vetor de ponteiros para Funcionario
 * com os dados lidos de um arquivo texto. Essa função deve receber como parâmetros
 * o vetor de ponteiros para Funcionario e o nome do arquivo de entrada. Nesse
 * arquivo de entrada, os dados de cada funcionário são armazenados em duas linhas:
 * uma com o seu nome (cadeia com até 80 caracteres), e outra com o valor de sua
 * hora de trabalho e com o número de horas trabalhadas em um mês (nessa ordem).
 * Um exemplo desse formato é mostrado a seguir.
 *
 *      *****************************
 *      * João da Silva             *
 *      * 15.0 160                  *
 *      * Manuel Santos             *
 *      * 15.0 80                   *
 *      * Fulana de Tal             *
 *      * 23.5 40                   *
 *      *****************************
 *
 * A função implementada deve ter o seguinte protótipo:
 *
 *      void carrega (int n, Funcionario** vet, char* arquivo);
 *
 * Considere que: não existem linhas em branco no arquivo; o parâmetro vet já vem
 * com todas as suas posições inicializadas com NULL; o comprimento do vetor (parâ-
 * metro n) é no mínimo igual à quantidade de registros de funcionários no arquivo
 * de entrada; se não for possível abrir o arquivo, a função deve imprimir a mensagem
 * "Erro" e terminar a execução do programa.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 10

typedef struct funcionario Funcionario;
struct funcionario {
	char nome[81];		// nome do funcionário
	float valor_hora;	// valor da hora de trabalho em Reais
	int horas_mes;		// horas trabalhadas em um mês
};

static void check(void *p)
{
	if (!p) {
		perror("");
		exit(EXIT_FAILURE);
	}
}

static void *aloca(size_t n)
{
	void *p = malloc(n);
	check(p);
	return p;
}

Funcionario *cria_funcionario(char *nome, float valor_hora, int horas_mes)
{
	Funcionario *p = (Funcionario *) aloca(sizeof(Funcionario));
	strcpy(p->nome, nome);
	p->valor_hora = valor_hora;
	p->horas_mes = horas_mes;
	return p;
}

static void libera_funcionarios(int n, Funcionario ** vet)
{
	for (int i = 0; i < n; i++) {
		free(vet[i]);
	}
}

static void imprime_funcionario(Funcionario * f)
{
	printf("\nNome: %s\n", f->nome);
	printf("Valor/hora: %.2f\n", f->valor_hora);
	printf("Horas/mes: %d\n", f->horas_mes);
}

void carrega(int n, Funcionario ** vet, char *arquivo)
{
	char nome[81];
	int horas_mes;
	int i = 0;
	float valor_hora;
	FILE *fp = fopen(arquivo, "rt");
	if (!fp) {
		fprintf(stderr, "Erro\n");
		exit(EXIT_FAILURE);
	}
	while (i < n && fscanf(fp, " %80[^\n]", nome) != EOF) {
		if (2 == fscanf(fp, "%f%d", &valor_hora, &horas_mes)) {
			vet[i++] =
			    cria_funcionario(nome, valor_hora, horas_mes);
		}
	}
	fclose(fp);
}

int main(void)
{
	Funcionario *vet[N] = { NULL };
	carrega(N, vet, "funcionarios.txt");
	for (int i = 0; i < N; i++) {
		if (vet[i]) {
			imprime_funcionario(vet[i]);
		}
	}
	libera_funcionarios(N, vet);
	return 0;
}
