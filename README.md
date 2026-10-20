# Sistema de Gerenciamento de Produtos — Árvore AVL

**Disciplina:** Estrutura de Dados  
**Curso:** Desenvolvimento de Software Multiplataforma — Fatec Araras  
**Aluno:** Odil Klein da Silva Neto  
**Professor:** Ederaldo Luiz Ratz  
**Linguagem:** C++ (sem contêineres da STL — `struct`, ponteiros,
`new`/`delete`, recursão e `char[]`)

Protótipo para uma distribuidora de equipamentos eletrônicos: cadastro de
produtos por código único + descrição em uma **Árvore AVL**, que se
**auto-balanceia a cada inserção/remoção** por meio de rotações LL, RR, LR e
RL, mantendo a busca em O(log n) mesmo com inserções ordenadas.

## Como compilar e executar

```bash
g++ -Wall -o avl main.cpp
./avl
```

## Funcionalidades implementadas

| Requisito | Função | Observação |
|---|---|---|
| 1 — Inserção | `inserir()` | Atualiza alturas na subida da recursão e rotaciona (LL/RR/LR/RL). Códigos repetidos são **rejeitados**. |
| 2 — Busca | `buscar()` | Mensagem de encontrado/não encontrado; apresenta a descrição. |
| 3 — Listagem ordenada | `listarEmOrdem()` | Percurso esquerda → raiz → direita (crescente). |
| 4 — Informações | `mostrarInformacoes()` | Total de produtos, altura, código da raiz e fator de bal. da raiz. |
| 5 — Validação | `validarBalanceamento()` | Percorre todos os nós: fb ∈ [−1, +1] e altura coerente. |
| 6 — Liberação | `liberar()` | Pós-ordem, na opção 0. |
| Desafio | `remover()` | Remoção com rebalanceamento (folha, um filho, dois filhos). |

**Convenção de altura:** folha = 1, árvore vazia = 0.

---

## Estrutura do nó

```c
struct No {
    Produto produto;   // codigo (chave) + descricao
    No *esquerdo;      // subárvore esquerda (códigos menores)
    No *direito;       // subárvore direita  (códigos maiores)
    int altura;        // altura do nó (folha = 1) — permite fb em O(1)
};
```

Fator de balanceamento: `fb = altura(esquerdo) − altura(direito)`.
Positivo = esquerda mais alta; negativo = direita mais alta.

---

## Casos de rotação

**LL — rotação simples à direita**
```
      30              20
     /       ->      /  \
   20              10    30
  /
10
```

**RR — rotação simples à esquerda**
```
10              20
  \      ->    /  \
   20        10    30
     \
      30
```

**LR — rotação dupla (esquerda no filho, depois direita na raiz)**
```
  30            30          20
 /      ->      /    ->    /  \
10            20         10    30
  \          /
   20       10
```

**RL — rotação dupla (direita no filho, depois esquerda na raiz)**
```
10            10          20
  \    ->       \   ->   /  \
   30           20      10   30
  /               \
 20               30
```

---

## Testes obrigatórios — evidências de execução

Cada teste foi executado de fato (programa compilado com `g++ -Wall -Wextra`).
A árvore esperada pelo enunciado em todos os casos: **raiz 20, esquerda 10,
direita 30**.

### Teste A — caso LL (30, 20, 10)

```
[ok] Produto 30 inserido. | altura atual: 1 | raiz: 30 | fb raiz: +0
[ok] Produto 20 inserido. | altura atual: 2 | raiz: 30 | fb raiz: +1
[ok] Produto 10 inserido. | altura atual: 2 | raiz: 20 | fb raiz: +0
```
Ao final: raiz **20**, filhos **10** e **30** — ✔ conforme o enunciado.
A rotação aconteceu ao inserir o 10 (fb +2 na raiz 30, filho esquerdo 20
com fb +1 ⇒ caso LL).

### Teste B — caso RR (10, 20, 30)

```
[ok] Produto 10 inserido. | altura atual: 1 | raiz: 10 | fb raiz: +0
[ok] Produto 20 inserido. | altura atual: 2 | raiz: 10 | fb raiz: -1
[ok] Produto 30 inserido. | altura atual: 2 | raiz: 20 | fb raiz: +0
```
Raiz **20**, filhos **10** e **30** — ✔. Rotação à esquerda na inserção do 30.

