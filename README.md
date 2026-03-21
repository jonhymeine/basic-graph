# basic-graph

Projeto em C++ para representar grafos com duas implementacoes:

- `MatrixGraph` (matriz de adjacencia)
- `ListGraph` (lista de adjacencia)

O programa principal le um arquivo de entrada e cria o tipo de grafo escolhido via argumento.

## Pre-requisitos

- `g++` com suporte a C++20

## Estrutura Principal

- `src/main.cpp`: leitura do arquivo e criacao do grafo
- `src/matrix_graph.cpp` e `src/matrix_graph.h`: implementacao em matriz
- `src/list_graph.cpp` e `src/list_graph.h`: implementacao em lista
- `graph_examples/`: exemplos de arquivos de entrada

## Compilar o programa principal

Na raiz do projeto:

```bash
g++ -std=c++20 src/main.cpp src/list_graph.cpp src/matrix_graph.cpp -o main
```

## Executar o programa principal

Uso:

```bash
./main <arquivo> <list|matrix>
```

Exemplos:

```bash
./main graph_examples/1.txt list
./main graph_examples/1.txt matrix
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

### Teste do MatrixGraph

```bash
g++ -std=c++20 src/matrix_graph_test.cpp src/matrix_graph.cpp -o matrix_graph_test
./matrix_graph_test
```

### Teste do ListGraph

```bash
g++ -std=c++20 src/list_graph_test.cpp src/list_graph.cpp -o list_graph_test
./list_graph_test
```
