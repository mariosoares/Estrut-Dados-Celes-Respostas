#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NPAL 64
#define NTAB 997

typedef struct hash Hash;
typedef struct palavra Palavra;

struct hash {
	Palavra *v[NTAB];
};

struct palavra {
	int n;
	char pal[NPAL];
	Palavra *prox;
};

static int le_palavra(FILE * fp, char *s)
{
	int i = 0;
	int c;

	while ((c = fgetc(fp)) != EOF) {
		if (isalpha(c)) {
			break;
		}
	}
	if (EOF == c) {
		return 0;
	}
	s[i++] = c;
	while (i < NPAL - 1 && (c = fgetc(fp)) != EOF && isalpha(c)) {
		s[i++] = c;
	}
	s[i] = '\0';
	return 1;
}

static void *aloca(size_t n)
{
	void *p = malloc(n);
	if (!p) {
		perror("Erro");
		exit(EXIT_FAILURE);
	}
	return p;
}

static Hash *inicializa(void)
{
	Hash *tab = (Hash *) aloca(sizeof(Hash));
	for (int i = 0; i < NTAB; i++) {
		tab->v[i] = NULL;
	}
	return tab;
}

static int hash(char *s)
{
	int total = 0;
	for (int i = 0; s[i]; i++) {
		total += s[i];
	}
	return total % NTAB;
}

static Palavra *acessa(Hash * tab, char *s)
{
	int h = hash(s);
	Palavra *p;

	for (p = tab->v[h]; p; p = p->prox) {
		if (!strcmp(s, p->pal)) {
			return p;
		}
	}
	p = (Palavra *) aloca(sizeof(Palavra));
	p->n = 0;
	strcpy(p->pal, s);
	p->prox = tab->v[h];
	tab->v[h] = p;
	return p;
}

static void libera(Hash * tab)
{
	for (int i = 0; i < NTAB; i++) {
		Palavra *p = tab->v[i];
		while (p) {
			Palavra *t = p;
			p = p->prox;
			free(t);
		}
	}
	free(tab);
}

static int conta_elems(Hash * tab)
{
	int total = 0;
	for (int i = 0; i < NTAB; i++) {
		for (Palavra * p = tab->v[i]; p; p = p->prox) {
			total++;
		}
	}
	return total;
}

static Palavra **cria_vetor(int n, Hash * tab)
{
	int j = 0;
	Palavra **v = (Palavra **) aloca(n * sizeof(Palavra *));

	for (int i = 0; i < NTAB; i++) {
		for (Palavra * p = tab->v[i]; p; p = p->prox) {
			v[j++] = p;
		}
	}
	return v;
}

static int compara(const void *v1, const void *v2)
{
	Palavra **p1 = (Palavra **) v1;
	Palavra **p2 = (Palavra **) v2;
	int dif = (*p2)->n - (*p1)->n;

	if (!dif) {
		return strcmp((*p1)->pal, (*p2)->pal);
	}
	return dif;
}

static void imprime(Hash * tab)
{
	int n = conta_elems(tab);
	Palavra **v = cria_vetor(n, tab);
	qsort(v, n, sizeof(Palavra *), compara);

	for (int i = 0; i < n; i++) {
		printf("%s = %d\n", v[i]->pal, v[i]->n);
	}
	free(v);
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Uso: %s <arquivo>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	char s[NPAL];
	Hash *tab = inicializa();
	FILE *fp = fopen(argv[1], "rt");

	if (!fp) {
		perror("Erro");
		exit(EXIT_FAILURE);
	}

	while (le_palavra(fp, s)) {
		Palavra *p = acessa(tab, s);
		p->n++;
	}

	imprime(tab);
	libera(tab);
	fclose(fp);
	return 0;
}
