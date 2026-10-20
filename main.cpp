/*
 * ===========================================================================
 *  Sistema de Gerenciamento de Produtos - Arvore AVL
 * ===========================================================================
 *  Disciplina: Estrutura de Dados
 *  Curso: Analise e Desenvolvimento de Sistemas - Fatec Araras
 *  Aluno:  Odil Klein da Silva Neto
 *  Professor: Ederaldo Luiz Ratz
 * --------------------------------------------------------------------------
 *  A AVL e uma ABB auto-balanceavel: a diferenca e que cada no guarda a
 *  sua ALTURA e, a cada insercao/remocao, o algoritmo calcula o FATOR DE
 *  BALANCEAMENTO (fb = altura(esq) - altura(dir)) e aplica rotacoes quando
 *  |fb| > 1, garantindo que a altura seja sempre O(log n).
 *
 *  Convencao de altura usada: no folha = 1; arvore vazia = 0.
 *  Fator de balanceamento da raiz 50 (esq=2 niveis, dir=2 niv.) = 2 - 2 = 0.
 *
 *  Casos de rotação tratados:
 *    LL -> rotacao simples a direita
 *    RR -> rotacao simples a esquerda
 *    LR -> rotacao simples a esquerda no filho, depois a direita na raiz
 *    RL -> rotacao simples a direita no filho, depois a esquerda na raiz
 *
 *  Sem STL (proibido pela atividade): struct, ponteiros, new/delete,
 *  recursao e char[] para texto.
 * ===========================================================================
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>

/* --------------------------- ENTIDADES ---------------------------------- */

struct Produto {
    int  codigo;               /* chave unica (repetido = rejeitado)       */
    char descricao[100];       /* descricao livre (definida pelo aluno)    */
};

/* No da AVL: alem dos dados e dos filhos, guarda a ALTURA, que e o campo
 * que permite calcular o fator de balanceamento em O(1). */
struct No {
    Produto produto;
    No    *esquerdo;
    No    *direito;
    int    altura;             /* convencao: folha = 1, vazia = 0          */
};

/* ---------------------- FUNCOES AUXILIARES ------------------------------ */

int altura(No *n) {
    return (n == NULL) ? 0 : n->altura;      /* arvore vazia tem altura 0 */
}

int max2(int a, int b) {
    return (a > b) ? a : b;
}

/* Fator de balanceamento: positivo = subarvore esquerda mais alta. */
int fatorBalanceamento(No *n) {
    return altura(n->esquerdo) - altura(n->direito);
}

/* Atualiza a altura de um no a partir das alturas dos filhos. */
void atualizarAltura(No *n) {
    if (n != NULL)
        n->altura = 1 + max2(altura(n->esquerdo), altura(n->direito));
}

No* criarNo(const Produto &p) {
    No *novo = new No;
    novo->produto   = p;
    novo->esquerdo  = NULL;
    novo->direito   = NULL;
    novo->altura    = 1;                     /* no isolated = folha       */
    return novo;
}

/* --------------------------- ROTACOES ----------------------------------- */

/* Rotacao simples a DIREITA (caso LL).
 *
 *      z                y
 *     / \             /   \
 *    y   T4   ->     x     z
 *   / \             / \   / \
 *  x   T3          T1 T2 T3 T4
 */
No* rotacaoDireita(No *z) {
    No *y  = z->esquerdo;
    No *T3 = y->direito;

    y->direito  = z;                /* z desce, y sobe                     */
    z->esquerdo = T3;

    atualizarAltura(z);             /* primeiro o filho, depois o pai      */
    atualizarAltura(y);
    return y;                       /* nova raiz da subarvore              */
}

/* Rotacao simples a ESQUERDA (caso RR) - espelho da anterior. */
No* rotacaoEsquerda(No *z) {
    No *y  = z->direito;
    No *T2 = y->esquerdo;

    y->esquerdo = z;
    z->direito  = T2;

    atualizarAltura(z);
    atualizarAltura(y);
    return y;
}

/* ------------------------- INSERCAO AVL --------------------------------- */

/* Insere recursivamente e, na subida da recursao, atualiza alturas e
 * aplica a rotacao cabivel (LL, RR, LR ou RL). Retorna a nova raiz. */
