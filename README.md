# Editor de Imagem Portable Pixmap (PPM)

Esse é um código realizado para o Trabalho 01 da cadeira de Estrutura de Dados, desenvolvido por Laura Boemo.

## Requisitos Funcionais Diponíveis

- [x] Ler imagens no formato PPM/P3
- [x] Escrever imagens no formato PPM/P3
- [x] Aplicação 01 de Filtro de Cor - Aumento de Contraste
- [x] Aplicação 02 de Filtro de Cor - Negativação do Vermelho 
- [x] Aplicação 01 de Filtro de Modificação de Pixels - Rotacionar horizontalmente
- [x] Aplicação 02 de Filtro de Modificação de Pixels - Rotacionar verticalmente
- [ ] Operação de Recorte
- [ ] Operação de Colar
- [x] Operação de Escala
- [ ] Seleção de Subconjuntos

## Requisitos de Estrutura de Dados Diponíveis

- [x] Armazenamento e manipulação das imagens utilizando ponteiros e blocos de memória alocados dinamicamente
- [x] Sempre que possível, a aplicação de filtro/operações não utiliza blocos de memória adicionais

## Relatório

Este código em C implementa um programa simples de edição de imagens no formato PPM (Portable Pixmap). As estruturas de dados desempenham um papel crucial no funcionamento do programa, permitindo a representação, manipulação e processamento eficiente das imagens. Vamos analisar como as estruturas de dados são aplicadas neste código:

1. Estruturas de Pixels (pixel):

A estrutura pixel é usada para representar as cores individuais dos pixels na imagem. Ela contém três campos inteiros para as componentes de cor Red (R), Green (G) e Blue (B).

2. Estrutura da Imagem PPM (ppmFile):

A estrutura ppmFile é usada para representar a imagem PPM como um todo. Ela contém os seguintes campos:

- row: Representa o número de linhas na imagem.
- col: Representa o número de colunas na imagem.
- colorDepth: Representa a profundidade máxima de cor (geralmente 0-255) da imagem PPM.
- body: Um ponteiro para um array de pixels que armazena os dados da imagem.

3. Leitura da Imagem (readImage):

A função readImage usa a estrutura ppmFile para ler um arquivo PPM de entrada. Ela aloca dinamicamente memória para armazenar os dados da imagem e preenche a estrutura com as informações lidas do arquivo.

4. Escrita da Imagem (writeImage):

A função writeImage usa a estrutura ppmFile para escrever os dados da imagem em um novo arquivo PPM de saída. Ela utiliza os campos da estrutura para criar o cabeçalho do arquivo e escreve os pixels da imagem.

5. Filtros de Edição de Imagem (extremeContrast, negateRed, horizontalFlip, verticalFlip, zoom):

Cada uma dessas funções aplica um filtro específico à imagem. Elas usam a estrutura ppmFile para acessar os pixels da imagem e realizar as operações necessárias. Por exemplo, extremeContrast ajusta o contraste da imagem, enquanto horizontalFlip inverte a imagem horizontalmente. Essas funções atualizam os valores dos pixels na estrutura ppmFile.

6. Menu de Opções:

O código permite ao usuário selecionar quais filtros aplicar à imagem por meio de um menu interativo. As escolhas dos usuários são armazenadas em um array de caracteres choice, que é usado para determinar quais filtros executar.
No geral, as estruturas de dados (pixel e ppmFile) são usadas para representar e manipular os dados da imagem, enquanto as funções operam nesses dados para aplicar os filtros desejados. O código é um exemplo prático de como as estruturas de dados são essenciais para processar informações complexas, como imagens, de maneira organizada e eficiente.