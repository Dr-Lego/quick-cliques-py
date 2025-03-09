"""
This module provides Python wrappers for the Quick Cliques C++ implementation,
allowing users to find all maximal cliques in a graph using various algorithms.
"""

from typing import List, Literal, Union

import numpy as np

try:
    from quick_cliques.quick_cliques_cpp import adjacency_matrix_to_list
    from quick_cliques.quick_cliques_cpp import find_cliques as cpp_find_cliques
except ImportError:
    raise ImportError(
        "Failed to import C++ extension module. Make sure the package is properly installed."
    )


def find_cliques(
    graph: Union[np.ndarray, List[List[int]]],
    algorithm: Literal["tomita", "adjlist", "degeneracy", "hybrid"] = "degeneracy",
) -> List[List[int]]:
    """
    Run the specified clique finding algorithm on the given graph.

    Args:
        graph: Either an adjacency matrix (numpy array) or adjacency list (list of lists)
        algorithm: One of 'tomita', 'adjlist', 'degeneracy', or 'hybrid'

    Returns:
        List of maximal cliques, where each clique is a list of vertex indices
    """
    if isinstance(graph, np.ndarray):
        if len(graph.shape) != 2 or graph.shape[0] != graph.shape[1]:
            raise ValueError("Adjacency matrix must be a square 2D array")

        # Convert boolean numpy array to adjacency list
        adj_list = adjacency_matrix_to_list(graph)
    else:
        # It's already an adjacency list
        adj_list = graph

        # Validate the adjacency list
        n = len(adj_list)
        for i, neighbors in enumerate(adj_list):
            for j in neighbors:
                if not isinstance(j, int) or j < 0 or j >= n:
                    raise ValueError(
                        f"Invalid neighbor {j} for vertex {i}. Neighbors must be integers in range [0, {n-1}]"
                    )

    # Call the C++ implementation
    return cpp_find_cliques(adj_list, algorithm)