No* inserir(No *raiz, const Produto &p) {
    /* 1) insercao normal de ABB */
    if (raiz == NULL)
        return criarNo(p);

    if (p.codigo == raiz->produto.codigo) {
        printf("  [!] ERRO: codigo %d ja cadastrado para \"%s\". "
               "Produto nao inserido.\n",
               p.codigo, raiz->produto.descricao);
        return raiz;                       /* rejeita duplicado          */
    }

    if (p.codigo < raiz->produto.codigo)
        raiz->esquerdo = inserir(raiz->esquerdo, p);
    else
        raiz->direito  = inserir(raiz->direito, p);

    /* 2) atualiza a altura do no atual (subida da recursao) */
    atualizarAltura(raiz);

    /* 3) calcula o fator de balanceamento */
    int fb = fatorBalanceamento(raiz);

    /* Se |fb| > 1, a subarvore ficou desbalanceada; decide o caso
     * comparando o fator do FILHO mais alto: */

    /* Caso LL: esquerda-esquerda -> rotacao simples a direita */
    if (fb > 1 && p.codigo < raiz->esquerdo->produto.codigo)
        return rotacaoDireita(raiz);

    /* Caso RR: direita-direita -> rotacao simples a esquerda */
    if (fb < -1 && p.codigo > raiz->direito->produto.codigo)
        return rotacaoEsquerda(raiz);

    /* Caso LR: esquerda-direita -> rotacao a esquerda no filho esq.,
     * depois rotacao a direita na raiz */
    if (fb > 1 && p.codigo > raiz->esquerdo->produto.codigo) {
        raiz->esquerdo = rotacaoEsquerda(raiz->esquerdo);
        return rotacaoDireita(raiz);
    }

    /* Caso RL: direita-esquerda -> rotacao a direita no filho dir.,
     * depois rotacao a esquerda na raiz */
    if (fb < -1 && p.codigo < raiz->direito->produto.codigo) {
        raiz->direito = rotacaoDireita(raiz->direito);
        return rotacaoEsquerda(raiz);
    }

    return raiz;                             /* ja balanceado            */
}

/* ---------------------------- BUSCA ------------------------------------- */

No* buscar(No *raiz, int codigo) {
    if (raiz == NULL) return NULL;
    if (codigo == raiz->produto.codigo) return raiz;
    if (codigo < raiz->produto.codigo)
        return buscar(raiz->esquerdo, codigo);
    return buscar(raiz->direito, codigo);
}

/* ------------------------ LISTAGEM EM ORDEM ----------------------------- */

void listarEmOrdem(No *raiz) {
    if (raiz == NULL) return;
    listarEmOrdem(raiz->esquerdo);
    printf("  %-6d %s\n", raiz->produto.codigo, raiz->produto.descricao);
    listarEmOrdem(raiz->direito);
}

/* ---------------------- INFORMACOES DA ARVORE --------------------------- */

int contarNos(No *raiz) {
    if (raiz == NULL) return 0;
    return 1 + contarNos(raiz->esquerdo) + contarNos(raiz->direito);
}

void mostrarInformacoes(No *raiz) {
    if (raiz == NULL) {
        printf("  Arvore vazia: nenhum produto cadastrado.\n");
        return;
    }
    printf("  Quantidade de produtos : %d\n", contarNos(raiz));
    printf("  Altura da arvore       : %d\n", altura(raiz));
    printf("  Codigo na raiz         : %d\n", raiz->produto.codigo);
    printf("  Fator de bal. da raiz  : %+d\n", fatorBalanceamento(raiz));
}

/* ---------------- VALIDACAO DO BALANCEAMENTO ---------------------------- */

/* Percorre TODOS os nos verificando se o fb esta entre -1 e +1 (inclusive).
 * Tambem confere se a altura armazenada esta coerente com a estrutura. */
int nosDesbalanceados(No *raiz) {
    if (raiz == NULL) return 0;

    int conta = 0;
    int fb = fatorBalanceamento(raiz);
    if (fb < -1 || fb > 1) {
        printf("  [!] No %d DESBALANCEADO: fator = %+d\n", raiz->produto.codigo, fb);
        conta++;
    }
    /* bonus: altura armazenada incorreta tambem seria erro de implementacao */
    int alturaReal = 1 + max2(altura(raiz->esquerdo), altura(raiz->direito));
    if (raiz->altura != alturaReal) {
        printf("  [!] No %d com ALTURA INCOERENTE: armazenada %d, real %d\n",
               raiz->produto.codigo, raiz->altura, alturaReal);
        conta++;
    }
    return conta + nosDesbalanceados(raiz->esquerdo)
                 + nosDesbalanceados(raiz->direito);
}

void validarBalanceamento(No *raiz) {
    if (raiz == NULL) {
        printf("  Arvore vazia: nada a validar.\n");
        return;
    }
    int problemas = nosDesbalanceados(raiz);
    if (problemas == 0)
        printf("  [ok] ARVORE BALANCEADA: todos os nos tem fator "
               "entre -1 e +1.\n");
    else
        printf("  [!] %d problema(s) de balanceamento encontrado(s).\n", problemas);
}

/* ---------------------- REMOCAO (DESAFIO) ------------------------------- */

No* menorNo(No *raiz) {
    while (raiz && raiz->esquerdo != NULL)
        raiz = raiz->esquerdo;
    return raiz;
}

/* Remove mantendo o balanceamento: apos o descarte, a subida da recursao
 * atualiza alturas e rotaciona os mesmos quatro casos da insercao. */
