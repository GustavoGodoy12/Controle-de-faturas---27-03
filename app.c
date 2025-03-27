/*

  Este código faz:
   - Inserir faturas
   - Buscar faturas
   - Atualizar status de pagamento
   - Remover faturas
   - Listar todas as faturas em ordem crescente
  
  A ideia é usar uma árvore binária de busca (BST) onde cada nó
  representa uma fatura. Assim, podemos inserir e remover as
  faturas mantendo a árvore organizada pelo número da fatura.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Definição da estrutura de cada nó (representando uma Fatura):

typedef struct NOzArvore { 
    int QqNum;             // Número da fatura (chave usada na árvore)
    char QqVenc[50];       // Data de vencimento da fatura
    float QqValor;         // Valor da fatura
    char QqStatus[10];     // Pode ser "pendente" ou "paga"
    struct NOzArvore *QqEsq;  // Ponteiro para o filho esquerdo
    struct NOzArvore *QqDir;  // Ponteiro para o filho direito
} NOzArvore;

// Cria um novo nó de fatura, preenchendo os dados e retornando ponteiro para ele

NOzArvore* CriaNohFatura(int xxNumero, const char *xxVenc, float xxVal, const char *xxStat) 
{
    // Aloca espaço na memória para o novo nó
    NOzArvore* vNovo = (NOzArvore*)malloc(sizeof(NOzArvore));

    // Atribui valores recebidos nos campos da fatura
    vNovo->QqNum = xxNumero;
    strcpy(vNovo->QqVenc, xxVenc);
    vNovo->QqValor = xxVal;
    strcpy(vNovo->QqStatus, xxStat);

    // Inicia os ponteiros esquerdo e direito como NULL
    vNovo->QqEsq = NULL;
    vNovo->QqDir = NULL;

    // Retorna o novo nó criado
    return vNovo;
}

// Insere uma nova fatura na árvore seguindo as regras da BST (por número)

NOzArvore* EncaixaFaturaBST(NOzArvore *Raiz, NOzArvore *NovoNoh)
{
    // Se a árvore estiver vazia (Raiz == NULL), o novo nó se torna a raiz
    if (!Raiz) return NovoNoh;

    // Se o número do novo nó for menor, vai para a esquerda
    if (NovoNoh->QqNum < Raiz->QqNum) {
        Raiz->QqEsq = EncaixaFaturaBST(Raiz->QqEsq, NovoNoh);
    } 
    // Se for maior ou igual, insere à direita
    else {
        Raiz->QqDir = EncaixaFaturaBST(Raiz->QqDir, NovoNoh);
    }

    // Retorna a raiz (que não mudou)
    return Raiz;
}


// Busca uma fatura pelo número. Se encontrar, retorna o ponteiro do nó, se não, retorna NULL

NOzArvore* LocalizaFatura(NOzArvore *Raiz, int nFat)
{
    // Se chegou num nó vazio, não encontrou
    if (!Raiz) return NULL;

    // Se o número do nó for o que buscamos, retornamos
    if (Raiz->QqNum == nFat) {
        return Raiz;
    }
    // Se o número buscado é menor, busca no filho esquerdo
    else if (nFat < Raiz->QqNum) {
        return LocalizaFatura(Raiz->QqEsq, nFat);
    }
    // Se for maior, busca no filho direito
    else {
        return LocalizaFatura(Raiz->QqDir, nFat);
    }
}

// Exibe todas as faturas em ordem crescente usando percurso in-ordem

void EhhExibirOrd(NOzArvore *Raiz)
{
    // Se a raiz for nula, não faz nada
    if (Raiz == NULL) return;

    // Visita primeiro o lado esquerdo
    EhhExibirOrd(Raiz->QqEsq);

    // Exibe as informações do nó atual
    printf("\n----------------------------\n");
    printf("Numero da Fatura: %d\n", Raiz->QqNum);
    printf("Data de Vencimento: %s\n", Raiz->QqVenc);
    printf("Valor: %.2f\n", Raiz->QqValor);
    printf("Status: %s\n", Raiz->QqStatus);
    printf("----------------------------\n");

    // Depois visita o lado direito
    EhhExibirOrd(Raiz->QqDir);
}

// Atualiza o status de uma fatura (para "paga" ou "pendente"), caso exista

void AlteraStatusFatura(NOzArvore *Raiz, int nFat, const char *novoStat)
{
    // Primeiro busca o nó
    NOzArvore *Encontrado = LocalizaFatura(Raiz, nFat);
    if (Encontrado) {
        // Se encontrou, altera o status
        strcpy(Encontrado->QqStatus, novoStat);
        printf("\nStatus da Fatura %d atualizado para '%s'.\n", nFat, novoStat);
    } else {
        // Se não encontrou, avisa
        printf("\nFatura %d nao foi encontrada.\n", nFat);
    }
}


// Encontra o nó de valor mínimo na árvore (vai sempre para a esquerda)

NOzArvore* AchaMinimo(NOzArvore *Raiz)
{
    NOzArvore *Atual = Raiz;
    // Enquanto houver filho à esquerda, continua
    while (Atual && Atual->QqEsq != NULL) {
        Atual = Atual->QqEsq;
    }
    return Atual;
}

// Remove uma fatura pelo número. Se não existir, não faz nada

NOzArvore* RetiraFaturaBST(NOzArvore *Raiz, int nFat)
{
    // Se a árvore estiver vazia, não tem o que remover
    if (!Raiz) {
        return NULL;
    }

    // Se o número a remover for menor que o nó atual, vai para a esquerda
    if (nFat < Raiz->QqNum) {
        Raiz->QqEsq = RetiraFaturaBST(Raiz->QqEsq, nFat);
    }
    // Se for maior, vai para a direita
    else if (nFat > Raiz->QqNum) {
        Raiz->QqDir = RetiraFaturaBST(Raiz->QqDir, nFat);
    }
    // Se for igual, encontramos o nó a remover
    else {
        // Caso 1: nó sem filho esquerdo
        if (Raiz->QqEsq == NULL) {
            NOzArvore *temp = Raiz->QqDir;
            free(Raiz);
            return temp;
        }
        // Caso 2: nó sem filho direito
        else if (Raiz->QqDir == NULL) {
            NOzArvore *temp = Raiz->QqEsq;
            free(Raiz);
            return temp;
        }
        // Caso 3: nó com dois filhos
        NOzArvore *temp = AchaMinimo(Raiz->QqDir); // Busca o menor na subárvore direita
        // Copia os dados do sucessor para o nó atual
        Raiz->QqNum = temp->QqNum;
        strcpy(Raiz->QqVenc, temp->QqVenc);
        Raiz->QqValor = temp->QqValor;
        strcpy(Raiz->QqStatus, temp->QqStatus);
        // Remove o sucessor original
        Raiz->QqDir = RetiraFaturaBST(Raiz->QqDir, temp->QqNum);
    }
    return Raiz;
}

// Função principal: apresenta um menu e executa as operações na BST

int main(void)
{
    // Declaramos a raiz da BST
    NOzArvore *RaizFaturas = NULL;  

    // Variáveis usadas para o menu e para receber dados
    int Opcao = -1;       
    int NumFat;           
    char DataVenc[50];    
    float ValorFat;       
    char Stat[10];        

    do {
        // Menu de opções
        printf("\n- Controle de faturas -\n");
        printf("1- Inserir Fatura\n");
        printf("2- Buscar Fatura\n");
        printf("3- Atualizar Status de Fatura\n");
        printf("4- Remover Fatura\n");
        printf("5- Exibir Todas as Faturas\n");
        printf("0- Sair\n");
        printf("opcao: ");
        scanf("%d", &Opcao);
        getchar(); // Para consumir o "enter" deixado pelo scanf

        switch (Opcao) {

            case 1: 
                // Insere fatura
                printf("\nDigite o numero da fatura: ");
                scanf("%d", &NumFat);
                getchar();

                printf("Digite a data de vencimento: ");
                fgets(DataVenc, 50, stdin);
                // Retira o '\n' que o fgets coloca ao final
                DataVenc[strcspn(DataVenc, "\n")] = '\0';

                printf("Digite o valor: ");
                scanf("%f", &ValorFat);
                getchar();

                printf("Digite o status ('pendente' ou 'paga'): ");
                fgets(Stat, 10, stdin);
                Stat[strcspn(Stat, "\n")] = '\0';

                // Cria o nó e insere na árvore
                {
                    NOzArvore* NovoNoh = CriaNohFatura(NumFat, DataVenc, ValorFat, Stat);
                    RaizFaturas = EncaixaFaturaBST(RaizFaturas, NovoNoh);
                }
                printf("\nFatura inserida com sucesso!\n");
                break;

            case 2:
                // Buscar fatura
                printf("\nInforme o numero da fatura para busca: ");
                scanf("%d", &NumFat);
                {
                    NOzArvore *Res = LocalizaFatura(RaizFaturas, NumFat);
                    if (Res) {
                        // Se encontrou, mostra os dados
                        printf("\nFatura Encontrada!\n");
                        printf("Numero: %d\n", Res->QqNum);
                        printf("Data de Vencimento: %s\n", Res->QqVenc);
                        printf("Valor: %.2f\n", Res->QqValor);
                        printf("Status: %s\n", Res->QqStatus);
                    } else {
                        // Se não encontrou, avisa
                        printf("\nFatura nao encontrada.\n");
                    }
                }
                break;

            case 3:
                // Atualiza status
                printf("\nInforme o numero da fatura para atualizar: ");
                scanf("%d", &NumFat);
                getchar();
                printf("Digite o novo status ('pendente' ou 'paga'): ");
                fgets(Stat, 10, stdin);
                Stat[strcspn(Stat, "\n")] = '\0';
                AlteraStatusFatura(RaizFaturas, NumFat, Stat);
                break;

            case 4:
                // Remove fatura
                printf("\nInforme o numero da fatura a remover: ");
                scanf("%d", &NumFat);
                RaizFaturas = RetiraFaturaBST(RaizFaturas, NumFat);
                printf("\nSe a fatura existia, foi removida.\n");
                break;

            case 5:
                // Exibir todas as faturas
                if (!RaizFaturas) {
                    printf("\nNenhuma fatura cadastrada.\n");
                } else {
                    printf("\n-- Faturas em ordem crescente --\n");
                    EhhExibirOrd(RaizFaturas);
                }
                break;

            case 0:
                // Sai do programa
                printf("\nSaindo...\n");
                break;

            default:
                // Caso o usuário digite algo fora das opções
                printf("\nOpcao invalida! Tente novamente.\n");
        }

    } while (Opcao != 0); // Continua até o usuário escolher sair

    // Se quiséssemos, poderíamos liberar a memória de toda a árvore aqui, percorrendo-a e dando free.

    return 0;
}
