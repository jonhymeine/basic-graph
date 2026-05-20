CXXFLAGS := -std=c++17
SRC_DIR := src
BUILD_DIR := build
UNIT_TESTS_DIR := $(SRC_DIR)/unit_tests
GRAPH_DIR := $(SRC_DIR)/graph

create-build-dir:
	mkdir -p $(BUILD_DIR)

build-list: create-build-dir
	g++ $(CXXFLAGS) -c $(GRAPH_DIR)/list_graph.cpp -o $(BUILD_DIR)/list_graph.o

test-list: build-list
	g++ $(CXXFLAGS) $(UNIT_TESTS_DIR)/list_graph_test.cpp $(BUILD_DIR)/list_graph.o -o $(BUILD_DIR)/test-list.o

build-matrix: create-build-dir
	g++ $(CXXFLAGS) -c $(GRAPH_DIR)/matrix_graph.cpp -o $(BUILD_DIR)/matrix_graph.o

test-matrix: build-matrix
	g++ $(CXXFLAGS) $(UNIT_TESTS_DIR)/matrix_graph_test.cpp $(BUILD_DIR)/matrix_graph.o -o $(BUILD_DIR)/test-matrix.o

main: build-list build-matrix
	g++ $(CXXFLAGS) $(SRC_DIR)/main.cpp $(BUILD_DIR)/list_graph.o $(BUILD_DIR)/matrix_graph.o -o $(BUILD_DIR)/main.o