No* remover(No *raiz, int codigo) {
    if (raiz == NULL) {
        printf("  [!] Codigo %d nao encontrado na arvore.\n", codigo);
        return NULL;
    }

    if (codigo < raiz->produto.codigo)
        raiz->esquerdo = remover(raiz->esquerdo, codigo);
    else if (codigo > raiz->produto.codigo)
        raiz->direito  = remover(raiz->direito, codigo);
    else {
        /* achou o no */
        if (raiz->esquerdo == NULL && raiz->direito == NULL) {
            delete raiz;                       /* folha                  */
            return NULL;
        }
        if (raiz->esquerdo == NULL) {
            No *tmp = raiz->direito;           /* um filho (direito)     */
            delete raiz;
            return tmp;
        }
        if (raiz->direito == NULL) {
            No *tmp = raiz->esquerdo;          /* um filho (esquerdo)    */
            delete raiz;
            return tmp;
        }
        /* dois filhos: substitui pelo sucessor em-ordem */
        No *sucessor = menorNo(raiz->direito);
        raiz->produto = sucessor->produto;
        raiz->direito = remover(raiz->direito, sucessor->produto.codigo);
    }

    /* rebalanceamento na subida (igual a insercao) */
    atualizarAltura(raiz);
    int fb = fatorBalanceamento(raiz);

    if (fb > 1 && fatorBalanceamento(raiz->esquerdo) >= 0)
        return rotacaoDireita(raiz);                /* LL                  */

    if (fb > 1 && fatorBalanceamento(raiz->esquerdo) < 0) {
        raiz->esquerdo = rotacaoEsquerda(raiz->esquerdo);   /* LR          */
        return rotacaoDireita(raiz);
    }

    if (fb < -1 && fatorBalanceamento(raiz->direito) <= 0)
        return rotacaoEsquerda(raiz);               /* RR                  */

    if (fb < -1 && fatorBalanceamento(raiz->direito) > 0) {
        raiz->direito = rotacaoDireita(raiz->direito);     /* RL          */
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

/* ------------------------- LIBERACAO ------------------------------------ */

void liberar(No *raiz) {
    if (raiz == NULL) return;
    liberar(raiz->esquerdo);
    liberar(raiz->direito);
    delete raiz;
}

/* ------------------------- PROGRAMA ------------------------------------- */

int main() {
    No     *raiz = NULL;
    int     opcao, codigo;
    Produto p;

    do {
        printf("\n+--------------------------------------------+\n");
        printf("|   GERENCIAMENTO DE PRODUTROS - AVL        |\n");
        printf("+--------------------------------------------+\n");
        printf("| 1) Cadastrar produto (insere + balanceia)  |\n");
        printf("| 2) Consultar produto (por codigo)          |\n");
        printf("| 3) Listar produtos em ordem crescente      |\n");
        printf("| 4) Informacoes da arvore                   |\n");
        printf("| 5) Validar balanceamento                   |\n");
        printf("| 6) Remover produto (desafio adicional)     |\n");
        printf("| 0) Encerrar (libera a memoria)             |\n");
        printf("+--------------------------------------------+\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) break;

        switch (opcao) {
        case 1:                                    /* CADASTRAR */
            printf("Codigo: ");
            scanf("%d", &p.codigo);
            printf("Descricao: ");
            scanf(" %99[^\n]", p.descricao);
            if (buscar(raiz, p.codigo) != NULL) {
                printf("  [!] ERRO: codigo %d ja cadastrado para \"%s\". "
                       "Produto nao inserido.\n",
                       p.codigo, buscar(raiz, p.codigo)->produto.descricao);
            } else {
                raiz = inserir(raiz, p);
                printf("  [ok] Produto %d inserido. | altura atual: %d | "
                       "raiz: %d | fb raiz: %+d\n",
                       p.codigo, altura(raiz), raiz->produto.codigo,
                       fatorBalanceamento(raiz));
            }
            break;

        case 2:                                    /* CONSULTAR */
            printf("Codigo a consultar: ");
            scanf("%d", &codigo);
            {
                No *achado = buscar(raiz, codigo);
                if (achado == NULL)
                    printf("  [!] Produto de codigo %d NAO encontrado.\n", codigo);
                else
                    printf("  [ok] Produto %d ENCONTRADO: %s\n",
                           achado->produto.codigo, achado->produto.descricao);
            }
            break;

        case 3:                                    /* EM ORDEM */
            if (raiz == NULL) printf("  Arvore vazia.\n");
            else {
                printf("  %-6s %s\n", "CODIGO", "DESCRICAO");
                listarEmOrdem(raiz);
            }
            break;

        case 4:                                    /* INFORMACOES */
            mostrarInformacoes(raiz);
            break;

        case 5:                                    /* VALIDAR */
            validarBalanceamento(raiz);
            break;

        case 6:                                    /* REMOVER */
            printf("Codigo a remover: ");
            scanf("%d", &codigo);
            raiz = remover(raiz, codigo);
            printf("  [ok] Remocao processada. | altura: %d | raiz: %d\n",
                   altura(raiz), raiz ? raiz->produto.codigo : 0);
            break;

        case 0:
            printf("  Liberando memoria e encerrando...\n");
            liberar(raiz);
            raiz = NULL;
            break;

        default:
            printf("  Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}