### Teste C — caso LR (30, 10, 20)

```
[ok] Produto 30 inserido. | altura atual: 1 | raiz: 30 | fb raiz: +0
[ok] Produto 10 inserido. | altura atual: 2 | raiz: 30 | fb raiz: +1
[ok] Produto 20 inserido. | altura atual: 2 | raiz: 20 | fb raiz: +0
```
Raiz **20**, filhos **10** e **30** — ✔. Rotação dupla (esquerda no 10,
direita no 30) na inserção do 20.

### Teste D — caso RL (10, 30, 20)

```
[ok] Produto 10 inserido. | altura atual: 1 | raiz: 10 | fb raiz: +0
[ok] Produto 30 inserido. | altura atual: 2 | raiz: 10 | fb raiz: -1
[ok] Produto 20 inserido. | altura atual: 2 | raiz: 20 | fb raiz: +0
```
Raiz **20**, filhos **10** e **30** — ✔. Rotação dupla (direita no 30,
esquerda no 10) na inserção do 20.

### Teste E — cenário integrado (50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45)

Árvore resultante (todas as inserções couberam sem rotação — os desvios
nunca passaram de 1):

```
                50                fb = +1
              /     \
            30       70           fb(30) = 0    fb(70) = -1
           /  \     /  \
         20   40   60   80        fb(20) = 0    fb(40) = 0
        /  \   / \
      10    25 35  45
```

Resultados da execução:

```
Quantidade de produtos : 11
Altura da arvore       : 4
Codigo na raiz         : 50
Fator de bal. da raiz  : +1
[ok] ARVORE BALANCEADA: todos os nos tem fator entre -1 e +1.
```

Percurso em ordem (crescente):

```
10, 20, 25, 30, 35, 40, 45, 50, 60, 70, 80
```

Contraste com a ABB da outra atividade: com **os mesmos 9 primeiros
códigos** (50…45), a ABB também ficou com altura 3, mas isso é sorte da
sequência — a AVL **garante** altura mínima para qualquer ordem.

### Extras verificados

- **Código duplicado:** `1` → `30` novamente ⇒
  `[!] ERRO: codigo 30 ja cadastrado... Produto nao inserido.` (total segue 11).
- **Busca:** 30 encontrado; 99 ⇒ `NAO encontrado`.
- **Remoção (desafio adicional):** remover 50 (raiz, dois filhos ⇒ entrou o
  sucessor 60 na raiz) e remover 35 (folha); após cada remoção a
  `validarBalanceamento` continuou respondendo `ARVORE BALANCEADA` e a listagem
  em ordem permaneceu crescente.
- **Memória:** compilado com `-fsanitize=address` e executado com o teste
  completo — **zero vazamentos e zero erros** (exit 0, sem relatórios).

---

## Análise e reflexão (questões do enunciado)

**1. Por que uma ABB convencional pode perder eficiência conforme a ordem de
inserção?**  
A ABB não tem mecanismo de balanceamento: sua forma reflete apenas a ordem
de chegada das chaves. Inserindo códigos já crescentes (10, 20, 30, 40…)
cada novo maior vira filho direito do anterior e a árvore degenera numa
cadeia de altura n−1. A busca deixa de descartar metade do conjunto a cada
passo e cai de O(log n) para O(n) — igual a uma lista encadeada.

**2. Qual a finalidade de armazenar a altura em cada nó?**  
O fator de balanceamento depende das alturas dos dois filhos. Calculando a
altura “do zero” a cada inserção, cada passo custaria O(n) e anularia o
ganho da estrutura. Guardando a altura no próprio nó, o fb de um nó é
obtido em **O(1)** e basta atualizá-lo na subida da recursão — o custo total
da inserção permanece O(log n).

**3. Diferença entre rotação simples e rotação dupla?**  
A **simples** (LL ou RR) resolve o desbalanceamento com um único giro de três
nós: o filho sobe e a raiz desce como seu filho. A **dupla** (LR ou RL) é
necessária quando o desvio está “curvado”: primeiro faz-se uma rotação
simples *no filho* para endireitar o caso, e só então a rotação *na raiz*.
No LR, por exemplo, gira-se o filho esquerdo à esquerda e depois a raiz à
direita.

