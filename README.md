# basic-graph

Projeto em C++ para representar grafos com duas implementacoes:

- `MatrixGraph` (matriz de adjacencia)
- `ListGraph` (lista de adjacencia)

O programa principal lê um arquivo de entrada e cria o tipo de grafo escolhido via argumento.

## Pre-requisitos

- `g++` com suporte a C++17

## Estrutura Principal

- `src/main.cpp`: leitura do arquivo e criacao do grafo
- `src/matrix_graph.cpp` e `src/matrix_graph.h`: implementacao em matriz
- `src/list_graph.cpp` e `src/list_graph.h`: implementacao em lista
- `src/bfs.h`, `src/dfs.h` e `src/dijkstra.h`: algoritmos de busca
- `src/greedy_coloring.h`, `src/welsh.powell.h`: algoritmos de força bruta
- `graph_examples/`: exemplos de arquivos de entrada

## Build com Makefile

Na raiz do projeto, use os alvos do `Makefile`:

```bash
make main
```

Esse comando:

- cria a pasta `build/`
- compila `src/list_graph.cpp` em `build/list_graph.o`
- compila `src/matrix_graph.cpp` em `build/matrix_graph.o`
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
./build/main.o <arquivo> <list|matrix>
```

Uso:

```bash
./build/main.o <arquivo> <list|matrix>
```

Exemplos:

```bash
./build/main.o graph_examples/1.txt list
./build/main.o graph_examples/1.txt matrix
```

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
