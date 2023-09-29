/*

~~~~~~~~~~~~~~~~~~~~~~ EDITOR IMAGEM PPM ~~~~~~~~~~~~~~~~~~~~~~~~~
                       Por:  Laura Boemo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MIN_DEPTH 0
#define MAX_DEPTH 255

/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                       	   Typedefining
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/

// Estrutura dos pixels
typedef struct {
	int R, G, B;
} pixel;

// Estrutura da imagem PPM
typedef struct {
	int row, col, colorDepth;
	pixel *body;
} ppmFile;

/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
						Prototypes (Funções)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/

// Leitura da Imagem PPM
ppmFile* readImage(char *imageFile);
// Retorno (Escrita) da Imagem PPM
void writeImage(char *imageName, ppmFile *imageFile);
// Filtro de Cor
void extremeContrast(ppmFile* imageFile);
// Filtro de Cor
void negateRed(ppmFile *imageFile);
// Filtro de Modificação de Pixels
void horizontalFlip(ppmFile *imageFile);
// Filtro de Modificação de Pixels
void verticalFlip(ppmFile *imageFile);
// Funcao de aplicacao de escala
void zoom(ppmFile *imageFile);

/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
						Inicialização
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
int main(void) {
	// Inicializando valores de entrada, saida e escolhas
	char inputFile[20], outputFile[20], choice[15];

	puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	puts("		Editor de Imagem Portable Pixmap (PPM)		        ");
	puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

	// Insercao dos nomes dos arquivos PPM 
	printf("Insira o nome do arquivo de entrada da imagem: ");
	scanf("%s", inputFile);

	printf("Insira o nome do arquivo de saida da imagem: ");
	scanf("%s", outputFile);

	// Atribuicao da leitura do arquivo de entrada ao typedef para arquivos PPM
	ppmFile *readFile = readImage(inputFile);

	puts("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	puts("			Opcoes de Edicao		        ");
	puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

	// Declaracao das opcoes de edicao de imagem PPM
	char options[5][100] = {
		"cor para contraste extremo",
		"cor para negativar cores vermelhas", 
		"modificacao de pixels para virar horizontalmente a imagem",
		"modificacao de pixels para virar verticalmente a imagem",
		"zoom na imagem"
	};

	// Leitura das escolhas feitas perante as opcoes fornecidas
	for (int i = 0; i < 5; ++i) {
		do {
			printf("Desejas aplicar o filtro de %s? (s/n): ", options[i]);
			fflush(stdin);
			scanf(" %c", &choice[i]);
		} while ((choice[i] != 's') && (choice[i] != 'S') && (choice[i] != 'n') && (choice[i] != 'N'));
	}

	// Chamada e execucao das opcoes selecionadas
	if (choice[0] == 's' || choice[0] == 'S')
		extremeContrast(readFile); 

	if (choice[1] == 's' || choice[1] == 'S')
		negateRed(readFile);

	if (choice[2] == 's' || choice[2] == 'S')
		horizontalFlip(readFile);
	
	if (choice[3] == 's' || choice[3] == 'S')
		verticalFlip(readFile);

	if (choice[4] == 's' || choice[4] == 'S')
		zoom(readFile);
	
	puts("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	puts("	Sua nova imagem PPM com filtros esta sendo processada...		        ");
	puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

	// Escrevendo o retorno da imagem PPM (editada) 
	writeImage(outputFile, readFile);

	// Liberacao da memoria dinamica alocada
	free(readFile->body);
	free(readFile);

	printf("%s esta disponivel!\n\n", outputFile);

	return EXIT_SUCCESS;
}

/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			 Esta funcao le o arquivo PPM de entrada
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
ppmFile* readImage(char *imageFile) {
	char imageFormat[4];
	FILE *fp; // Ponteiro File para o arquivo de entrada 

	// Alocando memoria dinamica para a imagem que sera lida
	ppmFile *imagePtr = (ppmFile*)malloc(sizeof(ppmFile));
	if (!imagePtr) {
		printf("Erro: A alocacao de memoria falhou!\n");
		exit(EXIT_FAILURE);
	}

	// Abertura do arquivo PPM em modo nao-binario
	fp = fopen(imageFile, "r");

	if (!fp) { // Caso o arquivo tenha falhado ao ter sido aberto
		printf("Erro: A abertura do arquivo falou!\n");
		exit(EXIT_FAILURE);
	}

	// Lendo o arquivo PPM e verificando seu formato
	fgets(imageFormat, sizeof(imageFormat), fp);
	if (imageFormat[0] != 'P' || imageFormat[1] != '3') {
		printf("Erro: O formato da imagem deve ser P3!\n");
		exit(EXIT_FAILURE);
	}

	// Lendo o tamanho da imagem e sua cor maxima
	fscanf(fp, "   %d %d   ", &imagePtr->col, &imagePtr->row);
	fscanf(fp, "  %d  ", &imagePtr->colorDepth);

	// Alocando memoria dinamica para os dados dos pixels
	imagePtr->body = (pixel*)malloc(imagePtr->row * imagePtr->col * sizeof(pixel));
	if (!imagePtr) {
		printf("Erro: A alocacao de memoria falhou!\n");
		exit(EXIT_FAILURE);
	}

	// Lendo os pixels que compoem o corpo da imagem
	for (int i = 0; i < imagePtr->row * imagePtr->col; ++i)
		fscanf(fp, "    %d %d %d    ", &(imagePtr->body[i].R), &(imagePtr->body[i].G), &(imagePtr->body[i].B));

	fclose(fp); // Fechando o arquivo PPM
	return imagePtr; // Retornando o arquivo lido
}

/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			Esta funcao escreve o arquivo PPM de saida
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
void writeImage(char *imageName, ppmFile *imageFile) {
	FILE *fp; // Ponteiro File para o arquivo de saida

	// Criando o ponteiro de saida
	fp = fopen(imageName, "w");
	if (!fp) { // Caso a criacao do arquivo falhe
		printf("Erro: Nao foi possivel criar o arquivo!\n");
		exit(EXIT_FAILURE);
	}

	// Escrevendo o cabecalho da imagem
	fprintf(fp, "P3"); // Numero magico
	fprintf(fp, "   %d %d   ", imageFile->col, imageFile->row);  // Colunas e Linhas
	fprintf(fp, "  %d  ", imageFile->colorDepth); // Profundidade maxima de cor

	// Escrevendo o corpo da imagem
	for (int i = 0; i < (imageFile->row * imageFile->col); ++i)
		fprintf(fp, "    %d %d %d    ", imageFile->body[i].R, imageFile->body[i].G, imageFile->body[i].B);

	fclose(fp); // Fechando a imagem criada
}

/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			Esta funcao altera cada numero de cor ou
			para o maximo possivel ou para zero, ao
			compara-lo com a metade da profundidade de cor.
			Gerando, assim, contraste extremo.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
void extremeContrast(ppmFile* imageFile) {
	// Se o numero da cor for maior que a metade da profundade da cor, sera alterado para 255, senao 0
	for (int i = 0; i < imageFile->col * imageFile->row; i++) {
		if (imageFile->body[i].R > 128) //  128 = metade da profundidade da cor
			imageFile->body[i].R = MAX_DEPTH;
		else
			imageFile->body[i].R = MIN_DEPTH;

		if (imageFile->body[i].G > 128)
			imageFile->body[i].G = MAX_DEPTH;
		else
			imageFile->body[i].G = MIN_DEPTH;

		if (imageFile->body[i].B > 128)
			imageFile->body[i].B = MAX_DEPTH;
		else
			imageFile->body[i].B = MIN_DEPTH;
	}
}

/* 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Esta funcao negara o numero do vermelho em cada pixel
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
void negateRed(ppmFile *imageFile) {
	// Se o vermelho for baixo em valor, ele se tornara alto e vice versa
	for (int i = 0; i < imageFile->col * imageFile->row; i++)
		imageFile->body[i].R = abs(imageFile->body[i].R - MAX_DEPTH);
}

/* 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Esta funcao rotaciona a imagem horizontalmente
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
void horizontalFlip(ppmFile *imageFile) {
	// Deslocando o centro da imagem
	int imageCenter = imageFile->col / 2;

	for (int i = 0; i < imageFile->row; ++i) {
		pixel *tmpArray = imageFile->body + i * imageFile->col; // Array temporario de Pixels
		for (int j = 0; j < imageCenter; ++j)
		{
			// Invertendo Pixels
			pixel temp = tmpArray[j];
			tmpArray[j] = tmpArray[imageFile->col - 1 - j];
			tmpArray[imageFile->col - 1 - j] = temp;
		}
	}
}

/* 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Esta funcao rotaciona a imagem verticalmente
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
void verticalFlip(ppmFile *imageFile) {
    int line_size = imageFile->col * sizeof(pixel);
    int imageCenter = imageFile->row / 2;

    // Alocando buff de troca
    pixel* buff = (pixel*)malloc(line_size);
    pixel* top;
    pixel* bottom;

    for (int y = 0; y < imageCenter; ++y) {
        // Realizando troca entre linhas do topo e da base
        top = imageFile->body  + (y * imageFile->col);
        bottom = imageFile->body  + ((imageFile->col - y - 1) * imageFile->col);

        memcpy(buff, top, line_size);
        memcpy(top, bottom, line_size);
        memcpy(bottom, buff, line_size);
    }
}

/* 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Esta funcao aplica escala (zoom) a imagem 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/

void zoom(ppmFile *imageFile) {
	char inputZoom[20];
	int zoomValue = 1;
	FILE *fp; // Ponteiro File para o arquivo de saida

	puts("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	puts("		Edicoes particulares do Zoom em PPM		        ");
	puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

	printf("Insira o nome do arquivo de zoom da imagem: ");
	scanf("%s", inputZoom);


	printf("Insira quanto zoom desejas: ");
	scanf("%d", &zoomValue);

	// Criando o ponteiro de saida
	fp = fopen(inputZoom, "w");
	if (!fp) { // Caso a criacao do arquivo falhe
		printf("Erro: Nao foi possivel criar o arquivo!\n");
		exit(EXIT_FAILURE);
	}

	// Escrevendo o cabecalho da imagem
	fprintf(fp, "P3"); // Numero magico
	fprintf(fp, "   %d %d   ", (imageFile->col * zoomValue), (imageFile->row * zoomValue));  // Colunas e Linhas com o zoom adicionado
	fprintf(fp, "  %d  ", imageFile->colorDepth); // Profundidade maxima de cor
	
    for (int i = 0; i < ((imageFile->row) * (imageFile->col)); i++)
        for (int k = 0; k < zoomValue * zoomValue; k++)
			fprintf(fp, "    %d %d %d    ", 
				(imageFile->body[i].R > imageFile->colorDepth) ? imageFile->colorDepth : imageFile->body[i].R,
				(imageFile->body[i].G > imageFile->colorDepth) ? imageFile->colorDepth : imageFile->body[i].G,
				(imageFile->body[i].B > imageFile->colorDepth) ? imageFile->colorDepth : imageFile->body[i].B
			);
	

	puts("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	puts("  Sua nova imagem PPM com filtros e zoom esta sendo processada...		        ");
	puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

	fclose(fp); // Fechando a imagem criada

	printf("%s esta disponivel!", inputZoom);
}