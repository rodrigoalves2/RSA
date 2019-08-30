#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#define MAX 1024
#define DIGITOS_PRIMO 4

#define COR_RESET "\033[1;0m"
#define COR_AMARELO "\x1b[33m"
#define COR_VERMELHO "\033[1;31m"

bool primo(long num) {
	int max = sqrt(num);

	if (~num & 1)
		return num == 2;

	for (int i = 3; i <= max; i += 2) {
		if (num % i == 0)
			return false;
	}

	return true;
}

long mdc(long a, long b) {
	if (!b)
		return a;

	return mdc(b, a % b);
}

bool converterString(char original[], int alvo[]) {
	int c;

	for (int i = 0; i < strlen(original) - 1; i++) {
		c = tolower(original[i]);

		if (c >= 'a' && c <= 'z') {
			alvo[i] = c - 97;
		} else if (c == ' ') {
			alvo[i] = 26;
		} else {
			system("color");
			fprintf(stderr, COR_VERMELHO "A mensagem contem caracteres invalidos!\n" COR_RESET);
			return false;
		}
	}

	return true;
}

unsigned long long expModRapida(unsigned long long m, unsigned long long e, unsigned long long n) {
	// m ^ e mod n
	long long resultado = 1;
	long long pot = m % n;

	for (; e > 0; e >>= 1) {
		if (e % 2 == 1) {
			resultado = (resultado * pot) % n;
		}

		pot = (pot * pot) % n;
	}

	return resultado;
}

long long inverso(long long a, long long b) {
	long long quociente[MAX], resposta[MAX];
	long long inverso, resto, aOriginal = a;
	int i, j, k;

	for (i = 0; b != 0; i++) {
		quociente[i] = a / b;
		resto = a % b;
		a = b;
		b = resto;
	}

	resposta[0] = 1;
	resposta[1] = quociente[i - 2];

	for (k = 1, j = i - 3; j >= 0; j--, k++) {
		resposta[k + 1] = quociente[j] * resposta[k] + resposta[k - 1];
	}

	inverso = resposta[k];

	if (k & 1) {
		inverso *= -1;
	}

	while (inverso < 0) {
		inverso += aOriginal;
	}

	return inverso;
}

char decodificarCaractere(int caractere) {
	if (caractere == 26) // Espaço
		return 32;

	return caractere + 97;
}

void salvarArquivo(char nome[], char conteudo[]) {
	FILE *arquivo;
	arquivo = fopen(nome, "w");
	fprintf(arquivo, "%s", conteudo);
	fclose(arquivo);
}

long proximoPrimo(long num) {
	if (~num & 1)
		num++;

	while (num += 2) {
		if (primo(num)) {
			return num;
		}
	}
}

long sugerirExpoente(long phi) {
	long sugerido;

	sugerido = rand() % phi;

	while (mdc(phi, sugerido) != 1) {
		sugerido = rand() % phi;
	}

	return sugerido;
}

long sugerirPrimo(int digitos) {
	long sugerido;

	sugerido = rand() % (int)pow(10, digitos);

	return proximoPrimo(sugerido);
}

void limparStdin() {
	while (getchar() != '\n');
}