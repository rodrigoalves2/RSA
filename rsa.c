#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "funcoes.c"

void promptGerarChaves();
void promptCriptografarMensagem();
void promptDescriptografarMensagem();

void main() {
	int opcao;
	//system("color");

	srand(time(NULL));

	printf("%s\n%s\n%s\n%s\n%s",
		"Escolha uma opcao:",
		" 1 - Gerar um par de chave privada/publica", 
		" 2 - Criptografar mensagem",
		" 3 - Descriptografar mensagem",
		"> "
	);

	scanf("%d", &opcao);

	if (opcao == 1) {
		promptGerarChaves();
	} else if (opcao == 2) {
		promptCriptografarMensagem();
	} else if (opcao == 3) {
		promptDescriptografarMensagem();
	} else {
		printf("Comando invalido!\n");

		limparStdin();

		return main();
	}

	printf("%s\n%s\n%s",
		" 1 - Continuar",
		" 2 - Sair",
		"> "
	);

	scanf("%d", &opcao);
	limparStdin();

	if (opcao == 1) {
		return main();
	}

	return;
}

void promptGerarChaves() {
	unsigned long long p, q, n, phi, e;

	char bufferChavePublica[256];
	char bufferChavePrivada[256];

	// Pega o número 'p' da entrada e verifica se é primo
	printf("Digite um numero primo:\n> ");
	scanf("%llu", &p);

	while (!primo(p)) {
		printf("Este numero nao e primo, escolha outro (sugerido %ld):\n> ", sugerirPrimo(DIGITOS_PRIMO));
		scanf("%llu", &p);
	}

	// Pega o número 'q' da entrada e verifica se é primo
	printf("Digite outro primo:\n> ");
	scanf("%llu", &q);

	while (!primo(q)) {
		printf("Este numero nao e primo, escolha outro (sugerido %ld):\n> ", sugerirPrimo(DIGITOS_PRIMO));
		scanf("%llu", &q);
	}

	n = p * q;
	
	if(n < 26) {
		printf(COR_VERMELHO "N nao pode ser menor que 26" COR_RESET "\n\n");
		promptGerarChaves();
	}
	
	phi = (p - 1) * (q - 1);

	// Péga o expoente 'e' e verifica se é válido
	printf("Digite o expoente \"e\" (sugerido: %ld):\n> ", sugerirExpoente(phi));
	scanf("%llu", &e);

	while (e <= 1 || e >= phi || mdc(phi, e) != 1) {
		printf("O expoente precisa ser relativamente primo ao \"phi\", menor que ele e maior que 1 (Sugerido: %ld):\n> ", sugerirExpoente(phi));
		scanf("%llu", &e);
	}

	snprintf(bufferChavePublica, 256, "n:\n%llu\n\ne:\n%llu\n", n, e);
	snprintf(bufferChavePrivada, 256, "p:\n%llu\n\nq:\n%llu\n\ne:\n%llu\n\nphi:\n%llu\n", p, q, e, phi);

	salvarArquivo("chave_publica.txt", bufferChavePublica);
	salvarArquivo("chave_privada.txt", bufferChavePrivada);

	printf("A chave publica foi salva no diretorio de execucao!\n");
}

void promptCriptografarMensagem() {
	unsigned long long mensagemCriptografada[MAX];
	int mensagemConvertida[MAX];
	char mensagemFinal[MAX * 6];
	char mensagemOriginal[MAX];
	bool mensagemOK = false;
	char bufferChar[16];
	unsigned long long n, e;

	while (!mensagemOK) {
		printf("Digite a mensagem:\n> ");

		getchar();
		fgets(mensagemOriginal, MAX, stdin);

		mensagemOK = converterString(mensagemOriginal, mensagemConvertida);
	}

	printf("Digite a chave publica \"n\":\n> ");
	scanf("%llu", &n);

	printf("Digite o expoente \"e\":\n> ");
	scanf("%llu", &e);

	for (int i = 0; i < strlen(mensagemOriginal) - 1; i++) {
		mensagemCriptografada[i] = expModRapida(mensagemConvertida[i], e, n);

		snprintf(bufferChar, 16, "%llu%c", mensagemCriptografada[i], i == strlen(mensagemOriginal) - 2 ? '\n' : ' ');
		strcat(mensagemFinal, bufferChar);
	}

	salvarArquivo("mensagem_criptografada.txt", mensagemFinal);

	system("color");
	printf("A mensagem criptografada foi salva no arquivo " COR_AMARELO "mensagem_criptografada.txt" COR_RESET "\n");
}

void promptDescriptografarMensagem() {
	char mensagemDescriptografada[MAX];
	unsigned long long p, q, n, e, d, phi;
	unsigned long long caractereCifrado;
	int i = 0;

	printf("Digite \"p\":\n> ");
	scanf("%llu", &p);

	printf("Digite \"q\":\n> ");
	scanf("%llu", &q);

	printf("Digite \"e\":\n> ");
	scanf("%llu", &e);

	n = p * q;
	phi = (p - 1) * (q - 1);
	d = inverso(phi, e);

	printf("Digite a mensagem criptografada:\n> ");
	do {
		scanf("%llu", &caractereCifrado);

		mensagemDescriptografada[i] = decodificarCaractere(expModRapida(caractereCifrado, d, n));
	} while (i++, getchar() != '\n');

	mensagemDescriptografada[i] = '\0';

	printf("%s\n", mensagemDescriptografada);

	salvarArquivo("mensagem_descriptografada.txt", mensagemDescriptografada);

	system("color");
	printf("A mensagem descriptografada foi salva no arquivo " COR_AMARELO "mensagem_descriptografada.txt" COR_RESET "\n");
}