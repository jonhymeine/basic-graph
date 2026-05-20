# basic-graph

Projeto em C++ para representar grafos com duas implementacoes:

- `MatrixGraph` (matriz de adjacencia)
- `ListGraph` (lista de adjacencia)

O programa principal lê um arquivo de entrada, cria o tipo de grafo escolhido via argumento e executa apenas o grupo de algoritmos indicado.

## Pre-requisitos

- `g++` com suporte a C++17

## Estrutura Principal

- `src/main.cpp`: ponto de entrada da aplicacao
- `src/graph/`: implementacoes e interfaces dos grafos
  - `graph.h`
  - `list_graph.h` e `list_graph.cpp`
  - `matrix_graph.h` e `matrix_graph.cpp`
- `src/search_algorithms/`: algoritmos de busca e caminho minimo
  - `bfs.h`
  - `dfs.h`
  - `dijkstra.h`
- `src/minimum_spanning_tree_algorithms/`: algoritmo de arvore geradora minima
  - `prim.h`
- `src/coloring_algorithms/`: algoritmos de coloracao de grafos
  - `greedy_coloring.h`
  - `welsh_powell.h`
  - `dsatur.h`
- `src/unit_tests/`: testes unitarios das estruturas de grafo
  - `list_graph_test.cpp`
  - `matrix_graph_test.cpp`
- `graph_examples/`: exemplos de arquivos de entrada
- `build/`: saida gerada pelo Makefile

## Build com Makefile

Na raiz do projeto, use os alvos do `Makefile`:

```bash
make main
```

Esse comando:

- cria a pasta `build/`
- compila `src/graph/list_graph.cpp` em `build/list_graph.o`
- compila `src/graph/matrix_graph.cpp` em `build/matrix_graph.o`
- gera o executavel principal em `build/main.o`

Outros alvos disponiveis:

```bash
make build-list
make build-matrix
make test-list
make test-matrix
```

Os alvos `test-list` e `test-matrix` compilam os binarios de teste em `build/test-list.o` e `build/test-matrix.o`.

## Executar o programa principal

Depois do build:

```bash
./build/main.o <arquivo> <list|matrix> search <start_vertex>
./build/main.o <arquivo> <list|matrix> coloring
./build/main.o <arquivo> <list|matrix> mst
```

Exemplos:

```bash
./build/main.o graph_examples/1.txt list search 0
./build/main.o graph_examples/1.txt matrix coloring
```

### Modos de execução

- `search`: executa `bfs`, `dfs` e `dijkstra` a partir do vértice inicial informado.
- `coloring`: executa `greedy_coloring`, `welsh_powell`, `dsatur` e `brute_force`.
- `mst`: executa o algoritmo de Prim e exibe a árvore geradora mínima, a soma das arestas e o tempo de execução.

O modo `mst` exige um grafo nao direcionado (`is_directed = 0`).

No modo `search`, o argumento `<start_vertex>` é obrigatório.
No modo `coloring`, esse argumento não deve ser informado.

## Formato esperado do arquivo de entrada

Primeira linha:

```text
vertices_count edges_count is_directed is_weighted
```

Linhas seguintes (uma aresta por linha):

- Grafo nao ponderado:

```text
from_vertex to_vertex
```

- Grafo ponderado:

```text
from_vertex to_vertex weight
```

## Executar testes

### MatrixGraph

```bash
make test-matrix
./build/test-matrix.o
```

### ListGraph

```bash
make test-list
./build/test-list.o
```
