/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

/*
    This file contains the Python bindings for the QuickClique library.
    NOTE: I'm not a C++ expert, so please feel free to suggest improvements.
*/

// local includes
#include "Tools.h"
#include "TomitaAlgorithm.h"
#include "AdjacencyListAlgorithm.h"
#include "HybridAlgorithm.h"
#include "DegeneracyAlgorithm.h"

// system includes
#include <list>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <ctime>

// pybind11 includes
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;
using namespace std;

/**
 * Helper function to check if an algorithm name is valid
 */
bool isValidAlgorithm(string const &name) {
    return (name == "tomita" || name == "adjlist" || name == "hybrid" || name == "degeneracy");
}

/**
 * Run a clique finding algorithm on the given graph.
 *
 * @param adjacency_list The adjacency list representation of the graph
 * @param algorithm_name The name of the algorithm to use (tomita, adjlist, degeneracy, hybrid)
 * @return A list of maximal cliques, where each clique is a list of vertex indices
 */
vector<vector<int>> find_cliques(const vector<vector<int>>& adjacency_list, const string& algorithm_name) {
    if (!isValidAlgorithm(algorithm_name)) {
        throw std::invalid_argument("Invalid algorithm name. Must be one of: tomita, adjlist, degeneracy, hybrid");
    }

    int n = adjacency_list.size(); // number of vertices
    int m = 0; // 2x number of edges (will be calculated)

    // Convert Python adjacency list to C++ format
    vector<list<int>> cpp_adjacency_list(n);
    for (int i = 0; i < n; i++) {
        for (int neighbor : adjacency_list[i]) {
            cpp_adjacency_list[i].push_back(neighbor);
            m++;
        }
    }
    m /= 2; // Each edge is counted twice

    Algorithm* pAlgorithm = nullptr;

    // Prepare data structures based on algorithm
    bool const bComputeAdjacencyMatrix(n < 20000);
    bool const bShouldComputeAdjacencyMatrix(algorithm_name == "tomita");

    if (bShouldComputeAdjacencyMatrix && !bComputeAdjacencyMatrix) {
        throw std::runtime_error("Unable to compute adjacency matrix, graph is too large: " + std::to_string(n) + " vertices");
    }

    char** adjacencyMatrix = nullptr;
    vector<vector<char>> vAdjacencyMatrix;

    if (bComputeAdjacencyMatrix) {
        adjacencyMatrix = (char**)Calloc(n, sizeof(char*));
        vAdjacencyMatrix.resize(n);

        for (int i = 0; i < n; i++) {
            adjacencyMatrix[i] = (char*)Calloc(n, sizeof(char));
            vAdjacencyMatrix[i].resize(n);
            for (int const neighbor : cpp_adjacency_list[i]) {
                adjacencyMatrix[i][neighbor] = 1;
                vAdjacencyMatrix[i][neighbor] = 1;
            }
        }
    }

    bool const bComputeAdjacencyArray(algorithm_name == "adjlist");
    vector<vector<int>> adjacencyArray;

    if (bComputeAdjacencyArray) {
        adjacencyArray.resize(n);
        for (int i = 0; i < n; i++) {
            adjacencyArray[i].resize(cpp_adjacency_list[i].size());
            int j = 0;
            for (int const neighbor : cpp_adjacency_list[i]) {
                adjacencyArray[i][j++] = neighbor;
            }
        }
        cpp_adjacency_list.clear(); // Free up memory
    }

    // Create the appropriate algorithm
    if (algorithm_name == "tomita") {
        pAlgorithm = new TomitaAlgorithm(adjacencyMatrix, n);
    } else if (algorithm_name == "adjlist") {
        pAlgorithm = new AdjacencyListAlgorithm(adjacencyArray);
    } else if (algorithm_name == "degeneracy") {
        pAlgorithm = new DegeneracyAlgorithm(cpp_adjacency_list);
    } else if (algorithm_name == "hybrid") {
        pAlgorithm = new HybridAlgorithm(cpp_adjacency_list);
    }

    // Store cliques
    list<list<int>> cliques;

    // Run algorithm and collect cliques
    pAlgorithm->Run(cliques);

    // Convert results to Python format
    vector<vector<int>> result;
    for (const auto& clique : cliques) {
        vector<int> py_clique;
        for (int vertex : clique) {
            py_clique.push_back(vertex);
        }
        result.push_back(py_clique);
    }

    // Clean up
    if (adjacencyMatrix != nullptr) {
        for (int i = 0; i < n; i++) {
            Free(adjacencyMatrix[i]);
        }
        Free(adjacencyMatrix);
    }

    delete pAlgorithm;

    return result;
}

/**
 * Convert a numpy adjacency matrix to an adjacency list
 */
vector<vector<int>> adjacency_matrix_to_list(py::array_t<bool, py::array::c_style | py::array::forcecast> adjacency_matrix) {
    py::buffer_info buf = adjacency_matrix.request();

    if (buf.ndim != 2) {
        throw std::runtime_error("Number of dimensions must be 2");
    }

    if (buf.shape[0] != buf.shape[1]) {
        throw std::runtime_error("Adjacency matrix must be square");
    }

    size_t n = buf.shape[0];
    bool* ptr = static_cast<bool*>(buf.ptr);

    vector<vector<int>> adjacency_list(n);

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if (ptr[i * n + j]) {
                adjacency_list[i].push_back(static_cast<int>(j));
            }
        }
    }

    return adjacency_list;
}

PYBIND11_MODULE(quick_cliques_cpp, m) {
    m.doc() = "Python bindings for Quick Cliques C++ implementation";

    m.def("find_cliques", &find_cliques,
          "Find all maximal cliques in a graph",
          py::arg("adjacency_list"), py::arg("algorithm_name"));

    m.def("adjacency_matrix_to_list", &adjacency_matrix_to_list,
          "Convert an adjacency matrix to an adjacency list",
          py::arg("adjacency_matrix"));
}