**4. Por que a rotação não compromete a ordenação dos elementos?**  
Porque ela apenas **reatribui ponteiros** entre poucos nós, seguindo a ordem
in-ordem preservada: em toda rotação o nó que sobe é “entre” o que desce e o
terceiro nó (no in-ordem eles aparecem na mesma sequência antes e depois).
Nenhuma chave é alterada nem movida para o lado errado em relação às demais,
logo a propriedade da ABB (esquerda < raiz < direita) continua valendo.

**5. Complexidade de pior caso da busca em uma AVL × ABB comum?**  
- **AVL:** O(log n) no pior caso — o balanceamento garante altura ≤ 1,44·log₂(n+2),
  então mesmo o pior caso descarta uma fração constante do conjunto a cada passo.  
- **ABB comum:** O(n) no pior caso (sequência ordenada degenera em cadeia).  
No caso médio as duas são O(log n), mas só a AVL tem o pior caso controlado.

**6. O que deve acontecer quando um produto com código já cadastrado é inserido
novamente?**  
O programa **rejeita a inserção**: emite aviso (`[!] ERRO: codigo X ja
cadastrado`) e não cria novo nó nem altera o existente. A unicidade da chave é
o que mantém a propriedade da ABB/AVL — dois nós com o mesmo código tornariam
as comparações ambíguas e invalidariam a busca binária. A verificação é feita
antes de chamar `inserir` (evita mensagem contraditória) e, defensivamente,
dentro da própria `inserir`.

**7. A raiz da árvore pode mudar depois de uma inserção?**  
Sim. Quando o desbalanceamento ocorre na raiz, a rotação a promove a raiz da
subárvore desbalanceada — que é a árvore inteira quando se trata da raiz
global. Nos testes A–D, por exemplo, cada inserção final (10, 30, 20, 20)
fez o **20** virar a nova raiz. A função `inserir` retorna a nova raiz e o
chamador (`main`) reatribui o ponteiro `raiz` — é por isso que a recursão
retorna `No*` em vez de `void`.

**8. Por que é necessário atualizar as alturas após uma rotação?**  
A rotação muda a profundidade dos nós envolvidos (o que desce pode ganhar
uma subárvore mais alta; o que sobe, obrigatoriamente, fica mais alto que os
dois lados). Se as alturas não forem recalculadas, o fb calculado na volta da
recursão usaria valores velhos, e a árvore poderia “acreditar” estar
balanceada quando não está — ou rotacionar desnecessariamente. Por isso
`rotacaoDireita`/`rotacaoEsquerda` chamam `atualizarAltura` em ordem: primeiro
o nó que desce (z), depois o que sobe (y).

---

## Conclusão — vantagens e limitações da AVL

A AVL resolve o problema de desempenho da ABB mantendo a árvore sempre quase
perfeitamente balanceada: como a diferença de altura entre as subárvores de
qualquer nó fica limitada a 1, a altura é garantidamente O(log n), o que
preserva busca, inserção e remoção em tempo logarítmico mesmo pior caso —
é a escolha certa para sistemas de **consulta frequente**, como o catálogo da
distribuidora. O custo dessa garantia é o **overhead das rotações e da
manutenção da altura**: cada inserção/remoção pode executar até duas rotações
e percorrer o caminho da folha até a raiz atualizando alturas, tornando a
escrita um pouco mais cara e a implementação visivelmente mais complexa que a
de uma ABB. Além disso, a AVL não é ideal quando as operações de escrita
dominam muito as de leitura — nesse cenário estruturas com balanceamento mais
frouxo (rubro-negra) ou menos rotacionáveis seriam preferíveis. Para o caso
proposto, onde cada consulta deve ser rápida entre milhares de produtos, o
balanceamento rigoroso da AVL paga com folga o custo extra das rotações.

---

## Estrutura do repositório

```
main.cpp       # programa completo (inserção, busca, listagem, informações,
               # validação, remoção desafio, liberação de memória)
teste_E.txt    # entradas do teste E + extras (duplicado, busca, remoções)
README.md      # este documento
```
