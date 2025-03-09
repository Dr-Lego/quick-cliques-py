#!/usr/bin/env python3
"""
Example script demonstrating the usage of the Quick Cliques Python module.
"""

import time

import numpy as np

from quick_cliques import find_cliques


def print_cliques(cliques, algorithm_name):
    print(f"{algorithm_name} found {len(cliques)} maximal cliques:")
    for i, clique in enumerate(cliques):
        print(f"  Clique {i+1}: {clique}")
    print(f"  Time: {time.time() - start_time:.6f} seconds\n")


# adjacency matrix
adj_matrix = np.array(
    [
        [0, 1, 1, 0, 0, 0],
        [1, 0, 1, 0, 0, 0],
        [1, 1, 0, 1, 1, 0],
        [0, 0, 1, 0, 1, 1],
        [0, 0, 1, 1, 0, 1],
        [0, 0, 0, 1, 1, 0],
    ],
    dtype=bool,
)


print("\nExpected maximal cliques: [0,1,2], [2,3,4], and [3,4,5]\n")

# Find all maximal cliques using different algorithms
algorithms = {
    "Tomita Algorithm": "tomita",
    "Adjacency List Algorithm": "adjlist",
    "Degeneracy Algorithm": "degeneracy",
    "Hybrid Algorithm": "hybrid",
}

for name, algorithm in algorithms.items():
    start_time = time.time()
    cliques = find_cliques(adj_matrix, algorithm)
    print_cliques(cliques, name)

print("Example completed successfully!")
